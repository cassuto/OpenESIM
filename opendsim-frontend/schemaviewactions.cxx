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

#define TRACE_UNIT "schemaviewactions"
#include <cstring>
#include <cstdio>
#include <dsim/logtrace.h>

#include "schemascene.h"
#include "elementgraphitem.h"
#include "componentgraphitem.h"
#include "elementwire.h"
#include "elementline.h"
#include "elementtext.h"
#include "elementpin.h"
#include "elementrect.h"
#include "elementellipse.h"
#include "elementjoint.h"

#include "mainwindow.h"
#include "schemaview.h"

namespace dsim
{

DECLARE_ELEMENT_CAST(ComponentGraphItem, "component")
DECLARE_ELEMENT_CAST(ElementWire, "wire")

void SchemaView::switchEventHandlers( pfnMouseEvent mousePressEvent,
                                      pfnMouseEvent mouseMoveEvent,
                                      pfnMouseEvent mouseReleaseEvent,
                                      pfnKeyEvent   keyPressEvent,
                                      pfnResetEvent resetEvent )
{
  m_mousePressEvent     = mousePressEvent;
  m_mouseMoveEvent      = mouseMoveEvent;
  m_mouseReleaseEvent   = mouseReleaseEvent;
  m_keyPressEvent       = keyPressEvent;
  m_resetEvent          = resetEvent;
}

void SchemaView::setMode( DrawMode mode )
{
  m_mode = mode;

  switch( mode )
  {
    case MODE_SELECTION:
      this->setCursor( QCursor( Qt::ArrowCursor ));
      break;
    case MODE_MOVING:
      break;
    case MODE_WIRE:
      this->setCursor( QCursor( QPixmap(":/bitmaps/greenpen.cur")) );
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
          deleteElement( m_hintElement );
          m_hintElement = 0l;
        }
      m_moving = false;

      switchEventHandlers( &SchemaView::mousePressSelect,
                           &SchemaView::mouseMoveSelect,
                           &SchemaView::mouseReleaseSelect,
                           0l,
                           0l );
      break;

    case MODE_MOVING:
      switchEventHandlers( 0l,
                           0l,
                           0l,
                           0l,
                           0l );
      break;

    case MODE_WIRE:
      {
        switchEventHandlers ( &SchemaView::mousePressWire,
                              &SchemaView::mouseMoveWire,
                              0l,
                              0l,
                              0l );
        break;
      }

    case MODE_PIN:
      {
        ElementBase *symbol = createElement( "text", QPoint( 0, 0 ) );
        ((ElementText *)symbol)->setText( "PIN 1" );

        ElementBase *reference = createElement( "text", QPoint( 0, 0 ) );
        ((ElementText *)reference)->setText( "1" );

        m_hintElement = createElement( "pin", QPoint( 0, 0 ) );
        ((ElementPin *)m_hintElement)->setSub( (ElementText *)symbol, ((ElementText *)reference) );
        ((ElementPin *)m_hintElement)->setVisible( false );

        switchEventHandlers( &SchemaView::mousePressPin,
                             &SchemaView::mouseMovePin,
                             0l,
                             &SchemaView::keyPressPin,
                             0l );
        break;
      }

    case MODE_LINE:
      {
        switchEventHandlers( &SchemaView::mousePressLine,
                             &SchemaView::mouseMoveLine,
                             0l,
                             0l,
                             &SchemaView::resetLine );
        break;
      }

    case MODE_TEXT:
      {
        m_hintElement = createElement( "text", QPoint( 0, 0 ) );
        ((ElementText *)m_hintElement)->setVisible( false );

        switchEventHandlers( &SchemaView::mousePressText,
                             &SchemaView::mouseMoveText,
                             0l,
                             &SchemaView::keyPressText,
                             0l );
        break;
      }
    case MODE_RECT:
      {
        switchEventHandlers( &SchemaView::mousePressRect,
                             &SchemaView::mouseMoveRect,
                             0l,
                             0l,
                             0l );
        break;
      }
    case MODE_ELLIPSE:
      {
        switchEventHandlers( &SchemaView::mousePressEllipse,
                             &SchemaView::mouseMoveEllipse,
                             0l,
                             0l,
                             0l );
        break;
      }
    case MODE_SCRIPT:
    case MODE_COMPONENT:
      break;
  }
}

static ElementPin *componentPinAt( SchemaView *schematic, const QPointF &pos ) // pos is in view coordinates
{
  QList<QGraphicsItem *> list = schematic->items( pos.toPoint() );
  foreach( QGraphicsItem *current, list )
    {
      ElementBase *element = elementbase_cast( current );
      if( element )
        {
          ComponentGraphItem *component = element_cast<ComponentGraphItem *>( element );
          if( component )
            {
              return component->atPin( schematic->mapToScene( pos.toPoint() ) );
            }
        }
    }
  return 0l;
}

// ------------------------------------------------------------------ //
// Mouse actions for selection mode
// ------------------------------------------------------------------ //

bool SchemaView::mousePressSelect( QMouseEvent *event )
{ UNUSED(event); return true; }

bool SchemaView::mouseMoveSelect( QMouseEvent *event )
{
  if( componentPinAt( this, event->pos() ) )
    {
      setMode( MODE_WIRE ); // switch the mode automatically when the mouse is floating on a pin
      return true;
    }
  return true;
}

bool SchemaView::mouseReleaseSelect( QMouseEvent *event )
{ UNUSED(event); return true; }

// ------------------------------------------------------------------ //
// Mouse actions for component mode
// ------------------------------------------------------------------ //

bool SchemaView::mousePressWire( QMouseEvent *event )
{
  if( event->button() == Qt::LeftButton )
    {
      ElementPin *pin = componentPinAt( this, event->pos() );
      if( pin && 0l == pin->connectedWire() )
        {
          /*
           * Start a connecting wire from the pin
           */
          m_hintElement = createElement( "wire", QPoint( 0, 0 ) );
          ElementWire *wire = static_cast<ElementWire *>(m_hintElement);

          wire->connectStartPort( pin );

          QPointF port = pin->portScenePos();
          wire->addWire( QLineF( port, port) , 0 );

          m_mousePressEvent = &mousePressWire2; // switch callback
          m_mouseMoveEvent = &mouseMoveWire2;
          return true;
        }
    }
  setMode( MODE_SELECTION );
  return true;
}

bool SchemaView::mouseMoveWire( QMouseEvent *event )
{
  if( 0l==componentPinAt( this, event->pos() ) )
    {
      setMode( MODE_SELECTION );
    }
  return true;
}

bool SchemaView::mousePressWire2( QMouseEvent *event )
{
  if( event->button() == Qt::LeftButton )
    {
      /*
       * Tell the target port we will connect to
       */
      ElementAbstractPort *port = 0l;
      ElementWire *currentWire = static_cast<ElementWire *>(m_hintElement);
      ElementPin *pin = componentPinAt( this, event->pos() );
      if( pin && 0l == pin->connectedWire() )
        {
          port = pin; // find a target pin
        }
      else // find if there was a wire where we could put a joint
        {
          QPointF scenePos = mapToScene( event->pos() );

          QList<QGraphicsItem *> list = items( event->pos() );
          foreach( QGraphicsItem *current, list )
            {
              ElementBase *element = elementbase_cast( current );
              if( element )
                {
                  ElementWire *wire = element_cast<ElementWire *>( element );
                  if( wire )
                    {
                      port = wire->addJoint( scenePos );
                    }
                }
            }
        }
      if( port )
        {
          /*
           * Close the connecting wire if conditions are meet
           */
          if( port != currentWire->startPort() )
            {
              currentWire->connectEndPort( port );

              m_hintElement = 0l; // accept this wire
              setMode( MODE_SELECTION );
              return true;
            }
        }
      else
        {
          currentWire->addActiveWire(); // create an active wire for corner
          return true;
        }
    }
  setMode( MODE_SELECTION );
  return true;
}

bool SchemaView::mouseMoveWire2( QMouseEvent *event )
{
  ((ElementWire *)m_hintElement)->layoutWires( 0l, mapToScene( event->pos() ) );
  return true;
}

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
  QPointF cp = togrid( mapToScene(event->pos()) );

  ElementText *symbolText = static_cast<ElementText *>(createElement( "text", cp, /*edit*/true, /*deser*/false ));
  ElementText *referenceText = static_cast<ElementText *>(createElement( "text", cp, /*edit*/true, /*deser*/false ));

  m_hintComponent = static_cast<ComponentGraphItem *>(createElement( "component", cp, /*edit*/true, /*deser*/false ));
  int rc = m_hintComponent->init( symbol.c_str(), symbolText, referenceText );
  if( MainWindow::instance()->processRc( rc ) )
    {
      deleteElement( m_hintComponent );
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

void SchemaView::dropEvent( QDropEvent *event )
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
