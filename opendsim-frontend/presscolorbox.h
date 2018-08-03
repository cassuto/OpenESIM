/*
 *  OpenDSIM (A/D mixed circuit simulator)
 *  Copyleft (C) 2016, The first Middle School in Yongsheng Lijiang China
 *
 *  This project is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public License(GPL)
 *  as published by the Free Software Foundation; either version 2.1
 *  of the License, or (at your option) any later version.
 *
 *  This project is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 */

#ifndef PRESSCOLORBOX_H_
#define PRESSCOLORBOX_H_

#include <QComboBox>

namespace dsim
{

class PressColorBox : public QComboBox
{
  Q_OBJECT

public:
  PressColorBox( QWidget *parent = 0l );

  void setColor( const QColor &color );
  void setEnabled( bool enabled );

  inline const QColor& color() const { return m_color; }

protected:
  void mousePressEvent( QMouseEvent *event );

signals:
  void clicked();
  void changed( const QColor &color );

private:
  void updateDisp();

private:
  QColor m_color;
};

}

#endif // PRESSCOLORBOX_H_
