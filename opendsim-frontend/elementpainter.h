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

#ifndef ELEMENTPAINTER_H_
#define ELEMENTPAINTER_H_

#include <string>
#include "elementgraphitem.h"
#include "staffgraphitem.h"

namespace dsim
{

class ComponentGraphImpl;
class SchematicImpl;
class StaffPad;

class ElementPainter : public ElementGraphItem<QGraphicsRectItem>, public StaffEvents
{
public:
  ElementPainter( const QRectF &rect, int id, SchemaScene *scene, bool edit, QGraphicsRectItem* parent = 0 );
  ~ElementPainter();

  const char *classname() const { return "painter"; }

  void setStyle( const char *style );
  void setRectParent( const QRectF &rect );
  QRectF rectParent() const;
  void normalizeRect();

  int serialize( LispDataset *dataset );
  int deserialize( LispDataset *dataset );
  void setSelected( bool selected );

  inline const std::string &tokenId() const { return m_tokenId; }
  inline void setTokenId( const std::string &tokenId ) { m_tokenId = tokenId; }
  inline ComponentGraphImpl *deviceGraph() const { return m_deviceGraph; }
  inline void setPixBuffer( QImage *pixbuffer ) { m_pixBuffer = pixbuffer; }

protected:
  void staffMoveEvent( int index, bool fineturning, QGraphicsSceneMouseEvent *event );
  void staffMouseReleaseEvent( int index, QGraphicsSceneMouseEvent *event );
  void paint( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0 );

private:
  void updatePads();

  std::string m_tokenId;
  QImage *m_pixBuffer;
  ComponentGraphImpl *m_deviceGraph;
  StaffPad *m_pads[4];
};

}

#endif
