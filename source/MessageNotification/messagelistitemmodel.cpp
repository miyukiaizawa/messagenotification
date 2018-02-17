#include "messagelistitemmodel.h"
#include <QFont>
#include <QBrush>
#include <QDebug>

MessageListItemModel::
MessageListItemModel(QObject *parent)
  : QAbstractItemModel(parent) {}

MessageListItemModel::
~MessageListItemModel() {}

QModelIndex
MessageListItemModel::
index(int row, int column, const QModelIndex &parent) const {
  return row < datalist.size() ? createIndex(row, column, datalist.at(row)) : QModelIndex();
}

QModelIndex
MessageListItemModel::
parent(const QModelIndex &child) const {
  return QModelIndex();
}

int
MessageListItemModel::
rowCount(const QModelIndex &parent) const {
  return datalist.size();
}

int
MessageListItemModel::
columnCount(const QModelIndex &parent) const {
  return 4;
}

QVariant
MessageListItemModel::
data(const QModelIndex &index, int role) const {

  int row = index.row();
  int col = index.column();

  if (row < 0 || datalist.size() <= row) {
    return QVariant();
  }

  MessageInfo* entry = (MessageInfo*)datalist[row];

  switch (role) {

  case Qt::DisplayRole: {
  } return QVariant();

  case Qt::FontRole: {
    QFont boldFont;
    boldFont.setBold(true);
    boldFont.setPointSize(18);
    return boldFont;
  } return QVariant();

  case Qt::BackgroundRole: {
    if (!entry->checked()) {
      return QBrush(Qt::yellow);
    }
  } return QBrush(Qt::white);

  case Qt::TextAlignmentRole: {

  } return Qt::AlignCenter + Qt::AlignVCenter;

  case Qt::CheckStateRole: {

    //if (row == 1 && col == 0) //add a checkbox to cell(1,0)
    //{
    //  return Qt::Checked;
    //}
  } return QVariant();

  case Qt::UserRole: {

    return QVariant::fromValue(entry->category() + entry->date().toString() + entry->message());

  } return QVariant();

  case ItemRoles::MessageColorRole: {
    if (entry->category() == "alert") {
      return QBrush(Qt::red);
    }
  } return QBrush(Qt::black);

  case ItemRoles::CategoryRole: {
  } return QVariant::fromValue(entry->category());

  case ItemRoles::DateRole: {
  } return QVariant::fromValue(entry->date());

  case ItemRoles::MessageRole: {
  } return QVariant::fromValue(entry->message());

  case ItemRoles::ImageInfoRole: {
  } return QVariant::fromValue(entry->imageInfo());
  case ItemRoles::CheckedRole: {
  } return QVariant::fromValue(entry->checked());
  case ItemRoles::MessageInfoRole: {
  }return QVariant::fromValue(entry);

  }

  return QVariant();
}

Qt::ItemFlags
MessageListItemModel::
flags(const QModelIndex &index) const {
  return QAbstractItemModel::flags(index);
}

bool
MessageListItemModel::
setData(const QModelIndex &index, const QVariant &value, int role) {
  //1列目の場合だけcheckListを更新してチェック状態を更新する
  if (index.isValid() && index.column() == 0 && role == Qt::CheckStateRole) {
    emit dataChanged(index, index);
    return true;
  }
  return false;
}

QHash<int, QByteArray>
MessageListItemModel::
roleNames() const {
  QHash<int, QByteArray> roles;
  roles[CategoryRole] = "category";
  roles[DateRole] = "date";
  roles[MessageRole] = "message";
  roles[ImageInfoRole] = "imageInfo";
  return roles;
}

void
MessageListItemModel::
addItem(MessageInfo* obj) {
  datalist.append(obj);
  emit dataChanged(createIndex(0, 0, datalist[0]),
                   createIndex(datalist.size() - 1, 0, datalist[datalist.size() - 1]));
}

void
MessageListItemModel::
requestDataChanged() {
  if (datalist.empty()) {
    emit dataChanged(createIndex(0, 0), createIndex(0, 0));
  }
  else {
    emit dataChanged(createIndex(0, 0, datalist[0]),
                     createIndex(datalist.size() - 1, 0, datalist[datalist.size() - 1]));
  }
}

void
MessageListItemModel::
insertItem(QModelIndex index, MessageInfo* obj) {
  datalist.insert(index.row(), obj);
  emit dataChanged(createIndex(0, 0, datalist[0]),
                   createIndex(datalist.size() - 1, 0, datalist[datalist.size() - 1]));
}

void
MessageListItemModel::
eraseItem(const QModelIndex &index) {
  datalist.erase(datalist.begin() + index.row());


  auto beginIndex = 0;
  auto endIndex = (datalist.size() == 0) ? 0 : datalist.size() - 1;

  if (datalist.size() == 0) {
    emit dataChanged(createIndex(0, 0),
                     createIndex(endIndex - 1, 0));
  }
  else {
    emit dataChanged(createIndex(0, 0, datalist[beginIndex]),
                     createIndex(endIndex - 1, 0, datalist[endIndex]));

  }
}

MessageInfo*
MessageListItemModel::
getItem(const QModelIndex &index) {
  if (datalist.empty() || index.row() < 0 || datalist.size() <= index.row()) {
    return nullptr;
  }
  return datalist[index.row()];
}

void
MessageListItemModel::
clearItems() {
  datalist.clear();
}
