#pragma once
#ifndef __MESSAGE_LIST_ITEM_DELEGATE_H__
#define __MESSAGE_LIST_ITEM_DELEGATE_H__

#include <QStyledItemDelegate>
#include <QPainter>

class MessageListItemDelegate : public QStyledItemDelegate {
  Q_OBJECT
public:

  void paint(QPainter *painter,
             const QStyleOptionViewItem &option,
             const QModelIndex &index) const override;

  QSize sizeHint(const QStyleOptionViewItem &option,
                 const QModelIndex &index) const override;


private:

  void drawText(QPainter *painter,
                const QString& text,
                const QFont font,
                const QRect& area,
                const QRect& offset,
                const QColor& color,
                int alignFlag,
                QRect& drawnRect) const {

    QFontMetrics fm(font);

    QRect rect = makeTextRect(text, fm, area, offset);

    if ((alignFlag & Qt::AlignHorizontal_Mask) == Qt::AlignRight) {
      auto r = rect.width();
      rect.setX(-area.x() + area.width() - offset.x()- rect.width() );
      rect.setWidth(r);
    }

    painter->save();

    painter->setPen(color);
    painter->setFont(font);
    painter->drawText(rect, alignFlag, text);

    painter->restore();

    drawnRect = rect;

  }

  QRect makeTextRect(QString text,
                     QFontMetrics fontMetrics,
                     QRect area,
                     QRect offset,
                     int alignFlag = Qt::AlignLeft | Qt::AlignTop | Qt::TextWordWrap,
                     QRect upperRect = QRect(0, 0, 0, 0)) const {

    QRect rect(offset.left() + area.left(), offset.top() + area.top(),
               offset.width() + area.width(), offset.height() + area.height());
    return fontMetrics.boundingRect(rect, alignFlag, text);
  }

signals:
  void imageButtonClicked();

private:

private:
  static QRect iconSize;
  static int padding;
};



#endif //__MESSAGE_LIST_ITEM_DELEGATE_H__
