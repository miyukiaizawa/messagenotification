#pragma once
#ifndef __IMAGE_BOX_H__
#define __IMAGE_BOX_H__
#include <QMainWindow>
#include <QApplication>
#include <QBOXLayout>
#include <QMenuBar>
#include <QToolBar>
#include <QStatusBar>
#include <QAction>
#include <QLabel>
#include "FrameLabel.h"
#include "window_QT.h"
#include "MatToQImage.h"

class ImageBox : public QMainWindow {
  Q_OBJECT
public:

  ImageBox(const QString& name, QWidget* parent = 0) :
    QMainWindow(parent) {
    setupUi(this);


    imageBox = new CvWindow(name);

    imageBox->myToolBar->setHidden(true);
    foreach(QAction *a, imageBox->vect_QActions)
      mainToolBar->addAction(a);
    mainToolBar->setFloatable(false);
    mainToolBar->setMovable(true);
    QObject::connect(mainToolBar, SIGNAL(visibilityChanged(bool)), this, SLOT(updateToolBoxAction(bool)));


    hideToolbarAction = new QAction(QIcon(""), "Hide ToolBar", this);
    hideToolbarAction->setIconVisibleInMenu(false);
    QObject::connect(hideToolbarAction, SIGNAL(triggered()), this, SLOT(hideToolBox()));

    displayToolBarAction = new QAction(QIcon(""), "Display ToolBar", this);
    displayToolBarAction->setIconVisibleInMenu(false);
    QObject::connect(displayToolBarAction, SIGNAL(triggered()), this, SLOT(displayToolBox()));

    imageBox->myStatusBar->setHidden(true);
    statusBarMsg = imageBox->myStatusBar_msg;
    statusBar->addWidget(imageBox->myStatusBar_msg);

    layout = new QVBoxLayout;
    layout->addWidget(imageBox);

    centralWidget->setLayout(layout);
  }

  void setupUi(QMainWindow* win) {
    win->setWindowTitle(QApplication::translate("ImageClass", "Image", nullptr));
    if (win->objectName().isEmpty())
      win->setObjectName(QStringLiteral("ImageClass"));

    win->resize(600, 400);

    menuBar = new QMenuBar(win);
    menuBar->setObjectName(QStringLiteral("menuBar"));
    win->setMenuBar(menuBar);

    mainToolBar = new QToolBar(win);
    mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
    win->addToolBar(mainToolBar);

    centralWidget = new QWidget(win);
    centralWidget->setObjectName(QStringLiteral("centralWidget"));
    win->setCentralWidget(centralWidget);

    statusBar = new QStatusBar(win);
    statusBar->setObjectName(QStringLiteral("statusBar"));
    win->setStatusBar(statusBar);

  }

  void showImage(cv::Mat& image) {
    CvMat cimage = image;
    imageBox->updateImage(&cimage);
  }

  private slots:

  void updateToolBoxAction(bool visible) {
    if (visible) {
      imageBox->vect_QActions.removeOne(displayToolBarAction);
      imageBox->vect_QActions.push_back(hideToolbarAction);
    }
    else {
      imageBox->vect_QActions.removeOne(hideToolbarAction);
      imageBox->vect_QActions.push_back(displayToolBarAction);
    }
  }

  void displayToolBox() {
    mainToolBar->setHidden(false);
    if (imageBox->vect_QActions.removeOne(displayToolBarAction))
      imageBox->vect_QActions.push_back(hideToolbarAction);
  }
  
  void hideToolBox() {
    mainToolBar->setHidden(true);
    if (imageBox->vect_QActions.removeOne(hideToolbarAction))
      imageBox->vect_QActions.push_back(displayToolBarAction);
  }


public:
  QFrame *line;
  QMenuBar* menuBar;
  QToolBar* mainToolBar;
  QWidget* centralWidget;
  QStatusBar* statusBar;

  QLabel* statusBarMsg;
  CvWindow* imageBox;

  QBoxLayout* layout;

  QAction* hideToolbarAction;
  QAction* displayToolBarAction;
};


#endif //__IMAGE_BOX_H__
