#include "messagelistitemmodel.h"
#include <QFont>
#include <QBrush>
#include <QDebug>
#include <QApplication>
#include <QImage>
#include <QAction>
#include <QIcon>

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
    QFont boldFont = QApplication::font();
    boldFont.setBold(true);
    boldFont.setPointSize(18);
    return boldFont;
  } return QVariant();

  case Qt::BackgroundRole: {
  } return QVariant();

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

  case ItemRoles::IconRole: {
    if (!entry->checked()) {
      QImage icon(":/MessageNotification/icon/warning-sign-red.png");
      QSize size = qvariant_cast<QSize>(data(index, ItemRoles::IconSizeRole));
      icon = icon.scaled(size, Qt::AspectRatioMode::KeepAspectRatio);
      return QVariant::fromValue(icon);
    }
  } return QVariant();

  case ItemRoles::IconSizeRole: {
  } return QVariant::fromValue(QSize(64, 64));

  case ItemRoles::MessageColorRole: {
    if (entry->category() == "Alert") {
      return QBrush(Qt::red);
    }
    if (entry->category() == "Info") {
      return QBrush(Qt::green);
    }
  } return QBrush(Qt::white);

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

  case ItemRoles::ItemBackGroundRole: {
  } return QBrush(QColor::fromRgba(qRgba(66, 66, 66, 66)));

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

QModelIndex 
MessageListItemModel::
findChildIndex(MessageInfo* obj) {
  int row = -1, col = 0;
  MessageInfo* data = nullptr;

  if (obj == nullptr) {
    return createIndex(row, col, data);
  }

  for (int index = 0; index < datalist.size(); index++) {
    data = datalist[index];
    if (data->id() == obj->id()) {
      row = index;
      data = datalist[index];
      break;
    }
  }

  return createIndex(row, col, data);
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
eraseDuplicatedItem(const QList<MessageInfo*>& items) {

  for (int Oldindex = 0; Oldindex < datalist.size(); Oldindex++) {

    auto oldItem = datalist[Oldindex];
    bool found_same = false;
    for (int newIndex = 0; newIndex < items.size(); newIndex++) {
      auto newItem = items[newIndex];
      if (oldItem->id() == newItem->id()) {
        found_same = true;
        break;
      }
    }

    if (found_same) {
      continue;
    }
    datalist.erase(datalist.begin() + Oldindex);
  }

}

void
MessageListItemModel::
updateNewItem(const QList<MessageInfo*>& items) {

  //update data
  for (int newIndex = items.size() - 1; newIndex >= 0; newIndex--) {
    auto newItem = items[newIndex];

    bool insertable = true;
    for (int oldIndex = datalist.size() - 1; oldIndex >= 0; oldIndex--) {
      auto oldItem = datalist[oldIndex];
      if (oldItem->id() == newItem->id()) {
        oldItem->setCategory(newItem->category());
        oldItem->setDate(newItem->date());
        oldItem->setMessage(newItem->message());
        oldItem->setImageInfo(newItem->imageInfo());
        oldItem->setChecked(newItem->checked());
        insertable = false;
        break;
      }
    }

    if (insertable) {
      datalist.push_back(newItem);
      //add anyway without to take care about item order 
    }
  }
}

void
MessageListItemModel::
assainItems(const QList<MessageInfo*>& items, bool greater_sort) {

  eraseDuplicatedItem(items);

  updateNewItem(items);

  //need to sort item order at here
  if (greater_sort) {
    qSort(datalist.begin(), datalist.end(), qGreater<MessageInfo>());
  }
  else {
    qSort(datalist.begin(), datalist.end(), qLess<MessageInfo>());
  }

  requestDataChanged();
}

