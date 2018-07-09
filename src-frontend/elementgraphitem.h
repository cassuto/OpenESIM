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

#ifndef SCHEMAELEMENT_H_
#define SCHEMAELEMENT_H_

#include <QtWidgets>
#include <string>
#include <device/graph.h>

#include "domitem.h"
#include "domdataset.h"
#include "schemagraph.h"

namespace dsim
{

class ElementGraphItem : public QGraphicsItem, public DomItem
{
public:
  ElementGraphItem( int id, bool editable, SchemaGraph *schgraph, QGraphicsItem *parent = 0 );

  virtual const char *classname() { return "none"; }

  virtual QRectF boundingRect() const { return m_area; }
  virtual void setBoundingRect( const QRect &area );
  virtual void paintBound( QPainter *painter );
  virtual void move( QPointF delta );
  virtual void setStyle( const char *style );
  virtual void setDirect( ElemDirect direct );
  virtual void setSelected( bool selected );
  virtual void setLayout();

  virtual int serialize( DomDataset *dataset );
  virtual int deserialize( DomDataset *dataset );
  virtual int addElement( ElementGraphItem *item );
  virtual int resolveSubElements();

  bool ref();
  bool isRef();
  void setVisible( bool visible );
  const QList<ElementGraphItem*> &elements() const;

  inline int id() const { return m_id; }
  inline void resetId( int id ) { m_id = id; }
  inline const char *style() const { return m_style.c_str(); }
  inline bool editable() const { return m_editable; }
  inline bool moving() const { return m_moving; }
  inline SchemaView *view() const { return m_schgraph->view(); }

  static void applyStyle( QPainter *painter, const char *style, bool selected );
  static void applyTextStyle( QGraphicsSimpleTextItem *painter, const char *style, bool selected );

protected:
  virtual void mousePressEvent( QGraphicsSceneMouseEvent *event );
  virtual void mouseMoveEvent( QGraphicsSceneMouseEvent *event );
  virtual void mouseReleaseEvent( QGraphicsSceneMouseEvent *event );

protected:
  int                       m_id;
  SchemaGraph              *m_schgraph;
  QRectF                    m_area;
  bool                      m_editable;
  bool                      m_moving;
  ElemDirect                m_direct;
  std::string               m_style;
  QList<ElementGraphItem*>  m_elements;
  QList<int>                m_elementIds;
  bool                      m_ref;

  friend class ElementSelpad;
};

}

#endif
