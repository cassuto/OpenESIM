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

#include "lispdataset.h"
#include "componentgraphitem.h"
#include "elementtext.h"
#include "pinsettingsdialog.h"

#include "elementpin.h"

namespace dsim
{

ElementPin::ElementPin( ElemDirect direct, const QPointF &pos, ElementText *symbol, ElementText *reference, int id, SchemaGraph *scene, bool editable, QGraphicsItem *parent )
          : ElementGraphItem<QGraphicsItem>( id, scene, editable, parent )
          , m_symbolLabel( 0l )
          , m_referenceLabel( 0l )
          , m_showSymbol( true )
          , m_showReference( true )
{
  setStyle( "pin" );
  setBoundingRect( QRect(-4, -4, 12, 8) );

  setPos( pos );
  setDirect( direct );
  setLength( 32 );

  if( !editable ) setCursor( Qt::CrossCursor );

  setSub( symbol, reference );
}

ElementPin::~ElementPin()
{
}

void ElementPin::setBoundingRect( const QRect &bounding )
{ prepareGeometryChange(); m_bounding = bounding; }

void ElementPin::setDirect( ElemDirect direct )
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

  setLayout();
}

void ElementPin::setLength( int length )
{
  m_length = length;

  setBoundingRect( QRect(-4, -4, m_length + 4, 8) );
}

void ElementPin::setSub( ElementText *symbol, ElementText *reference )
{
  if( 0l==m_symbolLabel && symbol && 0l==m_referenceLabel && reference )
    {
      m_symbolLabel = symbol;           addElement( symbol );
      m_referenceLabel = reference;     addElement( reference );

      m_symbolLabel->setStyle( "pin" );
      m_referenceLabel->setStyle( "pin" );
      setLayout();
    }
}

void ElementPin::setLayout()
{
  if( !m_symbolLabel || !m_referenceLabel ) return;

  updateReferenceLabel();

  QRectF symbolBound = m_symbolLabel->boundingRect();

  int xlabelpos = pos().x();
  int ylabelpos = pos().y();

  switch( m_direct )
  {
    case ELEM_RIGHT:
      xlabelpos -= symbolBound.width()+m_length+4;
      ylabelpos -= 5;
      m_symbolLabel->setDirect( ELEM_LEFT );
      break;

    case ELEM_TOP:
      xlabelpos += 5;
      ylabelpos += m_length+4;
      m_symbolLabel->setDirect( m_direct );
      break;

    case ELEM_LEFT:
      xlabelpos += m_length+4;
      ylabelpos -= 5;
      m_symbolLabel->setDirect( ELEM_LEFT );
      break;

    case ELEM_BOTTOM:
      xlabelpos -= 5;
      ylabelpos -= m_length+4;
      m_symbolLabel->setDirect( m_direct );
      break;
  }
  m_symbolLabel->setPos( QPointF(xlabelpos, ylabelpos) );
}

void ElementPin::updateReferenceLabel()
{
  if( !m_symbolLabel || !m_referenceLabel ) return;

  updateVisible();

  QRectF symbolBound = m_symbolLabel->boundingRect();

  int xlabelpos = pos().x();
  int ylabelpos = pos().y();

  switch( m_direct )
  {
    case ELEM_RIGHT:
      xlabelpos -= symbolBound.width()+m_length+4;
      ylabelpos -= 5;
      m_referenceLabel->setDirect( ELEM_LEFT );
      m_referenceLabel->setPos( QPointF(xlabelpos + symbolBound.width() + 8, ylabelpos - m_referenceLabel->boundingRect().height()/2 ) );
      break;

    case ELEM_TOP:
      xlabelpos += 5;
      ylabelpos += m_length+4;
      m_referenceLabel->setDirect( m_direct );
      m_referenceLabel->setPos( QPointF(xlabelpos - m_referenceLabel->boundingRect().height()/2, ylabelpos - m_referenceLabel->boundingRect().width() - 8 ) );
      break;

    case ELEM_LEFT:
      xlabelpos += m_length+4;
      ylabelpos -= 5;
      m_referenceLabel->setDirect( ELEM_LEFT );
      m_referenceLabel->setPos( QPointF(xlabelpos - m_referenceLabel->boundingRect().width() - 8, ylabelpos - m_referenceLabel->boundingRect().height()/2  ) );
      break;

    case ELEM_BOTTOM:
      xlabelpos -= 5;
      ylabelpos -= m_length+4;
      m_referenceLabel->setDirect( m_direct );
      m_referenceLabel->setPos( QPointF(xlabelpos - m_referenceLabel->boundingRect().height()/2, ylabelpos + symbolBound.width() + 8 ) );
      break;
  }
}

void ElementPin::updateVisible()
{
  m_symbolLabel->setVisible( m_showSymbol );
  m_referenceLabel->setVisible( m_showReference );
}

int ElementPin::serialize( LispDataset *dataset )
{
  int rc = ElementGraphItem::serialize( dataset );  UPDATE_RC(rc);

  rc = dataset->ser( double(pos().x()) );           UPDATE_RC(rc);
  rc = dataset->ser( double(pos().y()) );           UPDATE_RC(rc);
  rc = dataset->ser( int(m_length) );               UPDATE_RC(rc);
  rc = dataset->ser( m_showSymbol );                UPDATE_RC(rc);
  rc = dataset->ser( m_showReference );             UPDATE_RC(rc);

  return rc;
}

int ElementPin::deserialize( LispDataset *dataset )
{
  int rc = ElementGraphItem::deserialize( dataset ); UPDATE_RC(rc);

  double x, y;
  int length;

  rc = dataset->des( x );                           UPDATE_RC(rc);
  rc = dataset->des( y );                           UPDATE_RC(rc);
  rc = dataset->des( length );                      UPDATE_RC(rc);
  rc = dataset->des( m_showSymbol );                UPDATE_RC(rc);
  rc = dataset->des( m_showReference );             UPDATE_RC(rc);

  setPos( QPointF( x, y ) );
  setLength( length );
  return rc;
}

int ElementPin::resolveSubElements()
{
  int rc = ElementGraphItem::resolveSubElements(); UPDATE_RC(rc);

  m_symbolLabel = static_cast<ElementText *>(elements()[0]);
  m_referenceLabel = static_cast<ElementText *>(elements()[1]);

  updateVisible();
  return 0;
}

QString ElementPin::symbol() const
{ return m_symbolLabel->text(); }

QString ElementPin::reference() const
{ return m_referenceLabel->text(); }

void ElementPin::setSymbol( const QString &symbol )
{ m_symbolLabel->setText( symbol ); }

void ElementPin::setReference( const QString &reference )
{ m_referenceLabel->setText( reference ); updateReferenceLabel(); }


void ElementPin::mouseDoubleClickEvent( QGraphicsSceneMouseEvent *event )
{
  PinSettingsDialog pinSettings( this );

  if( pinSettings.exec() == QDialog::Accepted )
    {
      pinSettings.apply( this );
    }
}

void ElementPin::paint( QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget )
{
  Q_UNUSED(option); Q_UNUSED(widget);

  Templatestyle::apply( painter, customLine(), customFill(), style(), isSelected() );

  QPen pen = painter->pen();
  pen.setCapStyle( Qt::SquareCap );
  pen.setJoinStyle( Qt::RoundJoin );
  painter->setPen( pen );

  if( m_length < 1 ) m_length = 1;
  painter->drawLine( 0, 0, m_length-1, 0);
/*
  pen.setColor( Qt::red );
  pen.setWidthF( 0.7 );
  painter->setPen( pen );
  painter->drawEllipse( -4, -4, 8, 8 );
  */
}

}
