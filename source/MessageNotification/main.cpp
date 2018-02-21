#include <QtWidgets/QApplication>
#include <filesystem>
#include <QTextCodec>
#include <QPalette>
#include <QDir>

#include "notificationlistview.h"
#include "messagenotification.h" 
#include "DarkStyle.h"
#include "framelesswindow.h"
#include "opencv2\opencv.hpp"
#include "ImageBox.h"

int main(int argc, char *argv[]) {

  QDir exe_path(argv[0]);
  exe_path.cdUp();
  QDir pwd = exe_path.path();
  QDir dbname = pwd.path() + QStringLiteral("/Logs.db");

  QApplication app(argc, argv);
  app.setStyle(new DarkStyle);

  DatabaseInfo dbinfo("QSQLITE", "localhost", dbname.path(), "", "");

  FramelessWindow framelessWindow;

  auto notification = new MessageNotification();
  if (!notification->createConnetion(&dbinfo)) {
    //DB“Ç‚Ýž‚Ý’¼‚·‚æ‚¤‚É‚·‚éŽ–B
  }

  notification->run();

  framelessWindow.setWindowTitle("Notification");
  framelessWindow.setMinimumSize(QSize(480, 320));
  framelessWindow.setContent(notification);
  framelessWindow.show();
  //framelessWindow.setWindowFlag(Qt::Tool, true);
  framelessWindow.setWindowIcon(QIcon(":/MessageNotification/icon/list.png"));


  QObject::connect(notification, SIGNAL(hideWindow()), &framelessWindow, SLOT(on_minimizeButton_clicked()));

  QObject::connect(notification, &MessageNotification::hideWindow, [&framelessWindow]() {
    framelessWindow.setWindowFlag(Qt::SubWindow, true);
  });
  QObject::connect(notification, &MessageNotification::showMaximizedWindow, [&framelessWindow]() {
    framelessWindow.setWindowFlag(Qt::SubWindow, false);
    framelessWindow.show();
  });
  QObject::connect(notification, &MessageNotification::showNormalWindow, [&framelessWindow]() {
    framelessWindow.setWindowFlag(Qt::SubWindow, false);
    framelessWindow.show();
  });
  QObject::connect(notification, SIGNAL(showMaximizedWindow()), &framelessWindow, SLOT(on_maximizeButton_clicked()));
  QObject::connect(notification, SIGNAL(showNormalWindow()), &framelessWindow, SLOT(on_restoreButton_clicked()));
  QObject::connect(notification, SIGNAL(closeWindow()), &framelessWindow, SLOT(on_closeButton_clicked()));




  notification->list->imagebox->statusBarMsg->setHidden(true);


  return app.exec();
}
