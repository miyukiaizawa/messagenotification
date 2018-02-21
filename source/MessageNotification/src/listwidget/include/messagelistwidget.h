#pragma once
#ifndef __MESSAGE_LIST_WIDGET_H__
#define __MESSAGE_LIST_WIDGET_H__

#include <QListWidget>
#include <QLayout>
#include <QLabel>
#include <QPushButton>
#include <QMessageBox>
#include <QThread>
#include <QEventLoop>

#include "messageinfo.h"
#include "databaseinfo.h"

class MessageListWidget : public QListWidget {
  Q_OBJECT
public:
  MessageListWidget(QWidget *parent = Q_NULLPTR) :
    QListWidget(parent) {}

  QList<QListWidgetItem*> findItems(const MessageInfo &info, Qt::MatchFlags flags) const {

  }


};


#endif //__MESSAGE_LIST_WIDGET_H__
