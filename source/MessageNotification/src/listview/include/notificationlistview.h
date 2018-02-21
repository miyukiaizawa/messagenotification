#ifndef __NOTIFICATION_LIST_VIEW_H__
#define __NOTIFICATION_LIST_VIEW_H__

#include <QtWidgets/QMainWindow>
#include "databaseinfo.h"
#include "messagelistitemmodel.h"
#include "messagelistitemdelegate.h"
#include <QListView>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QListWidget>
#include "ImageBox.h"
#include "framelesswindow.h"

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

class NotificationListView : public QWidget {
  Q_OBJECT
public:
  NotificationListView(QWidget *parent = 0);
  ~NotificationListView();

  public slots:
  bool createConnetion(DatabaseInfo *dbinfo);
  void run();
  void updateListItems();
  void eraseCurrentItem();
  void itemChecked(const QModelIndex & index);
  void updateImageBox(const QModelIndex & index);

private:
  void setupMainWidgetLayout(QWidget *parent);
  void setupMessageList(QLayout *parent);

  void adjustSelectionitem(const QModelIndex& index);

private:
  QMutex mutex_;
  QVBoxLayout *layout;

  QListView *list;
  MessageListItemModel* listModel;
  MessageListItemDelegate *listDelegate;
  QString imageboxName;

  QPushButton *eraseButton;

  QThread *thread;
  ListItemUpdateWorker* updateWorker;

public:
  FramelessWindow* framelessImageBox;
  ImageBox* imagebox;

};

#endif // __NOTIFICATION_LIST_VIEW_H__
