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

#include <device/device.h>
#include <device/graph.h>
#include <dsim/device-lib.h>
#include <dsim/error.h>

#include "schemagraph.h"
#include "elementgraphitem.h"
#include "elementtext.h"
#include "componentgraph.h"
#include "componentgraphitem.h"

namespace dsim
{

ComponentGraphItem::ComponentGraphItem( int id, SchemaGraph *scene, bool edit, QGraphicsItem *parent )
                  : ElementGraphItem<QGraphicsItem>( id, scene, edit, parent )
                  , m_device( 0l )
                  , m_deviceGraph( new ComponentGraph() )
                  , m_symbolText( 0l )
                  , m_referenceText( 0l )
{
  QGraphicsItem::setFlag( QGraphicsItem::ItemIsSelectable, true );
}

ComponentGraphItem::~ComponentGraphItem()
{
  delete m_deviceGraph;
}

void ComponentGraphItem::setBoundingRect( const QRect &bounding )
{ prepareGeometryChange(); m_bounding = bounding; }

int ComponentGraphItem::init( const char *deviceEntry, ElementText *symbolText, ElementText *referenceText, bool deser )
{
  m_symbolText = symbolText; addElement( m_symbolText );    // add sub element
  m_referenceText = referenceText; addElement( m_referenceText );

  m_symbolText->setStyle( "component" );
  m_referenceText->setStyle( "component" );

  const DeviceLibraryEntry *entry = device_lib_entry( deviceEntry );
  if( entry )
    {
      std::string ref;
      if( deser )
        {
          ref = m_reference; // specified reference
        }
      else // to allocate a default reference
        {
          ref = entry->reference_name;
          ref += "?";
        }
      m_device = view()->sheet()->createDevice( entry, ref.c_str(), id() );

      if( m_device )
        {
          m_symbol = deviceEntry;

          /*
           * Set up sub elements
           */
          m_symbolText->setText( deviceEntry );
          m_symbolText->setTextEditable( false );
          m_referenceText->setText( ref.c_str() );

          IRECT boundRect = m_device->get_bound();
          setBoundingRect( QRect( boundRect.x, boundRect.y, boundRect.w, boundRect.h ) );

          if( !deser ) setLayout();
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

std::string ComponentGraphItem::reference()
{ return m_referenceText->text().toStdString(); }

int ComponentGraphItem::resolveSubElements()
{
  int rc = ElementGraphItem::resolveSubElements(); UPDATE_RC(rc);

  m_symbolText = static_cast<ElementText *>(elements()[0]);
  m_referenceText = static_cast<ElementText *>(elements()[1]);

  return init( m_symbol.c_str(), m_symbolText, m_referenceText, /*deser*/true );
}

int ComponentGraphItem::serialize( LispDataset *dataset )
{
  int rc = ElementGraphItem<QGraphicsItem>::serialize( dataset ); UPDATE_RC(rc);

  rc = dataset->ser( double(pos().x()) );                   UPDATE_RC(rc);
  rc = dataset->ser( double(pos().y()) );                   UPDATE_RC(rc);
  rc = dataset->ser( m_symbol );                            UPDATE_RC(rc);
  rc = dataset->ser( reference() );                         UPDATE_RC(rc);

  return rc;
}

int ComponentGraphItem::deserialize( LispDataset *dataset )
{
  int rc = ElementGraphItem<QGraphicsItem>::deserialize( dataset ); UPDATE_RC(rc);

  double x, y;
  rc = dataset->des( x );                                   UPDATE_RC(rc);
  rc = dataset->des( y );                                   UPDATE_RC(rc);
  rc = dataset->des( m_symbol );                            UPDATE_RC(rc);
  rc = dataset->des( m_reference );                         UPDATE_RC(rc);

  setPos( QPointF( x, y ) );
  return 0;
}

void ComponentGraphItem::setSubs( ElementText *symbolText, ElementText *referenceText )
{

}

void ComponentGraphItem::paint( QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget )
{
  Q_UNUSED(option); Q_UNUSED(widget);

  QPen pen(QColor( 0, 0, 128 ), 1.0f, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);

  if ( QGraphicsItem::isSelected() )
    {
      pen.setColor( Qt::red);
    }

  painter->setPen( pen );

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
