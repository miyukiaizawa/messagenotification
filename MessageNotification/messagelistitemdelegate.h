#pragma once
#ifndef __MESSAGE_LIST_ITEM_DELEGATE_H__
#define __MESSAGE_LIST_ITEM_DELEGATE_H__

#include <QStyledItemDelegate>

class MessageListItemDelegate : public QStyledItemDelegate {
  Q_OBJECT
public:

  void paint(QPainter *painter, 
             const QStyleOptionViewItem &option, 
             const QModelIndex &index) const override;

  QSize sizeHint(const QStyleOptionViewItem &option, 
                 const QModelIndex &index) const override;

signals:
  void imageButtonClicked();

private:

private:
  static QRect iconSize;
  static int padding;
};



#endif //__MESSAGE_LIST_ITEM_DELEGATE_H__
