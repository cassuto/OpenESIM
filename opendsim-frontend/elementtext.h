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

#ifndef ELEMENTTEXT_H_
#define ELEMENTTEXT_H_

#include <QtWidgets>

#include "elementgraphitem.h"

namespace dsim
{

class ElementText : public ElementGraphItem<QGraphicsSimpleTextItem>
{
public:
  ElementText( ElemDirect direct, const QPointF &pos, int id, SchemaGraph *scene, bool edit, QGraphicsSimpleTextItem* parent = 0 );
  ~ElementText();

  const char *classname() { return "text"; }

  void setStyle( const char *style );
  void setDirect( ElemDirect direct );
  void setTextEditable( bool editable );

  int serialize( LispDataset *dataset );
  int deserialize( LispDataset *dataset );

protected:
  void mouseDoubleClickEvent( QGraphicsSceneMouseEvent *event );
  void paint( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0 );

private:
  bool m_textEditable;
};

}

#endif
