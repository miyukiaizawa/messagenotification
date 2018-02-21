#pragma once
#ifndef __MESSAGE_LIST_WIDGET_ITEM_WIDGET_H__
#define __MESSAGE_LIST_WIDGET_ITEM_WIDGET_H__

#include <QWidget>
#include <QLayout>
#include <QLabel>
#include <QPushButton>
#include "messageinfo.h"


class MessageListWidgetItemWidget : public QWidget {
  Q_OBJECT
public:
  MessageListWidgetItemWidget(QWidget* parent = 0) :
    QWidget(parent) {

    messageInfo = new MessageInfo;

    eraseButton = new QPushButton;
    eraseButton->setObjectName(QStringLiteral("eraseButton"));
    eraseButton->setText("X");
    eraseButton->setVisible(false);
    connect(messageInfo, SIGNAL(clicked()), this, SLOT(destroy()));

    title = new QLabel;
    title->setObjectName(QStringLiteral("title"));
    connect(messageInfo, SIGNAL(categoryChanged()), this, SLOT(updateCagegory()));

    message = new QLabel;
    message->setObjectName(QStringLiteral("message"));
    connect(messageInfo, SIGNAL(messageChanged()), this, SLOT(updateMessage()));

    showImageButton = new QPushButton;
    showImageButton->setObjectName(QStringLiteral("showImageButton"));
    showImageButton->setText("show");
    showImageButton->setVisible(false);
    connect(messageInfo, SIGNAL(clicked()), this, SLOT(onImageButtonClick()));
    connect(messageInfo, SIGNAL(imageInfoChanged()), this, SLOT(updateImageInfo()));

    line = new QHBoxLayout;
    line->setObjectName(QStringLiteral("line"));
    line->addWidget(message);
    line->addWidget(showImageButton);

    root = new QVBoxLayout;
    root->setObjectName(QStringLiteral("root"));
    root->addWidget(eraseButton, Qt::Alignment::enum_type::AlignRight);
    root->addWidget(title);
    root->addLayout(line);

    this->setLayout(root);
  }

  public slots:
  void requestDestroy(bool destroyWindow = true, bool destroySubWindows = true) {
    emit eraseButtonClicked();
    destroy(destroyWindow, destroySubWindows);
  }

  private slots:
  void updateCagegory() {
    title->setText(messageInfo->category());
  }
  void updateMessage() {
    message->setText(messageInfo->message());
  }
  void updateImageInfo() {
    if (!messageInfo->imageInfo().path().isEmpty()) {
      showImageButton->setVisible(true);
    }
  }
  void onImageButtonClick() {
    emit imageButtonClicked(messageInfo->imageInfo());
  }

signals:
  void eraseButtonClicked();
  void imageButtonClicked(const ImageInfo& info);

public:
  QBoxLayout *root;
  QBoxLayout *line;
  QLabel *title;
  QLabel *message;
  QPushButton *eraseButton;
  QPushButton *showImageButton;

  MessageInfo* messageInfo;

};


#endif //__MESSAGE_LIST_WIDGET_ITEM_WIDGET_H__
