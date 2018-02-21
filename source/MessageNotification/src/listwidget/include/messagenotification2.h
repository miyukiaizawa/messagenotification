#pragma once
#ifndef __MESSAGE_NOTIFICATION2_H__
#define __MESSAGE_NOTIFICATION2_H__
#include <QtWidgets/QMainWindow>
#include "ui_messagenotification.h"
#include <QListWidget>
#include <QLayout>
#include <QMessageBox>
#include <QThread>
#include <QEventLoop>
#include "messagelistwidget.h"
#include "messagelistwidgetitem.h"
#include "messagelistwidgetitemwidget.h"

class ListItemUpdateWorker2 : public QObject {
  Q_OBJECT
public:
  ListItemUpdateWorker2(QObject* parent = 0) {}
  ~ListItemUpdateWorker2() {}
  public slots :
    void doWork() {
    while (true) {

      emit updateListItems();
      //emit listModelChanged(listModel);

      QThread::sleep(sleepTime);
    }
  }

signals:
  void updateListItems();
  void workFinished();
public:
  int sleepTime;
};

class MessageNotification2 : public QMainWindow {
  Q_OBJECT
public:

  MessageNotification2(QWidget* parent = 0) :
    QMainWindow(parent) {

    ui.setupUi(this);
    auto list = new QListWidget;
    auto test_data = new MessageInfo(0,
                                     tr("alert"),
                                     QDateTime::currentDateTime(),
                                     tr("message"),
                                     ImageInfo("E:\\test.testjpg"),
                                     false);
    auto widget = new MessageListWidgetItemWidget;
    auto item = new MessageListWidgetItem(*test_data, list);
    item->setSizeHint(widget->sizeHint());
    widget->messageInfo->setCategory(test_data->category());
    widget->messageInfo->setMessage(test_data->message());
    widget->messageInfo->setDate(test_data->date());
    widget->messageInfo->setImageInfo(test_data->imageInfo());
    widget->messageInfo->setChecked(test_data->checked());

    list->addItem(item);
    list->setItemWidget(item, widget);

    auto layout = new QVBoxLayout;
    layout->addWidget(list);
    centralWidget()->setLayout(layout);

  }

  bool createConnetion(DatabaseInfo *dbinfo) {
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

  void run() {
    thread = new QThread;
    updateWorker = new ListItemUpdateWorker2;
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

  private slots:
  void updateListItems() {

    //データベースを読み込んできて、データをダウンロードする
    //todo : 表示件数を日時が遅い順に並べて、表示最大件数を決める。
    QSqlQuery qry("SELECT * FROM Log ORDER BY ID DESC Limit " + QString::number(limit_size));

    int index = 0;
    while (qry.next()) {
      auto id = qry.value("ID").toInt();
      auto category = qry.value("Category").toString();
      auto date = qry.value("Date").toDateTime();
      auto message = qry.value("Message").toString();
      auto imageInfo = ImageInfo(qry.value("ImagePath").toString());
      auto checked = qry.value("Checked").toBool();

      if (!messageInfos.empty() && messageInfos[index] != nullptr) {
        messageInfos[index]->setId(id);
        messageInfos[index]->setCategory(category);
        messageInfos[index]->setDate(date);
        messageInfos[index]->setMessage(message);
        messageInfos[index]->setImageInfo(imageInfo);
        index++;
      }
      else {
        auto new_message = new MessageInfo(id, category, date, message, imageInfo, checked);
        messageInfos.push_back(new_message);
      }

    }



    for (auto& info : messageInfos) {

    }
  }

  void setItemWidget() {
    auto item = new QListWidgetItem;
    auto widget = new MessageListWidgetItemWidget;
    list->setItemWidget(item, widget);

  }

private:
  Ui::MessageNotificationClass ui;

  QListWidget* list;
  QThread *thread;
  ListItemUpdateWorker2* updateWorker;
  QVector<MessageInfo*> messageInfos;
  int limit_size;
};

#endif //__MESSAGE_NOTIFICATION2_H__
