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

#include <string>

#include "schemagraph.h"
#include "elementgraphitem.h"
#include "componentgraphitem.h"
#include "elementline.h"
#include "elementtext.h"
#include "elementpin.h"
#include "elementrect.h"

#include "schemaview.h"

namespace dsim
{

void SchemaView::setMode( DrawMode mode )
{
  m_mode = mode;

  switch( mode )
  {
    case MODE_SELECTION:
      this->setCursor( QCursor( Qt::ArrowCursor ));
      break;
    case MODE_PIN:
    case MODE_LINE:
    case MODE_TEXT:
    case MODE_RECT:
    case MODE_ROUND:
    case MODE_SCRIPT:
    case MODE_COMPONENT:
      this->setCursor( QCursor( Qt::CrossCursor ));
      break;
  }

  switch( mode )
  {
    case MODE_SELECTION:
      if ( m_hintElement )
        {
          removeElement( m_hintElement );
          m_hintElement = 0l;
        }
      m_moving = false;

      m_mousePressEvent = &mousePressSelect;
      m_mouseMoveEvent = &mouseMoveSelect;
      m_mouseReleaseEvent = &mouseReleaseSelect;
      m_keyPressEvent = 0l;
      break;

    case MODE_PIN:
      {
        ElementBase *symbol = createElement( "text", QPoint( 0, 0 ) );
        ((ElementText *)symbol)->setText( "PIN 1" );

        m_hintElement = createElement( "pin", QPoint( 0, 0 ) );
        ((ElementPin *)m_hintElement)->setSub( (ElementText *)symbol );

        ((ElementPin *)m_hintElement)->setVisible( false );

        m_mousePressEvent = &mousePressPin;
        m_mouseMoveEvent = &mouseMovePin;
        m_mouseReleaseEvent = 0l;
        m_keyPressEvent = &keyPressPin;
        break;
      }

    case MODE_LINE:
      {
        m_hintElement = createElement( "line", QPoint( 0, 0 ) );
        ((ElementLine *)m_hintElement)->setVisible( false );

        m_mousePressEvent = &mousePressLine;
        m_mouseMoveEvent = &mouseMoveLine;
        m_mouseReleaseEvent = &mouseReleaseLine;
        m_keyPressEvent = 0l;
        break;
      }
      break;
    case MODE_TEXT:
      {
        m_hintElement = createElement( "text", QPoint( 0, 0 ) );
        ((ElementText *)m_hintElement)->setVisible( false );
        break;
      }
    case MODE_RECT:
      {
        m_hintElement = createElement( "rect", QPoint( 0, 0 ) );
        ((ElementRect *)m_hintElement)->setVisible( false );

        m_mousePressEvent = &mousePressRect;
        m_mouseMoveEvent = &mouseMoveRect;
        m_mouseReleaseEvent = &mouseReleaseRect;
        m_keyPressEvent = 0l;
        break;
      }
    case MODE_ROUND:
    case MODE_SCRIPT:
    case MODE_COMPONENT:
      break;
  }
}

// ------------------------------------------------------------------ //
// Mouse actions for selection mode
// ------------------------------------------------------------------ //

bool SchemaView::mousePressSelect( QMouseEvent *event )
{ return true; }

bool SchemaView::mouseMoveSelect( QMouseEvent *event )
{ return true; }

bool SchemaView::mouseReleaseSelect( QMouseEvent *event )
{ return true; }

// ------------------------------------------------------------------ //
// Mouse actions for component mode
// ------------------------------------------------------------------ //

bool SchemaView::mousePressComponent( QMouseEvent *event )
{
  m_hintComponent->setLayout();
  m_hintComponent = 0l;

  setMode( MODE_SELECTION );
  return true;
}

bool SchemaView::mouseMoveComponent( QMouseEvent *event )
{
  m_hintComponent->setPos( togrid( mapToScene( event->pos() ) ) );
  m_hintComponent->setLayout();
  m_hintComponent->setVisible( true );
  return true;
}

// ------------------------------------------------------------------ //
// Mouse actions for pin mode
// ------------------------------------------------------------------ //

bool SchemaView::mousePressPin( QMouseEvent *event )
{
  m_hintElement->setLayout();
  m_hintElement = 0l;

  setMode( MODE_SELECTION );
  return true;
}

bool SchemaView::mouseMovePin( QMouseEvent *event )
{
  ((ElementPin *)m_hintElement)->setPos( togrid( mapToScene( event->pos() ) ) );
  m_hintElement->setLayout();
  ((ElementPin *)m_hintElement)->setVisible( true );
  return true;
}

// ------------------------------------------------------------------ //
// Mouse actions for line mode
// ------------------------------------------------------------------ //

bool SchemaView::mousePressLine( QMouseEvent *event )
{
  if( event->button() == Qt::LeftButton )
    {
      ElementLine *lineElement = static_cast<ElementLine *>(m_hintElement);
      QPointF cp = togrid( mapToScene( event->pos() ) );

      if( m_moving == false ) // set the origin
        {
          lineElement->setPos( cp );
        }
      lineElement->addPoint( cp );

      m_moving = true;
      event->accept();
      return false;
    }
  else if( event->button() == Qt::RightButton )
    {
      ElementLine *lineElement = static_cast<ElementLine *>(m_hintElement);
      if( lineElement->pointCount() > 0 )
        {
          m_hintElement = 0l; // accept this line
        }
      setMode( MODE_SELECTION );

      event->accept();
      return false;
    }
  return true;
}

bool SchemaView::mouseMoveLine( QMouseEvent *event )
{
  if( m_moving )
    {
      ElementLine *line = static_cast<ElementLine *>(m_hintElement);

      line->setLastPoint( togrid( mapToScene( event->pos() ) ) );

      event->accept();
      return false;
    }
  return true;
}

bool SchemaView::mouseReleaseLine( QMouseEvent *event )
{
  if( m_moving )
    {
      m_hintElement->setLayout();
      m_hintElement->graphicsItem()->setVisible( true );

      m_moving = false;
      event->accept();
      return false;
    }
  return true;
}

// ------------------------------------------------------------------ //
// Mouse actions for rectangle mode
// ------------------------------------------------------------------ //

bool SchemaView::mousePressRect( QMouseEvent *event )
{
  if( event->button() == Qt::LeftButton )
    {
      ElementRect *rectElement = static_cast<ElementRect *>(m_hintElement);
      rectElement->setPos( togrid( mapToScene( event->pos() ) ) );

      m_moving = true;
      event->accept();
      return false;
    }
  else if( event->button() == Qt::RightButton )
    {
      setMode( MODE_SELECTION ); // give up drawing

      event->accept();
      return false;
    }
  return true;
}

bool SchemaView::mouseMoveRect( QMouseEvent *event )
{
  if( m_moving )
    {
      ElementRect *rectElement = static_cast<ElementRect *>(m_hintElement);
      QRectF rect = rectElement->rect();
      rect.setBottomRight( togrid( mapToScene( event->pos() ) ) );
      rectElement->setRect( rect );

      event->accept();
      return false;
    }
  return true;
}

bool SchemaView::mouseReleaseRect( QMouseEvent *event )
{
  if( m_moving )
    {
      ElementRect *rectElement = static_cast<ElementRect *>(m_hintElement);
      QRectF rect = rectElement->rect();

      if( rect.topLeft() != rect.bottomRight() ) // accept this rectangle
        {
          if( rect.width() < gridWh ) rect.setWidth( gridWh );
          if( rect.height() < gridHt ) rect.setHeight( gridHt );

          rectElement->setRect( rect );
          rectElement->setLayout();
          m_hintElement->graphicsItem()->setVisible( true );
        }

      m_moving = false;

      setMode( MODE_SELECTION );
      event->accept();
      return false;
    }
  return true;
}

// ------------------------------------------------------------------ //
// Mouse actions for dragging
// ------------------------------------------------------------------ //

void SchemaView::dragEnterEvent( QDragEnterEvent *event )
{
  event->accept();

  QString text = event->mimeData()->text();
  std::string symbol = text.toStdString();
#if 0
  IDevice *hintDevice = m_sheet->createDevice( symbol.c_str(), "", 1 );
  if( hintDevice )
    {
      m_hintComponent = new ComponentGraphItem( m_id[stack].alloc(), true, hintDevice, m_schemaGraph );
      m_hintComponent->setPos( mapToScene( event->pos() ) );
      m_hintComponent->setLayout();
      m_hintComponent->addToScene( m_schemaGraph );

      m_mousePressEvent = &mousePressComponent;
      m_mouseMoveEvent = &mouseMoveComponent;
      m_mouseReleaseEvent = 0l;
      m_keyPressEvent = &keyPressComponent;
    }
#endif
}

void SchemaView::dragMoveEvent( QDragMoveEvent *event )
{
  event->accept();

  if( m_hintComponent ) m_hintComponent->setPos( togrid( mapToScene( event->pos() ) ) );
}

void SchemaView::dragLeaveEvent( QDragLeaveEvent *event )
{
  event->accept();

  setMode( MODE_SELECTION );
}

void SchemaView::dropEvent( QGraphicsSceneDragDropEvent *event )
{
  event->accept();

  m_hintComponent->setLayout();
  m_hintComponent = 0l;

  setMode( MODE_SELECTION );
}

// ------------------------------------------------------------------ //
// Keyboard actions for component mode
// ------------------------------------------------------------------ //

bool SchemaView::keyPressComponent( QKeyEvent *event )
{
  if( event->key() == Qt::Key_Space )
    {
      if( m_hintElement )
        {
          if( m_hintDirect < ELEM_BOTTOM )
            ++m_hintDirect;
          else
            m_hintDirect = 0;
          ((ComponentGraphItem *)m_hintElement)->setDirect( (ElemDirect)m_hintDirect );
          return true;
        }
    }
  return false;
}

bool SchemaView::keyPressPin( QKeyEvent *event )
{
  if( event->key() == Qt::Key_Space )
    {
      if( m_hintElement )
        {
          if( m_hintDirect < ELEM_BOTTOM )
            ++m_hintDirect;
          else
            m_hintDirect = 0;
          ((ElementPin *)m_hintElement)->setDirect( (ElemDirect)m_hintDirect );
          return true;
        }
    }
  return false;
}


}
