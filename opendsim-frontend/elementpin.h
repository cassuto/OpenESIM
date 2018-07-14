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

#ifndef ELEMENTPIN_H_
#define ELEMENTPIN_H_

#include <QtWidgets>

#include "elementgraphitem.h"

namespace dsim
{

class ElementText;

class ElementPin : public ElementGraphItem<QGraphicsItem>
{
public:
  ElementPin( ElemDirect direct, const QPointF &pos, ElementText *symbol, ElementText *reference, int id, SchemaGraph *scene, bool editable, QGraphicsItem *parent = 0 );
  ~ElementPin();

  const char *classname() { return "pin"; }
  QRectF boundingRect() const { return m_bounding; }
  void setBoundingRect( const QRect &bounding );

  void setDirect( ElemDirect direct );
  void setLength( int length );
  void setSub( ElementText *symbol, ElementText *reference );
  void setLayout();

  int serialize( LispDataset *dataset );
  int deserialize( LispDataset *dataset );
  int resolveSubElements();

protected:
  void paint( QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget );

private:
  int                       m_length;
  QRectF                    m_bounding;
  ElementText              *m_symbolLabel;
  ElementText              *m_referenceLabel;
};


}

#endif
