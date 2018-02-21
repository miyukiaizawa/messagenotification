#pragma once
#ifndef __FRAME_H__
#define __FRAME_H__
#include <QFrame>
#include "titlebar.h"

class Frame : public QFrame {
public:

  Frame() {
    m_mouse_down = false;
    setFrameShape(Panel);

    setWindowFlags(Qt::FramelessWindowHint);
    setMouseTracking(true);

    m_titleBar = new TitleBar(this);
    m_content = new QWidget(this);

    QVBoxLayout *vbox = new QVBoxLayout(this);
    vbox->addWidget(m_titleBar);
    vbox->setMargin(0);
    vbox->setSpacing(0);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(m_content);
    layout->setMargin(5);
    layout->setSpacing(0);
    vbox->addLayout(layout);

  }

  QWidget *contentWidget() const { return m_content; }
  TitleBar *titleBar() const { return m_titleBar; }

  void mousePressEvent(QMouseEvent *e) {
    m_old_pos = e->pos();
    m_mouse_down = e->button() == Qt::LeftButton;
  }

  void mouseMoveEvent(QMouseEvent *e) {
    int x = e->x();
    int y = e->y();

    if (m_mouse_down) {
      int dx = x - m_old_pos.x();
      int dy = y - m_old_pos.y();
      QRect g = geometry();

      if (left)
        g.setLeft(g.left() + dx);

      if (right)
        g.setRight(g.right() + dx);

      if (bottom)
        g.setBottom(g.bottom() + dy);

      setGeometry(g);

      m_old_pos = QPoint(!left ? e->x() : m_old_pos.x(), e->y());
    }
    else {

      QRect r = rect();
      left = qAbs(x - r.left()) <= 5;
      right = qAbs(x - r.right()) <= 5;
      bottom = qAbs(y - r.bottom()) <= 5;

      bool hor = left | right;

      if (hor && bottom) {
        if (left)
          setCursor(Qt::SizeBDiagCursor);
        else
          setCursor(Qt::SizeFDiagCursor);
      }
      else if (hor) {
        setCursor(Qt::SizeHorCursor);
      }
      else if (bottom) {
        setCursor(Qt::SizeVerCursor);
      }
      else {
        setCursor(Qt::ArrowCursor);
      }
    }
  }

  void mouseReleaseEvent(QMouseEvent *e) {
    m_mouse_down = false;
  }

private:
  TitleBar *m_titleBar;
  QWidget *m_content;
  QPoint m_old_pos;
  bool m_mouse_down;
  bool left, right, bottom;

};

#endif //__FRAME_H__

