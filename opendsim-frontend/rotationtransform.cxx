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

#include <cmath>
#include <dsim/error.h>

#include "rotationtransform.h"

namespace dsim
{

int RotationTransform::rotationTransform( int angle, int x0, int y0 )
{
  double xFactor = cos( angle ), yFactor = sin( angle );
  double x = 0, y = 0;
  double x1, y1;
  int i = 0;

  QVector<QPointF>::iterator it;

  for( it=m_originPoints.begin(); it!=m_originPoints.end(); ++it )
    {
      x = (*it).x(); y = (*it).y();

      x1 = (x - x0)*xFactor - (y - y0)*yFactor + x0;
      y1 = (x - x0)*yFactor + (y - y0)*xFactor + y0;

      m_transPoints[i++] = QPointF( x1, y1 );
    }

  return 0;
}

int RotationTransform::rotationTransform( ElemDirect direction, int x0, int y0 )
{
  double xFactor, yFactor;

  switch( direction )
  {
    case ELEM_RIGHT:    return rotationTransform( 180, x0, y0 );
    case ELEM_TOP:      return rotationTransform( 90, x0, y0 );
    case ELEM_LEFT:     return rotationTransform( 0, x0, y0 );
    case ELEM_BOTTOM:   return rotationTransform( -90, x0, y0 );
  }

  return -DS_FAULT;
}

void RotationTransform::addPoint( const QPointF &point )
{ m_originPoints.append( point ); m_transPoints.append( point ); }

void RotationTransform::removeOne( const QPointF &point )
{ m_originPoints.removeOne( point ); m_transPoints.removeOne( point ); }

void RotationTransform::removeAll()
{ m_originPoints.clear(); m_transPoints.clear(); }

}
