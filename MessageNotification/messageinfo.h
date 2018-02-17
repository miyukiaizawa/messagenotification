#pragma once
#ifndef __MESSAGEINFO_H__
#define __MESSAGEINFO_H__

#include "imageinfo.h"
#include <QDateTime>

class MessageInfo : public QObject {
  Q_OBJECT
public:

  Q_PROPERTY(QString category READ category WRITE setCategory NOTIFY categoryChanged);
  Q_PROPERTY(QDateTime date READ date WRITE setDate NOTIFY dateChanged);
  Q_PROPERTY(QString message READ message WRITE setMessage NOTIFY messageChanged);
  Q_PROPERTY(ImageInfo imageInfo READ imageInfo WRITE setImageInfo NOTIFY imageInfoChanged);
  Q_PROPERTY(bool checked READ checked WRITE setChecked NOTIFY checkedChanged);
  Q_PROPERTY(int id READ id WRITE setId NOTIFY idChanged);

public:
  MessageInfo(QObject *parent = 0);
  MessageInfo(const int& _id,
              const QString &_category,
              const QDateTime &_date,
              const QString &_message,
              const ImageInfo& _imageInfo,
              const bool& _checked,
              QObject *parent = 0);
  MessageInfo(const MessageInfo& info);

  QString category() const;
  void setCategory(const QString& value);

  QDateTime date() const;
  void setDate(const QDateTime& value);

  QString message() const;
  void setMessage(const QString& value);

  ImageInfo imageInfo() const;
  void setImageInfo(const ImageInfo& value);

  bool checked() const;
  void setChecked(const bool& value);

  int id() const;
  void setId(const int& value);

signals:
  void categoryChanged();
  void dateChanged();
  void messageChanged();
  void imageInfoChanged();
  void checkedChanged();
  void idChanged();

private:
  QString category_;
  QDateTime date_;
  QString message_;
  ImageInfo imageInfo_;
  bool checked_;
  int id_;

};
Q_DECLARE_METATYPE(MessageInfo)

#endif // __MESSAGEINFO_H__
