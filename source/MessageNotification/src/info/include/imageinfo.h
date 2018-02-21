#pragma once
#ifndef __IMAGE_INFO_H__
#define __IMAGE_INFO_H__

#include <QObject>
#include <QString>
#include <QImage>

class ImageInfo : public QObject {
  Q_OBJECT
public:
  Q_PROPERTY(QString path READ path WRITE setPath NOTIFY pathChanged);
  Q_PROPERTY(QImage image READ image WRITE setImage NOTIFY imageChanged);
  Q_PROPERTY(bool hasImage READ hasImage NOTIFY imageChanged);

  ImageInfo(QObject* parent = 0);
  ImageInfo(QString _path, QObject* parent = 0);
  ImageInfo(const ImageInfo& info);
  ~ImageInfo();

  ImageInfo& operator =(const ImageInfo& info);
  bool operator ==(const ImageInfo& info);
  bool operator !=(const ImageInfo& info);

  QString path() const;
  void setPath(const QString& value);

  QImage image() const;
  void setImage(const QImage& value);

  bool hasImage() const;
signals:
  void pathChanged();
  void imageChanged();

private:
  QString path_;
  QImage image_;
};
Q_DECLARE_METATYPE(ImageInfo)


#endif //__IMAGE_INFO_H__
