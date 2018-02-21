#ifndef MESSAGENOTIFICATION_H
#define MESSAGENOTIFICATION_H

#include <QtWidgets/QMainWindow>
#include "ui_messagenotification.h"
#include "notificationlistview.h"

class QSystemTrayIcon;
class QMenu;

class MessageNotification : public QMainWindow {
  Q_OBJECT
public:
  MessageNotification(QWidget *parent = 0);
  ~MessageNotification();

  bool createConnetion(DatabaseInfo *dbinfo);
  void run();

protected:
  void closeEvent(QCloseEvent *);

private:
  void createTrayIcon();
  void createActions();

signals:
  void hideWindow();
  void showMaximizedWindow();
  void showNormalWindow();
  void closeWindow();
  private slots:
  void on_hide() {
    hide();
    emit hideWindow();
  }
  void on_showMaximized() {
    showMinimized();
    emit showMaximizedWindow();

  }
  void on_showNormal() {
    showNormal();
    emit showNormalWindow();
  }
  void on_close() {
    close();
    emit closeWindow();
  }



private:
  Ui::MessageNotificationClass ui;
  QBoxLayout *centralWidgetLayout;
public:
  NotificationListView *list;
private:
  QSystemTrayIcon* trayIcon;
  QMenu* trayIconMenu;
  QAction *minimizeAction;
  QAction *maximizeAction;
  QAction *restoreAction;
  QAction *quitAction;

};

#endif // MESSAGENOTIFICATION_H
