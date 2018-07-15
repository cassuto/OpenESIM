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
#include "elementellipse.h"

#include "mainwindow.h"
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
    case MODE_ELLIPSE:
    case MODE_SCRIPT:
    case MODE_COMPONENT:
      this->setCursor( QCursor( Qt::CrossCursor ));
      break;
  }

  switch( mode )
  {
    case MODE_SELECTION:
      if( m_resetEvent ) (this->*m_resetEvent)();
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
      m_resetEvent = 0l;
      break;

    case MODE_PIN:
      {
        ElementBase *symbol = createElement( "text", QPoint( 0, 0 ) );
        ((ElementText *)symbol)->setText( "PIN 1" );

        ElementBase *reference = createElement( "text", QPoint( 0, 0 ) );
        ((ElementText *)reference)->setText( "1000" );

        m_hintElement = createElement( "pin", QPoint( 0, 0 ) );
        ((ElementPin *)m_hintElement)->setSub( (ElementText *)symbol, ((ElementText *)reference) );
        ((ElementPin *)m_hintElement)->setVisible( false );

        m_mousePressEvent = &mousePressPin;
        m_mouseMoveEvent = &mouseMovePin;
        m_mouseReleaseEvent = 0l;
        m_keyPressEvent = &keyPressPin;
        m_resetEvent = 0l;
        break;
      }

    case MODE_LINE:
      {
        m_mousePressEvent = &mousePressLine;
        m_mouseMoveEvent = &mouseMoveLine;
        m_mouseReleaseEvent = 0l;
        m_keyPressEvent = 0l;
        m_resetEvent = &resetLine;
        break;
      }
      break;
    case MODE_TEXT:
      {
        m_hintElement = createElement( "text", QPoint( 0, 0 ) );
        ((ElementText *)m_hintElement)->setVisible( false );

        m_mousePressEvent = &mousePressText;
        m_mouseMoveEvent = &mouseMoveText;
        m_mouseReleaseEvent = 0l;
        m_keyPressEvent = &keyPressText;
        m_resetEvent = 0l;
        break;
      }
    case MODE_RECT:
      {
        m_mousePressEvent = &mousePressRect;
        m_mouseMoveEvent = &mouseMoveRect;
        m_mouseReleaseEvent = 0l;
        m_keyPressEvent = 0l;
        m_resetEvent = 0l;
        break;
      }
    case MODE_ELLIPSE:
      {
        m_mousePressEvent = &mousePressEllipse;
        m_mouseMoveEvent = &mouseMoveEllipse;
        m_mouseReleaseEvent = 0l;
        m_keyPressEvent = 0l;
        m_resetEvent = 0l;
        break;
      }
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
  if( event->button() == Qt::LeftButton )
    {
      m_hintComponent->setLayout();
      m_hintComponent = 0l;
    }
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
  if( event->button() == Qt::LeftButton )
    {
      m_hintElement->setLayout();
      m_hintElement = 0l;
    }
  setMode( MODE_SELECTION );
  return true;
}

bool SchemaView::mouseMovePin( QMouseEvent *event )
{
  m_hintElement->graphicsItem()->setPos( togrid( mapToScene( event->pos() ) ) );
  m_hintElement->setLayout();
  ((ElementText *)m_hintElement)->setVisible( true );
  return true;
}

// ------------------------------------------------------------------ //
// Mouse actions for text mode
// ------------------------------------------------------------------ //

bool SchemaView::mousePressText( QMouseEvent *event )
{
  if( event->button() == Qt::LeftButton )
    {
      m_hintElement = 0l; // accept this text
    }
  setMode( MODE_SELECTION );
  return true;
}

bool SchemaView::mouseMoveText( QMouseEvent *event )
{
  m_hintElement->graphicsItem()->setPos( togrid( mapToScene( event->pos() ) ) );
  m_hintElement->graphicsItem()->setVisible( true );
  return true;
}

// ------------------------------------------------------------------ //
// Mouse actions for line mode
// ------------------------------------------------------------------ //

bool SchemaView::mousePressLine( QMouseEvent *event )
{
  if( event->button() == Qt::LeftButton )
    {
      QPointF cp = togrid( mapToScene( event->pos() ) );


      if( !m_hintElement ) // set the origin
        {
          m_hintElement = createElement( "line", cp );
        }
      else
        {
          ((ElementLine *)m_hintElement)->addPoint( cp );
        }

      ElementLine *lineElement = static_cast<ElementLine *>(m_hintElement);

      m_hintCount = lineElement->pointCount();

      m_moving = true;
      event->accept();
      return false;
    }
  else if( event->button() == Qt::RightButton )
    {
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

      if( line->pointCount() == m_hintCount )
        line->addPoint( togrid( mapToScene( event->pos() ) ) );
      else
      line->setLastPoint( togrid( mapToScene( event->pos() ) ) );

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
      if( 0l == m_hintElement )
        {
          m_hintElement = createElement( "rect", QPoint( 0, 0 ) );
          ElementRect *rectElement = static_cast<ElementRect *>(m_hintElement);
          rectElement->setVisible( false );

          QRectF rect = rectElement->rect();
          rect.setTopLeft( togrid( mapToScene( event->pos() ) ) );
          rectElement->setRect( rect );

          m_moving = true;
          event->accept();
          return false;
        }
      else if( m_moving )
        {
          ElementRect *rectElement = static_cast<ElementRect *>(m_hintElement);
          QRectF rect = rectElement->rect();

          if( rect.topLeft() != rect.bottomRight() ) // accept this rectangle
            {
              m_hintElement->graphicsItem()->setVisible( true );
              m_hintElement = 0l;
            }

          m_moving = false;

          setMode( MODE_SELECTION );
          event->accept();
          return false;
        }
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
      rectElement->setVisible( true );

      event->accept();
      return false;
    }
  return true;
}

// ------------------------------------------------------------------ //
// Mouse actions for ellipse mode
// ------------------------------------------------------------------ //

bool SchemaView::mousePressEllipse( QMouseEvent *event )
{
  if( event->button() == Qt::LeftButton )
    {
      if( 0l == m_hintElement )
        {
          m_hintElement = createElement( "ellipse", QPoint( 0, 0 ) );
          ElementEllipse *ellipseElement = static_cast<ElementEllipse *>(m_hintElement);
          ellipseElement->setVisible( false );

          QRectF rect = ellipseElement->rect();
          rect.setTopLeft( togrid( mapToScene( event->pos() ) ) );
          ellipseElement->setRect( rect );

          m_moving = true;
          event->accept();
          return false;
        }
      else if( m_moving )
        {
          ElementEllipse *ellipseElement = static_cast<ElementEllipse *>(m_hintElement);
          QRectF rect = ellipseElement->rect();

          if( rect.topLeft() != rect.bottomRight() ) // accept this ellipse
            {
              ellipseElement->setVisible( true );
              ellipseElement->setRect( rect );
              m_hintElement = 0l;
            }

          m_moving = false;

          setMode( MODE_SELECTION );
          event->accept();
          return false;
        }
    }
  else if( event->button() == Qt::RightButton )
    {
      setMode( MODE_SELECTION ); // give up drawing

      event->accept();
      return false;
    }
  return true;
}

bool SchemaView::mouseMoveEllipse( QMouseEvent *event )
{
  if( m_moving )
    {
      ElementEllipse *ellipseElement = static_cast<ElementEllipse *>(m_hintElement);
      QRectF rect = ellipseElement->rect();
      rect.setBottomRight( togrid( mapToScene( event->pos() ) ) );
      ellipseElement->setRect( rect );
      ellipseElement->setVisible( true );

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
  QPointF cp = mapToScene( event->pos() );

  ElementText *symbolText = static_cast<ElementText *>(createElement( "text", cp, /*edit*/true, /*deser*/false ));
  ElementText *referenceText = static_cast<ElementText *>(createElement( "text", cp, /*edit*/true, /*deser*/false ));

  m_hintComponent = static_cast<ComponentGraphItem *>(createElement( "component", cp, /*edit*/true, /*deser*/false ));
  int rc = m_hintComponent->init( symbol.c_str(), symbolText, referenceText );
  if( MainWindow::instance()->processRc( rc ) )
    {
      removeElement( m_hintComponent );
      return;
    }

  m_hintComponent->setPos( cp );
  m_hintComponent->setLayout();

  m_mousePressEvent = 0l;
  m_mouseMoveEvent = 0l;
  m_mouseReleaseEvent = 0l;
  m_keyPressEvent = &keyPressComponent;
}

void SchemaView::dragMoveEvent( QDragMoveEvent *event )
{
  event->accept();

  if( m_hintComponent )
    {
      m_hintComponent->setPos( togrid( mapToScene( event->pos() ) ) );
      m_hintComponent->setLayout();
    }
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
  return keyPressRotate( event, m_hintComponent );
}

// ------------------------------------------------------------------ //
// Keyboard actions for pin mode
// ------------------------------------------------------------------ //

bool SchemaView::keyPressPin( QKeyEvent *event )
{
  return keyPressRotate( event, static_cast<ElementPin *>(m_hintElement) );
}

// ------------------------------------------------------------------ //
// Keyboard actions for text mode
// ------------------------------------------------------------------ //

bool SchemaView::keyPressText( QKeyEvent *event )
{
  return keyPressRotate( event, static_cast<ElementText *>(m_hintElement) );
}

// ------------------------------------------------------------------ //
// Reset actions for line mode
// ------------------------------------------------------------------ //

void SchemaView::resetLine()
{
  ElementLine *lineElement = static_cast<ElementLine *>(m_hintElement);

  if( lineElement )
    {
      if( lineElement->pointCount() != m_hintCount ) lineElement->removeLastPoint();

      if( lineElement->pointCount() > 0 )
        {
          m_hintElement = 0l; // accept this line
        }
    }
}

}
