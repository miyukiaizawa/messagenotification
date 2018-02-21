#pragma once
#ifndef __MESSAGE_LIST_ITEM_MODEL_H__
#define __MESSAGE_LIST_ITEM_MODEL_H__

#include <QAbstractItemModel>
#include "messageinfo.h"

class MessageListItemModel : public QAbstractItemModel {
  Q_OBJECT

public:

  MessageListItemModel(QObject *parent = 0);

  ~MessageListItemModel();

  QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
  QModelIndex parent(const QModelIndex &child) const override;
  int rowCount(const QModelIndex &parent = QModelIndex()) const override;
  int columnCount(const QModelIndex &parent = QModelIndex()) const override;

  QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
  Qt::ItemFlags flags(const QModelIndex &index) const override;
  bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
  QHash<int, QByteArray> roleNames() const;


  void addItem(MessageInfo* obj);
  QModelIndex findChildIndex(MessageInfo* obj);
  void insertItem(QModelIndex index, MessageInfo* obj);
  void eraseItem(const QModelIndex &index);
  MessageInfo* getItem(const QModelIndex &index);
  void assainItems(const QList<MessageInfo*>& items, bool greater_sort = true);

private:
  void eraseDuplicatedItem(const QList<MessageInfo*>& items);
  void updateNewItem(const QList<MessageInfo*>& items);
  void requestDataChanged();

private:
  QList<MessageInfo*> datalist;
};

enum ItemRoles {
  MessageInfoRole = Qt::UserRole + 1,
  IconRole,
  IconSizeRole,
  CategoryRole,
  DateRole,
  MessageRole,
  ImageInfoRole,
  CheckedRole,
  MessageColorRole,
  ItemBackGroundRole,
};

inline MessageInfo* messageInfo(const QModelIndex& index) {
  return qvariant_cast<MessageInfo*>(index.data(ItemRoles::MessageInfoRole));
}
inline QString category(const QModelIndex& index) {
  return qvariant_cast<QString>(index.data(ItemRoles::CategoryRole));
}
inline QDateTime date(const QModelIndex& index) {
  return qvariant_cast<QDateTime>(index.data(ItemRoles::DateRole));
}
inline QString message(const QModelIndex& index) {
  return qvariant_cast<QString>(index.data(ItemRoles::MessageRole));
}
inline ImageInfo imageInfo(const QModelIndex& index) {
  return qvariant_cast<ImageInfo>(index.data(ItemRoles::ImageInfoRole));
}
inline bool checked(const QModelIndex& index) {
  return qvariant_cast<bool>(index.data(ItemRoles::CheckedRole));
}

#endif //__MESSAGE_LIST_ITEM_MODEL_H__
