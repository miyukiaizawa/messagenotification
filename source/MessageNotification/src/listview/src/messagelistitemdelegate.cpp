#include "messagelistitemdelegate.h"
#include "messagelistitemmodel.h"
#include <QPainter>
#include <QApplication>
#include <QFontMetrics>
#include <QStyleOption>


QRect MessageListItemDelegate::iconSize = QRect(0, 0, 32, 32);
int MessageListItemDelegate::padding = 16;

void
MessageListItemDelegate::
paint(QPainter *painter,
      const QStyleOptionViewItem &option,
      const QModelIndex &index) const {



  if (!index.isValid())
    return;

  painter->save();

  auto backgroundColor = qvariant_cast<QBrush>(index.data(ItemRoles::ItemBackGroundRole));
  QRectF backgroundRect(option.rect.x() + 2,
                        option.rect.y() + 2,
                        option.rect.width() - 2 * 2,
                        option.rect.height() - 2);

  painter->fillRect(backgroundRect, backgroundColor);

  if (option.state & QStyle::State_Selected) {
    painter->fillRect(backgroundRect, option.palette.highlight());
  }

  auto uncheckIcon = qvariant_cast<QImage>(index.data(ItemRoles::IconRole));
  auto uncheckIconSize = qvariant_cast<QSize>(index.data(ItemRoles::IconSizeRole));
  auto messageColor = qvariant_cast<QBrush>(index.data(MessageColorRole));
  auto baseFont = qvariant_cast<QFont>(index.data(Qt::FontRole));

  QSize totalArea(0, 0);
  QRect upperRect(0, 0, 0, 0);

  //write category
  {
    QFont font = baseFont;
    font.setBold(true);
    drawText(painter,
             ::category(index), font,
             QRect(option.rect.x(), option.rect.y(), option.rect.width(), 0),
             QRect(uncheckIconSize.width() + padding * 2, padding, 0, 0),
             messageColor.color(),
             Qt::AlignLeft | Qt::AlignTop | Qt::TextWordWrap,
             upperRect);

    totalArea.setWidth(qMax(totalArea.width(), upperRect.width()));
    totalArea.setHeight(totalArea.height() + upperRect.height());

  }

  //write messages
  {
    auto message = ::message(index);
    auto lines = message.split((';'));

    QFont font = baseFont;
    font.setBold(true);
    font.setPointSize(font.pointSize()*0.8);

    for (auto& line : lines) {
      if (line.isEmpty()) {
        continue;
      }
      drawText(painter,
               line, font,
               QRect(option.rect.x(), upperRect.y() + upperRect.height(), option.rect.width(), 0),
               QRect(uncheckIconSize.width() + padding * 2, padding, 0, 0),
               option.palette.text().color(),
               Qt::AlignLeft | Qt::AlignTop | Qt::TextWordWrap,
               upperRect);

      totalArea.setWidth(qMax(totalArea.width(), upperRect.width()));
      totalArea.setHeight(totalArea.height() + upperRect.height());

    }

  }

  //write date
  {
    QFont font = baseFont;
    font.setBold(true);
    font.setPointSize(font.pointSize()*0.5);
    drawText(painter,
             ::date(index).toString(), font,
             QRect(option.rect.x(), option.rect.y(), option.rect.width(), 0),
             QRect(padding, upperRect.height() * 0.5 + padding, 0, 0),
             option.palette.text().color(),
             Qt::AlignRight | Qt::AlignTop | Qt::TextWordWrap,
             upperRect);

    totalArea.setWidth(qMax(totalArea.width(), upperRect.width()));
    totalArea.setHeight(totalArea.height() + upperRect.height());

  }

  //write inform icon
  if (!uncheckIcon.isNull()) {
    QSize s(padding, (totalArea.height() - uncheckIcon.height() + padding) / 2 + padding);
    QPoint p(s.width() + option.rect.x(), s.height() + option.rect.y());
    painter->drawImage(p, uncheckIcon);
  }

}

QSize
MessageListItemDelegate::
sizeHint(const QStyleOptionViewItem &option,
         const QModelIndex &index) const {

  if (!index.isValid())
    return QSize();

  auto baseFont = qvariant_cast<QFont>(index.data(Qt::FontRole));

  QSize size(0, 0);
  //category size
  {
    QFont font = baseFont;
    font.setBold(true);
    QFontMetrics fm(font);
    QRect rect = makeTextRect(::category(index),
                              fm,
                              option.rect,
                              QRect(0, 0, -iconSize.width(), 0),
                              Qt::AlignLeft | Qt::AlignTop | Qt::TextWordWrap);

    size.setWidth(qMax(size.width(), rect.width()));
    size.setHeight(size.height() + rect.height() + padding);
  }

  //write date
  {
    QFont font = baseFont;
    font.setBold(false);

    QFontMetrics fm(font);
    QRect rect = makeTextRect(::date(index).toString(),
                              fm,
                              QRect(0, 0, option.rect.width(), 0),
                              QRect(0, 0, -iconSize.width(), 0),
                              Qt::AlignLeft | Qt::AlignTop | Qt::TextWordWrap);

    size.setWidth(qMax(size.width(), rect.width()));
    size.setHeight(size.height() + rect.height());
  }

  //write messages
  {
    auto message = ::message(index);
    auto lines = message.split((';'));

    QFont font = baseFont;
    font.setBold(false);
    for (auto& line : lines) {
      if (line.isEmpty()) {
        continue;
      }
      QFont font = baseFont;
      font.setBold(false);

      QFontMetrics fm(font);
      QRect rect = makeTextRect(line,
                                fm,
                                QRect(0, 0, option.rect.width(), 0),
                                QRect(0, 0, -iconSize.width(), 0),
                                Qt::AlignLeft | Qt::AlignTop | Qt::TextWordWrap);

      size.setWidth(qMax(size.width(), rect.width()));
      size.setHeight(size.height() + rect.height() + padding);
    }
  }

  size.setWidth(qMax(size.width(), qMax(option.rect.width(), iconSize.width())));
  size.setHeight(qMax(size.height(), iconSize.height()));

  return size;
}
