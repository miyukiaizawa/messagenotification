#ifndef MESSAGENOTIFICATION_H
#define MESSAGENOTIFICATION_H

#include <QtWidgets/QMainWindow>
#include "ui_messagenotification.h"
#include "databaseinfo.h"
#include "messagelistitemmodel.h"
#include "messagelistitemdelegate.h"
#include <QListView>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QListWidget>

class ListItemUpdateWorker : public QObject {
  Q_OBJECT
public:
  ListItemUpdateWorker(QObject* parent = 0);
  ~ListItemUpdateWorker();
  public slots:
  void doWork();
signals:
  void updateListItems();
  void workFinished();
public:
  MessageListItemModel* listModel;
  int sleepTime;
};

class MessageNotification : public QMainWindow {
  Q_OBJECT
public:
  MessageNotification(QWidget *parent = 0);
  ~MessageNotification();

  public slots:
  bool createConnetion(DatabaseInfo *dbinfo);
  void run();

  private slots:
  void updateListItems();
  void eraseCurrentItem();
  void itemChecked(const QModelIndex & index);

  void updateImageBox(const QModelIndex & index);

private:
  void setupMainWidgetLayout(QWidget *parent);
  void setupMessageList(QLayout *parent);

private:
  Ui::MessageNotificationClass ui;

  QVBoxLayout *centralWidgetLayout;

  QListView *list;
  MessageListItemModel* listModel;
  MessageListItemDelegate *listDelegate;
  std::string image_box_name;

  QPushButton *eraseButton;

  QThread *thread;
  ListItemUpdateWorker* updateWorker;
};

#endif // MESSAGENOTIFICATION_H
