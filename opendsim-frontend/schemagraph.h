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

#ifndef SCHEMAGRAPH_H_
#define SCHEMAGRAPH_H_

#include <QtGui>
#include <QGraphicsScene>

namespace dsim
{

class SchemaView;

class SchemaGraph : public QGraphicsScene
{
  Q_OBJECT

public:
  SchemaGraph( qreal x, qreal y, qreal width, qreal height, SchemaView* parent );
  ~SchemaGraph();

  void remove();
  bool paintGrid();
  void setPaintGrid( bool paint );

  inline SchemaView *view() const { return m_schemaView; }

private:
  void drawBackground( QPainter*  painter, const QRectF & rect );

private:
  QRect                 m_scenerect;
  SchemaView*           m_schemaView;
  bool                  m_paintGrid;
};

inline int roundDown( int x, int roundness )
{
  if( x < 0 ) return (x-roundness+1) / roundness;
  else        return (x / roundness);
}

inline int roundDown( float x, int roundness ) { return roundDown( int(x), roundness ); }

inline QPoint roundDown( const QPoint & p, int roundness )
{
  return QPoint( roundDown( p.x(), roundness ), roundDown( p.y(), roundness ) );
}

#define gridWh (8)
#define gridHt (8)

inline int snapToGrid( int x  ) { return roundDown( x, 8 )*8 + 4; }

inline QPointF togrid( QPointF point )
{
  int valor;
  valor = snapToGrid( (int)point.x() );
  point.rx() = (float)valor;
  valor = snapToGrid( (int)point.y() );
  point.ry() = (float)valor;
  return point;
}

inline QPoint togrid( QPoint point )
{
  int valor;
  valor = snapToGrid( (int)point.x() );
  point.rx() = valor;
  valor = snapToGrid( (int)point.y() );
  point.ry() = valor;
  return point;
}

} // namespace dsim

#endif
