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

#include "elementgraphitem.h"
#include "elementabstractport.h"
#include <device/device.h> // for io_type_t

namespace dsim
{

class ElementText;
class ElementWire;
class ComponentGraphItem;

class ElementPin : public ElementGraphItem<QGraphicsItem>, public ElementAbstractPort
{
public:
  ElementPin( ElemDirect direct, const QPointF &pos, ElementText *symbol, ElementText *reference, int id, SchemaScene *scene, bool editable, QGraphicsItem *parent = 0 );
  ~ElementPin();

  const char *classname() const { return "pin"; }
  QRectF boundingRect() const { return m_bounding; }
  void setBoundingRect( const QRect &bounding );
  void setDirect( ElemDirect direct );
  void setLength( int length );
  void setSub( ElementText *symbol, ElementText *reference );
  void setLayout();
  QPointF portScenePos() const;
  int parentId() const { return id(); }
  ComponentGraphItem *component() const { return m_component; }
  inline void setComponent( ComponentGraphItem *component ) { m_component = component; }

  int serialize( LispDataset *dataset );
  int deserialize( LispDataset *dataset );
  int resolveSubElements();

  /*
   * Configuration interface
   */
  QString symbol() const;
  QString reference() const;
  inline int length() const { return m_length; }
  inline bool showSymbol() const { return m_showSymbol; }
  inline bool showReference() const { return m_showReference; }
  inline io_type_t ioType() const { return m_iotype; }
  void setSymbol( const QString &symbol );
  void setReference( const QString &reference );
  void setShowSymbol( bool show ) { m_showSymbol = show; updateVisible(); }
  void setShowReference( bool show ) { m_showReference = show; updateVisible(); }
  void setIOType( io_type_t type ) { m_iotype = type; }

protected: // for ElementWire only
  void connectWire( ElementWire *wire );
  void disconnectWire( ElementWire *wire, bool );

protected:
  void paint( QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget );

private:
  void updateVisible();
  void updateReferenceLabel();

private:
  int                       m_length;
  QRectF                    m_bounding;
  ElementText              *m_symbolLabel;
  ElementText              *m_referenceLabel;
  bool                      m_showSymbol;
  bool                      m_showReference;
  io_type_t                 m_iotype;
  ComponentGraphItem       *m_component;
};


}

#endif
