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

#define TRACE_UNIT "componentgraphitem"

#include <device/device.h>
#include <device/graph.h>
#include <dsim/logtrace.h>
#include <dsim/device-lib.h>
#include <dsim/error.h>
#include <cstring>

#include "schemascene.h"
#include "boundingtraits.h"
#include "elementgraphitem.h"
#include "elementtext.h"
#include "elementwire.h"
#include "elementpin.h"
#include "componentgraphimpl.h"
#include "componentgraphitem.h"

namespace dsim
{

DECLARE_ELEMENT_CAST(ElementText, "text")
DECLARE_ELEMENT_CAST(ElementPin, "pin")

ComponentGraphItem::ComponentGraphItem( int id, SchemaScene *scene, bool edit, QGraphicsItem *parent )
                  : ElementGraphItem<QGraphicsItemGroup>( id, scene, edit, parent )
                  , m_device( 0l )
                  , m_deviceGraph( new ComponentGraphImpl() )
                  , m_symbolText( 0l )
                  , m_referenceText( 0l )
{
  setFlags( QGraphicsItemGroup::ItemIsSelectable | QGraphicsItemGroup::ItemSendsGeometryChanges );
  setFineturningEnabled(false);
}

ComponentGraphItem::~ComponentGraphItem()
{
  delete m_deviceGraph;
}

int ComponentGraphItem::init( const char *deviceEntry, ElementText *symbolText, ElementText *referenceText, bool deser )
{
  m_symbolText = symbolText;
  m_referenceText = referenceText;

  if( !deser )
    {
      addElement( m_symbolText ); // add sub element
      addElement( m_referenceText );

      m_symbolText->setStyle( "component" );
      m_referenceText->setStyle( "component" );
    }

  const DeviceLibraryEntry *entry = device_lib_entry( deviceEntry );
  if( entry )
    {
      if( !deser ) // to allocate a default reference
        {
          m_symbol = deviceEntry;
          m_reference = entry->reference_name;
          m_reference += "?";
        }

      int rc = view()->sheet()->createDevice( entry, m_reference.c_str(), id(), &m_device );

      UPDATE_RC(rc);

      if( m_device )
        {
          /*
           * Set up sub elements
           */
          m_symbolText->setText( deviceEntry );
          m_symbolText->setTextEditable( false );
          m_referenceText->setText( m_reference.c_str() );

          /*
           * Load component symbol and layout
           */
          if( !deser )
            {
              QPointF oldpos = pos(); QGraphicsItemGroup::setPos( QPointF( 0, 0) ); // ensure right offset aligned to grid

              rc = view()->loadSymbol( this, entry->symbolfile ); UPDATE_RC(rc);

              QGraphicsItemGroup::setPos( oldpos );
              setLayout();
            }

          return 0;
        }
    }

  return -DS_CREATE_IDEVICE; // the element has become invalid, and you should remove it later
}

void ComponentGraphItem::setLayout()
{
  if( m_symbolText && m_referenceText )
    {
      m_symbolText->setPos( pos() + QPointF( boundingRect().left(),
                                boundingRect().top() - m_symbolText->boundingRect().height() - 8 ) );
      m_referenceText->setPos( pos() + QPointF( boundingRect().left(),
                                boundingRect().top() - m_symbolText->boundingRect().height() - m_referenceText->boundingRect().height() - 8 ) );
    }
}

void ComponentGraphItem::setPos( const QPointF &pos )
{
  QGraphicsItemGroup::setPos( pos + QPointF( gridWh/2, gridHt/2 ) ); // force align to grid
}

int ComponentGraphItem::addComponentElement( ElementBase *element )
{
  addToGroup( element->graphicsItem() );
  return ElementGraphItem::addElement( element );
}

ElementPin *ComponentGraphItem::atPin( const QPointF &pos )
{
  for( QList<ElementBase *>::iterator it = elements().begin()+2; it!=elements().end(); ++it )
    {
      if( 0==std::strcmp( (*it)->classname(), "pin" ) )
        {
          QRectF bounding = (*it)->boundingRect();
          bounding = (*it)->graphicsItem()->sceneTransform().mapRect( bounding );

          if( bounding.contains( pos ) )
            {
              return static_cast<ElementPin *>(*it);
            }
        }
    }
  return 0l;
}

std::string ComponentGraphItem::reference()
{ return m_referenceText->text().toStdString(); }

int ComponentGraphItem::resolveSubElements()
{
  int rc = ElementGraphItem::resolveSubElements(); UPDATE_RC(rc);

  QList<ElementBase *>::const_iterator it = elements().begin();

  m_symbolText = element_cast<ElementText *>(*it++);
  m_referenceText = element_cast<ElementText *>(*it++);

  if( 0l==m_symbolText || 0l==m_referenceText )
    return -DS_INVALID_ELEMENT_ID;

  QPointF oldpos = pos(); QGraphicsItemGroup::setPos( QPointF( 0, 0) ); // ensure right offset aligned to grid
  for( ; it!=elements().end(); ++it )
    {
      addToGroup( (*it)->graphicsItem() ); // 'the item and child items will be reparented to this group' (Qt Documents)
    }
  QGraphicsItemGroup::setPos( oldpos );

  return init( m_symbol.c_str(), m_symbolText, m_referenceText, /*deser*/true );
}

void ComponentGraphItem::releaseSubElements()
{
  if( device() )
    {
      view()->sheet()->deleteDevice( device() );
    }
  if( !elements().empty() )
    {
      if( elements().count() > 2 )
        {
          QList<ElementBase *>::const_iterator it;
          for( it = elements().begin()+2; it!=elements().end(); ++it )
            {
              (*it)->graphicsItem()->setParentItem( 0l );
            }
        }

      foreach( ElementBase *element, elements() )
        {
          // we assume that this operation( removing connected wires ) will not affect the list elements(),
          // however not the global element list.
          if( 0==std::strcmp( element->classname(), "pin" ) )
            {
              ElementPin *pin = static_cast<ElementPin *>(element);
              ElementWire *wire = pin->connectedWire();
              if( wire )
                {
                  view()->releaseElement( wire );
                }
            }
        }
    }
  ElementGraphItem::releaseSubElements();
}

int ComponentGraphItem::serialize( LispDataset *dataset )
{
  int rc = ElementGraphItem<QGraphicsItemGroup>::serialize( dataset ); UPDATE_RC(rc);

  rc = dataset->ser( double(pos().x()) );                   UPDATE_RC(rc);
  rc = dataset->ser( double(pos().y()) );                   UPDATE_RC(rc);
  rc = dataset->ser( m_symbol );                            UPDATE_RC(rc);
  rc = dataset->ser( reference() );                         UPDATE_RC(rc);

  return rc;
}

int ComponentGraphItem::deserialize( LispDataset *dataset )
{
  int rc = ElementGraphItem<QGraphicsItemGroup>::deserialize( dataset ); UPDATE_RC(rc);

  double x, y;
  std::string symbol;
  rc = dataset->des( x );                                   UPDATE_RC(rc);
  rc = dataset->des( y );                                   UPDATE_RC(rc);
  rc = dataset->des( m_symbol );                            UPDATE_RC(rc);
  rc = dataset->des( m_reference );                         UPDATE_RC(rc);

  QGraphicsItemGroup::setPos( QPointF( x, y ) );
  return 0;
}

QVariant ComponentGraphItem::itemChange( GraphicsItemChange change, const QVariant &value )
{
  if( change == ItemPositionHasChanged )
    {
      foreach( ElementBase *element, elements() )
        {
          if( 0==std::strcmp( element->classname(), "pin" ) )
            {
              ElementPin *pin = static_cast<ElementPin *>(element);
              ElementWire *wire = pin->connectedWire();
              if( wire )
                {
                  wire->layoutWires( pin, pin->portScenePos() );
                }
            }
        }
    }
  return value;
}

void ComponentGraphItem::mouseReleaseEvent( QGraphicsSceneMouseEvent *event )
{
  ElementGraphItem::mouseReleaseEvent( event );

  foreach( ElementBase *element, elements() )
    {
      if( 0==std::strcmp( element->classname(), "pin" ) )
        {
          ElementPin *pin = static_cast<ElementPin *>(element);
          ElementWire *wire = pin->connectedWire();
          if( wire )
            {
              wire->removeNullWires();
            }
        }
    }
}

void ComponentGraphItem::paint( QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget )
{
  Q_UNUSED(option); Q_UNUSED(widget);

  QPen pen(QColor( 0, 0, 128 ), 1.0f, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);

  if ( QGraphicsItemGroup::isSelected() )
    {
      pen.setColor( Qt::red);
    }

  painter->setPen( pen );

#if 0
  painter->drawRect( boundingRect() );
  pen.setWidth( 5 );
  painter->setPen( pen );
  painter->drawPoint( QPointF(0,0) );
#endif

  /*
   * Render component graphics
   */
  if( m_device )
    {
      m_deviceGraph->setPainter( painter );
      m_deviceGraph->setSelected( isSelected() );

      m_device->render_frame( m_deviceGraph );
    }
}

}
