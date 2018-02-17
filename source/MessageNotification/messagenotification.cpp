#include "messagenotification.h"
#include "opencv2\opencv.hpp"
#include <QTextCodec>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QFileDialog>
#include <QMessageBox>
#include <QLabel>
#include <QThread>
#include <QEventLoop>


ListItemUpdateWorker::
ListItemUpdateWorker(QObject* parent) {}

ListItemUpdateWorker::
~ListItemUpdateWorker() {}

void
ListItemUpdateWorker::
doWork() {
  while (true) {

    emit updateListItems();
    //emit listModelChanged(listModel);

    QThread::sleep(sleepTime);
  }
}


MessageNotification::
MessageNotification(QWidget *parent) :
  QMainWindow(parent) {

  ui.setupUi(this);
  setupMainWidgetLayout(this->centralWidget());

}

MessageNotification::
~MessageNotification() {}


void
MessageNotification::
setupMessageList(QLayout *parent) {

  list = new QListView();
  list->setObjectName(QStringLiteral("messageList"));
  list->setHidden(false);
  list->setEditTriggers(QAbstractItemView::NoEditTriggers);

  listModel = new MessageListItemModel;
  listModel->setObjectName(QStringLiteral("messageListItemModel"));
  list->setModel(listModel);

  listDelegate = new MessageListItemDelegate;
  listDelegate->setObjectName(QStringLiteral("messageListItemDelegate"));
  list->setItemDelegate(listDelegate);
  QListView::connect(list, SIGNAL(clicked(const QModelIndex &)),

                     this, SLOT(itemChecked(const QModelIndex &)));
  QListView::connect(list, SIGNAL(clicked(const QModelIndex &)),
                     this, SLOT(updateImageBox(const QModelIndex &)));

  parent->addWidget(list);
}

void
MessageNotification::
setupMainWidgetLayout(QWidget *parent) {

  centralWidgetLayout = new QVBoxLayout();

  //for erase Button
  eraseButton = new QPushButton;
  eraseButton->setObjectName(QStringLiteral("eraseButton"));
  eraseButton->setText(QStringLiteral("Erase"));
  QListView::connect(eraseButton, SIGNAL(clicked()),
                     this, SLOT(eraseCurrentItem()));
  centralWidgetLayout->addWidget(eraseButton);

  //add messagelist
  setupMessageList(centralWidgetLayout);

  parent->setLayout(centralWidgetLayout);

}


bool
MessageNotification::
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
MessageNotification::
run() {
  thread = new QThread;
  updateWorker = new ListItemUpdateWorker;
  updateWorker->listModel = listModel;
  updateWorker->sleepTime = 1;
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
MessageNotification::
updateListItems() {

  //�f�[�^�x�[�X��ǂݍ���ł��āA�f�[�^���_�E�����[�h����
  //todo : �\��������������x�����ɕ��ׂāA�\���ő匏�������߂�B
  QSqlQuery qry("SELECT * FROM Log ORDER BY ID DESC");

  listModel->clearItems();

  while (qry.next()) {
    listModel->addItem(new MessageInfo(qry.value("ID").toInt(),
                                       qry.value("Category").toString(),
                                       qry.value("Date").toDateTime(),
                                       qry.value("Message").toString(),
                                       ImageInfo(qry.value("ImagePath").toString()),
                                       qry.value("Checked").toBool()));

    auto item = new QListWidgetItem;
  }
}

void
MessageNotification::
eraseCurrentItem() {

  //�폜�ŃG���[�o�Ȃ��悤�ɃG���[�`�F�b�N����
  auto info = messageInfo(list->currentIndex());
  if (info == nullptr) {
    return;
  }

  //�f�[�^�x�[�X�ɍ��ڂ̍폜��\��
  QSqlQuery qry(
    "DELETE FROM log \
     WHERE ID = " + QString::number(info->id())
  );

  if (!qry.exec()) {
    //error message
    return;
  }

  //����������\������폜����
  listModel->eraseItem(list->currentIndex());
}

void
MessageNotification::
itemChecked(const QModelIndex & index) {

  //�폜�ŃG���[�o�Ȃ��悤�ɃG���[�`�F�b�N����
  auto info = messageInfo(list->currentIndex());
  if (info == nullptr) {
    return;
  }

  //�f�[�^�x�[�X�ɍ��ڂ��m�F�������Ƃ�ʒm
  QSqlQuery qry(
    "UPDATE log \
      SET Checked = 'true' \
     WHERE ID = " + QString::number(info->id())
  );

  if (!qry.exec()) {
    //error message
    return;
  }

  //�f�[�^�x�[�X�̍X�V������������\�����X�V
  info->setChecked(true);

}

void
MessageNotification::
updateImageBox(const QModelIndex & index) {
  auto imageInfo = ::imageInfo(index);
  if (!imageInfo.path().isEmpty()) {

    std::string stlpath = imageInfo.path().toLocal8Bit();
    cv::Mat image = cv::imread(stlpath);

    if (!image.empty()) {
      image_box_name = "image";
      //todo : window�̈ʒu�����b�Z���W���[�ׂ̗Ɉړ������Ƃ�
      cv::imshow(image_box_name, image);
      cv::waitKey(1);
    }
  }
  else if (!imageInfo.image().size().isEmpty()) {

  }
  else {
    if (!image_box_name.empty()) {
      cv::destroyWindow(image_box_name);
      image_box_name.clear();
    }
  }



}