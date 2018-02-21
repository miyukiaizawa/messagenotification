#include "notificationlistview.h"
#include "opencv2\opencv.hpp"
#include <QTextCodec>
#include <QFileDialog>
#include <QMessageBox>
#include <QLabel>
#include <QThread>
#include <QEventLoop>
#include <QApplication>


ListItemUpdateWorker::
ListItemUpdateWorker(QObject* parent) {}

ListItemUpdateWorker::
~ListItemUpdateWorker() {}

void
ListItemUpdateWorker::
doWork() {
  while (true) {

    emit updateListItems();

    QThread::usleep(sleepTime);
  }
}




NotificationListView::
NotificationListView(QWidget *parent) :
  QWidget(parent) {

  setupMainWidgetLayout(this);

}

NotificationListView::
~NotificationListView() {}

void
NotificationListView::
setupMessageList(QLayout *parent) {

  list = new QListView();
  list->setObjectName(QStringLiteral("messageList"));
  list->setHidden(false);
  list->setEditTriggers(QAbstractItemView::NoEditTriggers);
  list->setPalette(this->palette());
  list->setSpacing(2);
  list->setAutoFillBackground(true);

  listModel = new MessageListItemModel;
  listModel->setObjectName(QStringLiteral("messageListItemModel"));
  list->setModel(listModel);

  listDelegate = new MessageListItemDelegate;
  listDelegate->setObjectName(QStringLiteral("messageListItemDelegate"));
  list->setItemDelegate(listDelegate);
  QListView::connect(list, SIGNAL(clicked(const QModelIndex &)),
                     this, SLOT(itemChecked(const QModelIndex &)), Qt::AutoConnection);
  QListView::connect(list, SIGNAL(clicked(const QModelIndex &)),
                     this, SLOT(updateImageBox(const QModelIndex &)), Qt::AutoConnection);

  parent->addWidget(list);
}

void
NotificationListView::
setupMainWidgetLayout(QWidget *parent) {

  layout = new QVBoxLayout();

  //for erase Button
  eraseButton = new QPushButton;
  eraseButton->setObjectName(QStringLiteral("eraseButton"));
  eraseButton->setText(QStringLiteral("Erase Message"));
  QListView::connect(eraseButton, SIGNAL(clicked()),
                     this, SLOT(eraseCurrentItem()));
  layout->addWidget(eraseButton);

  //add messagelist
  setupMessageList(layout);
  parent->setLayout(layout);

  imageboxName = "image";
  imagebox = new ImageBox(imageboxName);

  framelessImageBox = new FramelessWindow;
  framelessImageBox->setWindowTitle("Image");
  framelessImageBox->setMinimumSize(QSize(480, 320));
  framelessImageBox->setContent(imagebox);
  framelessImageBox->setHidden(true);
}


bool
NotificationListView::
createConnetion(DatabaseInfo *dbinfo) {
  QSqlDatabase db = QSqlDatabase::addDatabase(dbinfo->databaseType);
  db.setHostName(dbinfo->hostName);
  db.setDatabaseName(dbinfo->databaseName);
  db.setUserName(dbinfo->userName);
  db.setPassword(dbinfo->password);
  if (!db.open()) {
    QMessageBox::critical(0, QObject::tr("Failed to open Database. "), db.lastError().text());
    return false;
  }
  return true;
}

void
NotificationListView::
run() {
  thread = new QThread;
  updateWorker = new ListItemUpdateWorker;
  updateWorker->listModel = listModel;
  updateWorker->sleepTime = 100;
  updateWorker->moveToThread(thread);
  connect(thread, SIGNAL(started()), updateWorker, SLOT(doWork()));
  connect(updateWorker, SIGNAL(workFinished()), thread, SLOT(quit()));
  //automatically delete thread and task object when work is done:
  connect(thread, SIGNAL(finished()), updateWorker, SLOT(deleteLater()));
  connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));

  connect(updateWorker, SIGNAL(updateListItems()), this, SLOT(updateListItems()));

  thread->start();
}

void
NotificationListView::
updateListItems() {

  //データベースを読み込んできて、データをダウンロードする
  //todo : 表示件数を日時が遅い順に並べて、表示最大件数を決める。
  QSqlQuery qry("SELECT * FROM Log ORDER BY ID DESC");

  //古いセレクトアイテムのインデックスを持ってくる
  auto oldselectedIndex = list->selectionModel()->currentIndex();

  QList<MessageInfo*> messages;
  while (qry.next()) {
    auto message = new MessageInfo(qry.value("ID").toInt(),
                                   qry.value("Category").toString(),
                                   qry.value("Date").toDateTime(),
                                   qry.value("Message").toString(),
                                   ImageInfo(qry.value("ImagePath").toString()),
                                   qry.value("Checked").toBool());
    messages.push_back(message);
  }

  listModel->assainItems(messages);

  adjustSelectionitem(oldselectedIndex);
}

void
NotificationListView::
adjustSelectionitem(const QModelIndex& index) {

  auto info = messageInfo(index);
  auto listIndex = listModel->findChildIndex(info);
  if (!listIndex.row() >= 0) {
    if (listIndex.row() != index.row()) {
      list->selectionModel()->
        setCurrentIndex(listIndex,
                        QItemSelectionModel::SelectionFlag::ClearAndSelect);
    }
  }
  else {
    list->clearSelection();
  }
}

void
NotificationListView::
eraseCurrentItem() {

  //削除でエラー出ないようにエラーチェックして
  auto info = messageInfo(list->currentIndex());
  if (info == nullptr) {
    return;
  }

  //データベースに項目の削除を申請
  QSqlQuery qry(
    "DELETE FROM log \
     WHERE ID = " + QString::number(info->id())
  );

  if (!qry.exec()) {
    //error message
    return;
  }

  //データベースから消えたら、画像データも削除する
  QFile file;
  file.setFileName(info->imageInfo().path());
  if (!file.remove()) {
    //画像がロックされたりして削除できないとき
    //通常参照されながらプログラム動かしたりしないので
    //とりあえずほっとく
  }
  //成功したら表示上も削除する
  listModel->eraseItem(list->currentIndex());

  //セレクトの削除
  list->clearSelection();

}

void
NotificationListView::
itemChecked(const QModelIndex & index) {
  // mutex_.lock();
  //削除でエラー出ないようにエラーチェックして
  auto info = messageInfo(list->currentIndex());
  if (info == nullptr) {
    return;
  }

  //データベースに項目を確認したことを通知
  QSqlQuery qry(
    "UPDATE log \
      SET Checked = 'true' \
     WHERE ID = " + QString::number(info->id())
  );

  if (!qry.exec()) {
    //error message
    return;
  }

  //データベースの更新が完了したら表示も更新
  info->setChecked(true);
  //mutex_.unlock();
}

void
NotificationListView::
updateImageBox(const QModelIndex & index) {
  mutex_.lock();
  auto imageInfo = ::imageInfo(index);
  if (!imageInfo.path().isEmpty()) {

    std::string stlpath = imageInfo.path().toLocal8Bit();
    cv::Mat image = cv::imread(stlpath);

    if (!image.empty()) {
      //todo : windowの位置をメッセンジャーの隣に移動させとく

      if (!imagebox->isVisible()) {
        auto w = QApplication::activeWindow();
        framelessImageBox->move(w->pos().x() + w->size().width(), w->pos().y());
      }
      imagebox->showImage(image);
      framelessImageBox->setHidden(false);
      framelessImageBox->show();
    }
  }
  else if (!imageInfo.image().size().isEmpty()) {

  }
  else {
    framelessImageBox->setHidden(true);
    framelessImageBox->close();
  }

  mutex_.unlock();

}