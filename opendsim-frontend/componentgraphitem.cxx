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
#include <frontend/device-lib.h>
#include <frontend/error.h>
#include <cstring>

#include "schemascene.h"
#include "boundingtraits.h"
#include "elementgraphitem.h"
#include "elementtext.h"
#include "elementwire.h"
#include "elementpin.h"
#include "elementorigin.h"
#include "propertycontainerimpl.h"
#include "componentgraphimpl.h"
#include "componentgraphitem.h"

namespace dsim
{

DECLARE_ELEMENT_CAST(ElementText, "text")
DECLARE_ELEMENT_CAST(ElementPin, "pin")

ComponentGraphItem::ComponentGraphItem( int id, SchemaScene *scene, bool edit, QGraphicsItem *parent )
                  : ElementGraphItem<QGraphicsItemGroup>( id, scene, edit, parent )
                  , m_deviceEntry( 0l )
                  , m_device( new PointerProxy<IDevice>( 0l ) )
                  , m_valueText( 0l )
                  , m_referenceText( 0l )
                  , m_properties( new PropertyContainerImpl )
                  , m_deviceGraph( new ComponentGraphImpl )
{
  m_schematic = new SchematicImpl( view(), this );

  setFlags( QGraphicsItemGroup::ItemIsSelectable | QGraphicsItemGroup::ItemSendsGeometryChanges );
  setFineturningEnabled( false );
}

ComponentGraphItem::~ComponentGraphItem()
{
  delete schematic();
  delete deviceGraph();
  delete properties();
  delete device()->get();
  delete device();
}

int ComponentGraphItem::init( const char *deviceEntry, ElementText *valueText, ElementText *referenceText )
{
  int rc;
  bool deser = ( device()->valid() );
  m_valueText = valueText;
  m_referenceText = referenceText;

  if( !deser )
    {
      addElement( m_valueText ); // add sub element
      addElement( m_referenceText );

      m_valueText->setStyle( "component" );
      m_referenceText->setStyle( "component" );

      rc = findEntry( deviceEntry );    UPDATE_RC(rc);

      m_symbol = deviceEntry;
      m_reference = m_deviceEntry->reference_name; // to allocate a default reference

      rc = createDevice();              UPDATE_RC(rc);

      if( properties()->valued() )
        m_value = properties()->value(); // if there is no default value, use the symbol instead
      else
        m_value = deviceEntry;
    }

  /*
   * Set up sub elements
   */
  setValue( m_value.c_str() );
  setReference( m_reference.c_str() );

  /*
   * Load component symbol and layout
   */
  if( !deser )
    {
      rc = view()->loadSymbol( this, m_deviceEntry->symbolfile ); UPDATE_RC(rc);

      setLayout();
    }
  return 0;
}

int ComponentGraphItem::createDevice()
{
  trace_assert( m_deviceEntry );

  delete device()->get();
  device()->erase();

  schematic()->reset();
  //properties()->reset();

  IDevice *dev = 0l;
  if( int rc = view()->sheet()->createDevice( m_deviceEntry, m_reference.c_str(), id(), schematic(), properties(), &dev ) )
    return rc;
  device()->set( dev );

  return dev ? 0 : -DS_CREATE_IDEVICE;
}

int ComponentGraphItem::initDevice()
{
  syncParameters();
  return device()->valid() ? device()->get()->init( schematic(), properties() ) : 0;
}

int ComponentGraphItem::findEntry( const char *symbol )
{
  if( !(m_deviceEntry = device_lib_entry( symbol )) )
    return -DS_FIND_DEVICE;
  return 0;
}

void ComponentGraphItem::setLayout()
{
  if( m_valueText && m_referenceText )
    {
      m_valueText->setPos( pos() + QPointF( boundingRect().left(),
                            boundingRect().top() - m_valueText->boundingRect().height() - 8 ) );
      m_referenceText->setPos( pos() + QPointF( boundingRect().left(),
                              boundingRect().top() - m_valueText->boundingRect().height() - m_referenceText->boundingRect().height() - 8 ) );
    }
}

void ComponentGraphItem::setDirect( ElemDirect direct )
{
  ElementGraphItem::setDirect( direct );
  if( elements().size() > 2 )
    {
      switch( direct )
      {
        case ELEM_RIGHT: setRotation( 180 ); break;
        case ELEM_TOP: setRotation( 90 ); break;
        case ELEM_LEFT: setRotation( 0 ); break;
        case ELEM_BOTTOM: setRotation( -90 ); break;
        default: return;
      }

      foreach( ElementBase *element, elements() )
        {
          if( 0==std::strcmp( element->classname(), "pin" ) )
            {
              ElementPin *pin = static_cast<ElementPin *>(element);
              if( ElementWire *wire = pin->connectedWire() )
                {
                  wire->layoutWires( pin, pin->portScenePos() );
                }
            }
        }
    }
}

int ComponentGraphItem::addComponentElement( ElementBase *element )
{
  addComponentElementInner( element );
  return ElementGraphItem::addElement( element );
}

void ComponentGraphItem::addComponentElementInner( ElementBase *element )
{
  if( 0==std::strcmp( element->classname(), "pin" ) )
    {
      ElementPin *pin = static_cast<ElementPin *>(element);
      pin->setComponent( this );
    }
  addToGroup( element->graphicsItem() );  // 'the item and child items will be reparented to this group' (Qt Documents)
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

QList<ElementBase *>::iterator ComponentGraphItem::pinsBegin()
{
  if( elements().size() == 0 )
    return elements().end();
  return elements().begin()+2;
}

QList<ElementBase *>::iterator ComponentGraphItem::pinsEnd()
{ return elements().end(); }

std::string ComponentGraphItem::reference()
{ return m_referenceText->text().toStdString(); }

std::string ComponentGraphItem::value()
{ return m_valueText->text().toStdString(); }

std::string ComponentGraphItem::symbol()
{ return m_symbol; }

void ComponentGraphItem::setReference( const QString &reference )
{ m_reference = reference.toStdString(); m_referenceText->setText( reference ); }

void ComponentGraphItem::setValue( const QString &value )
{ m_valueText->setText( value ); }

void ComponentGraphItem::syncParameters() // It's preferred to call this function before reading/writing properties
{
  properties()->setValue( value() );
}

int ComponentGraphItem::resolveSubElements()
{
  int rc = ElementGraphItem::resolveSubElements(); UPDATE_RC(rc);

  QList<ElementBase *>::const_iterator it = elements().begin();

  m_valueText = element_cast<ElementText *>(*it++);
  m_referenceText = element_cast<ElementText *>(*it++);

  if( 0l==m_valueText || 0l==m_referenceText )
    return -DS_INVALID_ELEMENT_ID;

  ElementOrigin *originElement = 0l;
  foreach( ElementBase *element, elements() )
    {
      if( 0==std::strcmp( element->classname(), "origin" ) )
        { originElement = static_cast<ElementOrigin *>(element); break; }
    }

  QPointF origin;
  if( originElement )
    { origin = originElement->originPos(); }
  else
    {
      /*
       * there is no origin information, using the default origin.
       * Unnecessary to consider grid alignment as the position of origin is relative to the origin of QGraphicsItemGroup. After an graphics item
       * is belonged to a QGraphicsItemGroup, whose position will represent the relative value, so the origin should always be located at (0, 0)...
       */
      origin = QPointF( 0, 0 );
    }

  QPointF oldpos = pos(); QGraphicsItemGroup::setPos( origin );
  for( ; it!=elements().end(); ++it )
    {
      addComponentElementInner( *it );
    }
  QGraphicsItemGroup::setPos( oldpos );

  setDirect( direct() );
  return init( m_symbol.c_str(), m_valueText, m_referenceText );
}

void ComponentGraphItem::releaseSubElements()
{
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
  rc = dataset->ser( symbol() );                            UPDATE_RC(rc);
  rc = dataset->ser( value() );                             UPDATE_RC(rc);
  rc = dataset->ser( reference() );                         UPDATE_RC(rc);
  rc = properties()->serialize( dataset );                  UPDATE_RC(rc);

  return rc;
}

int ComponentGraphItem::deserialize( LispDataset *dataset )
{
  int rc = ElementGraphItem<QGraphicsItemGroup>::deserialize( dataset ); UPDATE_RC(rc);

  double x, y;
  rc = dataset->des( x );                                   UPDATE_RC(rc);
  rc = dataset->des( y );                                   UPDATE_RC(rc);
  rc = dataset->des( m_symbol );                            UPDATE_RC(rc);
  rc = dataset->des( m_value );                             UPDATE_RC(rc);
  rc = dataset->des( m_reference );                         UPDATE_RC(rc);

  rc = findEntry( m_symbol.c_str() );                       UPDATE_RC(rc);
  properties()->reset();
  rc = createDevice();                                      UPDATE_RC(rc);
  rc = properties()->deserialize( dataset );                UPDATE_RC(rc);

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
  event->ignore();
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
  if( m_device->valid() )
    {
      m_deviceGraph->setPainter( painter );
      m_deviceGraph->setSelected( isSelected() );

      m_device->get()->render_frame( schematic(), m_deviceGraph );
    }
}

}
