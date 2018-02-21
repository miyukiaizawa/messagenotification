#include "messagenotification.h"
#include "opencv2\opencv.hpp"
#include "qxtglobalshortcut\qxtglobalshortcut.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QLabel>
#include <QMenu>
#include <QSystemTrayIcon>
#include <QCloseEvent>
#include <QApplication>


MessageNotification::
MessageNotification(QWidget *parent) :
  QMainWindow(parent) {
  ui.setupUi(this);

  ui.mainToolBar->setEnabled(false);
  ui.mainToolBar->setVisible(false);

  centralWidgetLayout = new QVBoxLayout;
  list = new NotificationListView;
  centralWidgetLayout->addWidget(list);

  centralWidget()->setLayout(centralWidgetLayout);

  createActions();
  createTrayIcon();

  QxtGlobalShortcut *shortcut = new QxtGlobalShortcut(this);
  shortcut->setShortcut(QKeySequence("Ctrl+Shift+F12"));

  connect(shortcut, SIGNAL(activated()),
          this, SLOT(showNormal()));

}

MessageNotification::
~MessageNotification() {}

bool
MessageNotification::
createConnetion(DatabaseInfo *dbinfo) {
  return list->createConnetion(dbinfo);
}

void
MessageNotification::
run() {
  list->run();
}


void
MessageNotification::
createTrayIcon() {
  trayIconMenu = new QMenu(this);
  trayIconMenu->addAction(minimizeAction);
  trayIconMenu->addAction(maximizeAction);
  trayIconMenu->addAction(restoreAction);
  trayIconMenu->addSeparator();
  trayIconMenu->addAction(quitAction);

  trayIcon = new QSystemTrayIcon(this);
  trayIcon->setContextMenu(trayIconMenu);
  trayIcon->setIcon(QIcon(":/MessageNotification/icon/list.png"));
  trayIcon->show();
}

void
MessageNotification::
createActions() {


  minimizeAction = new QAction(tr("Minimize"), this);
  connect(minimizeAction, SIGNAL(triggered()), this, SLOT(on_hide()));

  maximizeAction = new QAction(tr("Maximize"), this);
  connect(maximizeAction, SIGNAL(triggered()), this, SLOT(on_showMaximized()));

  restoreAction = new QAction(tr("Restore"), this);
  connect(restoreAction, SIGNAL(triggered()), this, SLOT(on_showNormal()));

  quitAction = new QAction(tr("Quit"), this);
  connect(quitAction, SIGNAL(triggered()), this, SLOT(on_close()));
}


void
MessageNotification::
closeEvent(QCloseEvent *event) {
  if (trayIcon->isVisible()) {
    hide();
    event->ignore();
  }
}



