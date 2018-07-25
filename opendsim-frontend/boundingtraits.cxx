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

#include <dsim/misc.h>
#include "elementgraphitem.h"

#include "boundingtraits.h"

namespace dsim
{

/**
 * Returns the bounding rectangle of this rectangle and the given
 * rectangle. (This code comes from QtCore module)
 */
static QRectF combineRect( const QRectF &s, const QRectF &r )
{
  if (s.isNull()) return r;
  if (r.isNull()) return s;

  qreal xp = s.x(), yp = s.y();
  qreal w = s.width(), h = s.height();

  qreal left = xp;
  qreal right = xp;
  if (w < 0)
    left += w;
  else
    right += w;

  if (r.width() < 0)
    {
      left = MIN(left, r.x() + r.width());
      right = MAX(right, r.x());
    }
  else
    {
      left = MIN(left, r.x());
      right = MAX(right, r.x() + r.width());
    }

  qreal top = yp;
  qreal bottom = yp;
  if (h < 0)
    top += h;
  else
    bottom += h;

  if (r.height() < 0)
    {
      top = MIN(top, r.y() + r.height());
      bottom = MAX(bottom, r.y());
    }
  else
    {
      top = MIN(top, r.y());
      bottom = MAX(bottom, r.y() + r.height());
    }

  return QRectF( left, top, right - left, bottom - top );
}

QRectF BoundingTraits::traits( const QList<ElementBase *>::const_iterator &begin, const QList<ElementBase *>::const_iterator &end )
{
  QPointF pos;
  QRectF bounding;

  for( QList<ElementBase *>::const_iterator it=begin; it!=end; ++it )
    {
      ElementBase *element = (*it);
      pos = element->graphicsItem()->pos();
      QTransform transform = element->graphicsItem()->transform();
      QRectF mapped = transform.mapRect( element->boundingRect() );
      mapped.adjust( pos.x(), pos.y(), pos.x(), pos.y() );

      bounding = combineRect( bounding, mapped );
    }

  return bounding;
}

QPointF BoundingTraits::traitsTopLeft( const QList<ElementBase *> &list )
{
  QPointF pos;
  double x = 0, y = 0;

  foreach( ElementBase *element, list )
    {
      pos = element->graphicsItem()->pos();

      x = MIN( x, pos.x() );
      y = MIN( y, pos.y() );
    }

  return QPointF( x, y );
}

}
