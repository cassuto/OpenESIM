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

#include <dsim/error.h>

#include "templatestyle.h"
#include "lispdataset.h"
#include "textsettingsdialog.h"

#include "elementtext.h"

namespace dsim
{

ElementText::ElementText( ElemDirect direct, const QPointF &pos, int id, SchemaGraph *scene, bool edit, QGraphicsSimpleTextItem* parent )
          : ElementGraphItem<QGraphicsSimpleTextItem>( id, scene, edit, parent )
          , m_textEditable( true )
{
  setPos( pos );
  setDirect( direct );

  setStyle( "component" );
  setText("");
  setRotation( 0 );
}

ElementText::~ElementText()
{
}

void ElementText::setStyle( const char *style )
{
  ElementGraphItem::setStyle( style );
  update();
}

void ElementText::setDirect( ElemDirect direct )
{
  ElementGraphItem::setDirect( direct );

  switch( direct )
  {
    case ELEM_RIGHT: setRotation( 180 ); break;
    case ELEM_TOP: setRotation( 90 ); break;
    case ELEM_LEFT: setRotation( 0 ); break;
    case ELEM_BOTTOM: setRotation( -90 ); break;
    default: return;
  }
}

void ElementText::setTextEditable( bool editable )
{ m_textEditable = editable; }

int ElementText::serialize( LispDataset *dataset )
{
  int rc = ElementGraphItem::serialize( dataset );          UPDATE_RC(rc);

  std::string symbol = text().toStdString();

  rc = dataset->ser( double(pos().x()) );                   UPDATE_RC(rc);
  rc = dataset->ser( double(pos().y()) );                   UPDATE_RC(rc);
  rc = dataset->ser( symbol );                              UPDATE_RC(rc);

  return rc;
}

int ElementText::deserialize( LispDataset *dataset )
{
  int rc = ElementGraphItem::deserialize( dataset );        UPDATE_RC(rc);

  double x, y;
  std::string symbol;

  rc = dataset->des( x );                                   UPDATE_RC(rc);
  rc = dataset->des( y );                                   UPDATE_RC(rc);
  rc = dataset->des( symbol );                              UPDATE_RC(rc);

  setPos( QPointF( x, y ) );
  setText( symbol.c_str() );
  return rc;
}

void ElementText::mouseDoubleClickEvent( QGraphicsSceneMouseEvent *event )
{
  if( event->button() == Qt::LeftButton )
    {
      if( m_textEditable )
        {
          TextSettingsDialog settings( text() );
          if( settings.exec() == QDialog::Accepted )
            {
              setText( settings.text() );
            }
        }
    }
}

void ElementText::paint( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget )
{
  Templatestyle::apply( static_cast<QGraphicsSimpleTextItem *>(this), customText(), style(), isSelected() );

  QGraphicsSimpleTextItem::paint( painter, option, widget );
}

}
