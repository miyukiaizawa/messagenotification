#include "messagelistitemdelegate.h"
#include "messagelistitemmodel.h"
#include <QPainter>
#include <QApplication>
#include <QFontMetrics>
#include <QStyleOption>

/*
基本的に、デリゲートでは描画周りを管理するだけなので、
ウィジェットを動作させようとすると、また別になる。
Modelにウィジェットの動作内容を保存させて、描画時に更新させるようなこともできるけれど、
ここで更新できるのは、あくまで描画情報だけなので、ListViewに関連するどこかに
ウィジェットをそのまま追加しないといけない。
*/


QRect MessageListItemDelegate::iconSize = QRect(0, 0, 32, 32);
int MessageListItemDelegate::padding = 5;

void
MessageListItemDelegate::
paint(QPainter *painter,
      const QStyleOptionViewItem &option,
      const QModelIndex &index) const {


  auto category = ::category(index);
  auto date = ::date(index);
  auto message = ::message(index);

  if (!index.isValid())
    return;

  painter->save();

  auto itemColor = qvariant_cast<QBrush>(index.data(Qt::BackgroundRole));
  painter->fillRect(option.rect, itemColor);
  if (option.state & QStyle::State_Selected) {
    painter->fillRect(option.rect, option.palette.highlight());
  }
  

  QString headerText = category;
  QString subheaderText = message;

  QFont headerFont = QApplication::font();
  headerFont.setBold(true);
  headerFont.setPointSize(14);
  QFont subheaderFont = QApplication::font();
  subheaderFont.setPointSize(12);

  QFontMetrics headerFm(headerFont);
  QFontMetrics subheaderFm(subheaderFont);

  /*
  * The x,y coords are not (0,0) but values given by 'option'. So, calculate the
  * rects again given the x,y,w.
  * Note that the given height is 0. That is because boundingRect() will return
  * the suitable height if the given geometry does not fit. And this is exactly
  * what we want.
  */
  QRect headerRect =
    headerFm.boundingRect(option.rect.left() + iconSize.width(), option.rect.top() + padding,
                          option.rect.width() - iconSize.width(), 0,
                          Qt::AlignLeft | Qt::AlignTop | Qt::TextWordWrap,
                          headerText);
  QRect subheaderRect =
    subheaderFm.boundingRect(headerRect.left(), headerRect.bottom() + padding,
                             option.rect.width() - iconSize.width(), 0,
                             Qt::AlignLeft | Qt::AlignTop | Qt::TextWordWrap,
                             subheaderText);
  auto messageColor = qvariant_cast<QBrush>(index.data(MessageColorRole));
  painter->setPen(messageColor.color());
  painter->setFont(headerFont);
  painter->drawText(headerRect, Qt::AlignLeft | Qt::AlignTop | Qt::TextWordWrap, headerText);

  painter->setPen(Qt::black);
  painter->setFont(subheaderFont);
  painter->drawText(subheaderRect, Qt::AlignLeft | Qt::AlignTop | Qt::TextWordWrap, subheaderText);


}

QSize
MessageListItemDelegate::
sizeHint(const QStyleOptionViewItem &option,
         const QModelIndex &index) const {

  if (!index.isValid())
    return QSize();

  QString headerText = "header text";
  QString subheaderText = "subheader text";

  QFont headerFont = QApplication::font();
  headerFont.setBold(true);
  headerFont.setPointSize(14);

  QFont subheaderFont = QApplication::font();
  subheaderFont.setPointSize(12);

  QFontMetrics headerFm(headerFont);
  QFontMetrics subheaderFm(subheaderFont);

  /* No need for x,y here. we only need to calculate the height given the width.
  * Note that the given height is 0. That is because boundingRect() will return
  * the suitable height if the given geometry does not fit. And this is exactly
  * what we want.
  */
  QRect headerRect = headerFm.boundingRect(0, 0,
                                           option.rect.width() - iconSize.width(), 0,
                                           Qt::AlignLeft | Qt::AlignTop | Qt::TextWordWrap,
                                           headerText);
  QRect subheaderRect = subheaderFm.boundingRect(0, 0,
                                                 option.rect.width() - iconSize.width(), 0,
                                                 Qt::AlignLeft | Qt::AlignTop | Qt::TextWordWrap,
                                                 subheaderText);

  QSize size(option.rect.width(), headerRect.height() + subheaderRect.height() + 3 * padding);

  /* Keep the minimum height needed in mind. */
  if (size.height() < iconSize.height())
    size.setHeight(iconSize.height());

  return size;
}
