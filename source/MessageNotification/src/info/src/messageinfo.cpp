#include "messageinfo.h"

MessageInfo::
MessageInfo(QObject *parent) :
  QObject(parent),
  id_(-1),
  category_(),
  date_(),
  message_(),
  imageInfo_(),
  checked_(false) {}

MessageInfo::
MessageInfo(MessageInfo *info) :
  QObject(info->parent()),
  id_(info->id_),
  category_(info->category_),
  date_(info->date_),
  message_(info->message_),
  imageInfo_(info->imageInfo_),
  checked_(info->checked_) {}

MessageInfo::
MessageInfo(const int &_id,
            const QString &_category,
            const QDateTime &_date,
            const QString &_message,
            const ImageInfo& _imageInfo,
            const bool& _checked,
            QObject *parent) :
  QObject(parent),
  id_(_id),
  category_(_category),
  date_(_date),
  message_(_message),
  imageInfo_(_imageInfo),
  checked_(_checked) {}

MessageInfo::
MessageInfo(const MessageInfo& info) :
  QObject(info.parent()),
  id_(info.id_),
  category_(info.category_),
  date_(info.date_),
  message_(info.message_),
  imageInfo_(info.imageInfo_),
  checked_(info.checked_) {}

MessageInfo&
MessageInfo::
operator = (const MessageInfo& other) {
  setParent(other.parent());
  id_ = other.id();
  category_ = other.category();
  date_ = other.date();
  message_ = other.message();
  imageInfo_ = other.imageInfo();
  return (*this);
}


QString
MessageInfo::
category() const {
  return category_;
}
void
MessageInfo::
setCategory(const QString& value) {
  if (category_ != value) {
    category_ = value;
    emit categoryChanged();
  }
}

QDateTime
MessageInfo::
date() const {
  return date_;
}

void
MessageInfo::
setDate(const QDateTime& value) {
  if (date_ != value) {
    date_ = value;
    emit dateChanged();
  }
}

QString
MessageInfo::
message() const {
  return message_;
}

void
MessageInfo::
setMessage(const QString& value) {
  if (message_ != value) {
    message_ = value;
    emit messageChanged();
  }
}

ImageInfo
MessageInfo::
imageInfo() const {
  return imageInfo_;
}

void
MessageInfo::
setImageInfo(const ImageInfo& value) {
  if (imageInfo_ != value) {
    imageInfo_ = value;
    emit imageInfoChanged();
  }
}

bool
MessageInfo::
checked() const {
  return checked_;
}

void
MessageInfo::
setChecked(const bool& value) {
  if (checked_ != value) {
    checked_ = value;
    emit checkedChanged();
  }
}

int
MessageInfo::
id() const {
  return id_;
}

void
MessageInfo::
setId(const int& value) {
  if (id_ != value) {
    id_ = value;
    emit idChanged();
  }
}

