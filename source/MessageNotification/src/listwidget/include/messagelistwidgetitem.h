#pragma once
#ifndef __MESSAGE_LIST_WIDGET_ITEM_H__
#define __MESSAGE_LIST_WIDGET_ITEM_H__
#include <qlistwidget.h> 
#include "messageinfo.h"

class MessageListWidgetItem : public QListWidgetItem {
  friend class MessageListModel;
  friend class MessageListWidget;
public:
  enum ItemType {
    CetegoryType = UserType + 1,
    MessageType,
    ImageInfoType,
    CheckedType
  };

  MessageListWidgetItem(QListWidget *view = Q_NULLPTR,
                        int type = Type) :
    messageInfo() {}

  MessageListWidgetItem(const MessageInfo& info,
                        QListWidget *view = Q_NULLPTR,
                        int type = Type) :
    messageInfo(info) {}

  MessageListWidgetItem(const QIcon &icon,
                        const MessageInfo& info,
                        QListWidget *view = Q_NULLPTR,
                        int type = Type) :
    messageInfo(info) {}

  QVariant data(int role) const {

    //custom type
    switch (role) {
    case CetegoryType: {
      return QVariant::fromValue(messageInfo.category());
    } return QVariant();

    case MessageType: {
      return QVariant::fromValue(messageInfo.message());
    } return QVariant();

    case ImageInfoType: {
      return QVariant::fromValue(messageInfo.imageInfo());
    } return QVariant();

    case CheckedType: {
      return QVariant::fromValue(messageInfo.checked());
    } return QVariant();

    }

    return QVariant();
  }

  void setData(int role, const QVariant &value) {

    //custom type
    switch (role) {
    case CetegoryType: {
      auto category = qvariant_cast<QString>(value);
      messageInfo.setCategory(category);
    } return;
    case MessageType: {
      auto message = qvariant_cast<QString>(value);
      messageInfo.setMessage(message);
    } return;
    case ImageInfoType: {
      auto imageInfo = qvariant_cast<ImageInfo>(value);
      messageInfo.setImageInfo(imageInfo);
    } return;
    case CheckedType: {
      auto checked = qvariant_cast<bool>(value);
      messageInfo.setChecked(checked);
    } return;

    }

  }


  void setMessageInfo(const MessageInfo& info) {
    messageInfo.setCategory(info.category());
    messageInfo.setMessage(info.message());
    messageInfo.setImageInfo(info.imageInfo());
    messageInfo.setChecked(info.checked());
  }

private:
  MessageInfo messageInfo;

};

#endif //__MESSAGE_LIST_WIDGET_ITEM_H__
