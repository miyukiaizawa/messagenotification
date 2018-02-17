#include "imageinfo.h"

ImageInfo::
ImageInfo(QObject* parent) :
  QObject(parent),
  path_(),
  image_() {}

ImageInfo::
ImageInfo(QString _path, QObject* parent) :
  QObject(parent),
  path_(_path),
  image_() {}

ImageInfo::
ImageInfo(const ImageInfo& info) {
  this->path_ = info.path_;
  this->setParent(info.parent());
  this->image_ = info.image_.copy();
}

ImageInfo::
~ImageInfo() {}

ImageInfo&
ImageInfo::
operator =(const ImageInfo& info) {
  this->setParent(info.parent());
  this->path_ = info.path_;
  this->image_ = info.image_.copy();
  return (*this);
}

bool
ImageInfo::
operator ==(const ImageInfo& info) {
  return (this->path_ == info.path_) && (this->image_ == info.image_);
}

bool
ImageInfo::
operator !=(const ImageInfo& info) {
  return (this->path_ != info.path_) || (this->image_ != info.image_);
}

QString
ImageInfo::
path() const {
  return path_;
}

void
ImageInfo::
setPath(const QString& value) {
  if (path_ != value) {
    path_ = value;
    pathChanged();
  }
}

QImage
ImageInfo::
image() const {
  return image_;
}

void
ImageInfo::
setImage(const QImage& value) {
  if (image_ != value) {
    image_ = value.copy();
    imageChanged();
  }
}

bool
ImageInfo::
hasImage() const {
  return (!image_.size().isEmpty());
}


