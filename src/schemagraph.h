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

#ifndef CIRCUIT_H
#define CIRCUIT_H

#include <QtGui>
#include <QGraphicsScene>

namespace dsim
{

class SchemaGraph : public QGraphicsScene
{
  Q_OBJECT

public:
  SchemaGraph( qreal x, qreal y, qreal width, qreal height, QGraphicsView*  parent );
  ~SchemaGraph();

static SchemaGraph*  instance() { return m_pschemagraph; }

  void remove();
  bool paintGrid();
  void setPaintGrid( bool paint );

private:
  void drawBackground( QPainter*  painter, const QRectF & rect );

private:
  static SchemaGraph*   m_pschemagraph;
  QRect                 m_scenerect;
  QGraphicsView*        m_graphicView;

  bool                  m_paintGrid;

};

} // namespace dsim

#endif
