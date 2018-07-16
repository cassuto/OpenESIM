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

#ifndef ROTATIONTRANSFORM_H_
#define ROTATIONTRANSFORM_H_

#include <QVector>
#include <QPointF>

#include <device/graph.h>

namespace dsim
{

class RotationTransform
{
public:
  int rotationTransform( int angle, int x0, int y0 );
  int rotationTransform( ElemDirect direction, int x0, int y0 );

  void addPoint( const QPointF &point );
  void removeOne( const QPointF &point );
  void removeAll();
  inline int count() const { return m_originPoints.count(); }

  inline const QVector<QPointF> &transPoints() const { return m_transPoints; }

private:
  QVector<QPointF> m_originPoints;
  QVector<QPointF> m_transPoints;
};

}

#endif
