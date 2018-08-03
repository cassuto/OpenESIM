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

#ifndef SCHEMAVIEW_H_
#define SCHEMAVIEW_H_

#include <QtGui>
#include <QGraphicsView>
#include <fstream>

#include "lispdataset.h"
#include "domitem.h"
#include "idallocator.h"

namespace dsim
{

enum DrawMode
{
  MODE_SELECTION = 0,
  MODE_MOVING,
  MODE_WIRE,
  MODE_PIN,
  MODE_LINE,
  MODE_TEXT,
  MODE_RECT,
  MODE_ELLIPSE,
  MODE_SCRIPT,
  MODE_COMPONENT
};

class SchemaScene;
class SchemaSheet;
class ElementBase;
class ElementAbstractPort;
class ComponentGraphItem;
class SchemaView;

typedef bool (SchemaView::*pfnMouseEvent)( QMouseEvent* );
typedef bool (SchemaView::*pfnKeyEvent)( QKeyEvent *event );
typedef void (SchemaView::*pfnResetEvent)();

class SchemaView : public QGraphicsView, public DomItem
{
  Q_OBJECT

public:
  SchemaView( SchemaSheet *sheet, QWidget *parent );
  ~SchemaView();

  void  clear();
  void  gotoCenter();
  qreal scaleFactor() { return m_scalefactor; }
  void  setPaintGrid( bool paint );
  void  setPaintFrameAxes( bool paint );
  void  setMode( DrawMode mode );
  ElementBase *createElement( const char *classname, const QPointF &pos, bool editable = true, bool deser = false );
  void  releaseElement( ElementBase *element );
  void  releaseAllElements();
  int   loadSymbol( ComponentGraphItem *component, const char *filename );
  ElementBase *element( int id );
  void  addId( int id );
  bool  idUnused( int id );
  inline QList<ElementBase *> & elements() { return m_elements[stack]; }

  int   serialize( LispDataset *dom );
  int   deserialize( LispDataset *dom );

public:
  inline SchemaSheet *sheet() const { return m_sheet; }
  inline DrawMode mode() const { return m_mode; }

protected:
  void dragMoveEvent( QDragMoveEvent *event );
  void dragEnterEvent( QDragEnterEvent *event );
  void dragLeaveEvent( QDragLeaveEvent *event );
  void dropEvent( QDropEvent *event );

  void mousePressEvent( QMouseEvent *event );
  void mouseMoveEvent( QMouseEvent *event );
  void mouseReleaseEvent( QMouseEvent *event );
  void wheelEvent( QWheelEvent *event );

  void keyPressEvent( QKeyEvent *event );
  void keyReleaseEvent( QKeyEvent *event );

  void resizeEvent( QResizeEvent *event );

private:
  void scaleView(qreal scaleFactor);
  void clearStack( int i );

  bool          m_paintGrid;
  bool          m_paintFrameAxes;
  qreal         m_scalefactor;
  QString       m_file;
  SchemaScene  *m_schemaGraph;
  SchemaSheet  *m_sheet;
  int           stack;
  QList<ElementBase *>m_elements[2];
  IdAllocator   m_id[2];
  bool          m_editable[2];
  ElementBase  *m_selectedElements;

private: // schemaviewmenus.cxx
  int           m_currentDirection;
  QAction      *m_actionEditProperties;
  QAction      *m_actionRotateLeft;
  QAction      *m_actionRotateRight;
  QAction      *m_actionXmirror;
  QAction      *m_actionYmirror;
  void createContextMenus();

private slots:
  void onEditProperties( bool checked = false );
  void onRemoveSelected( bool checked = false );
  void onRotateLeft( bool checked = false );
  void onRotateRight( bool checked = false );
  void onXmirror( bool checked = false );
  void onYmirror( bool checked = false );

protected:
  void contextMenuEvent( QContextMenuEvent* event );
  void mouseDoubleClickEvent( QMouseEvent *event );

private: // schemaviewactions.cxx
  bool mousePressSelect( QMouseEvent *event );
  bool mouseMoveSelect( QMouseEvent *event );
  bool mouseReleaseSelect( QMouseEvent *event );
  bool mousePressWire( QMouseEvent *event );
  bool mouseMoveWire( QMouseEvent *event );
  bool mousePressWire2( QMouseEvent *event );
  bool mouseMoveWire2( QMouseEvent *event );
  bool connectEndWire( ElementAbstractPort *port );
  bool mousePressComponent( QMouseEvent *event );
  bool mouseMoveComponent( QMouseEvent *event );
  bool mousePressPin( QMouseEvent *event );
  bool mouseMovePin( QMouseEvent *event );
  bool mousePressLine( QMouseEvent *event );
  bool mouseMoveLine( QMouseEvent *event );
  bool mousePressRect( QMouseEvent *event );
  bool mousePressText( QMouseEvent *event );
  bool mouseMoveText( QMouseEvent *event );
  bool mouseMoveRect( QMouseEvent *event );
  bool mousePressEllipse( QMouseEvent *event );
  bool mouseMoveEllipse( QMouseEvent *event );
  bool keyPressComponent( QKeyEvent *event );
  bool keyPressPin( QKeyEvent *event );
  bool keyPressText( QKeyEvent *event );
  void resetLine();

  void switchEventHandlers( pfnMouseEvent mousePressEvent,
                            pfnMouseEvent mouseMoveEvent,
                            pfnMouseEvent mouseReleaseEvent,
                            pfnKeyEvent   keyPressEvent,
                            pfnResetEvent resetEvent );

  template <typename T> bool keyPressRotate( QKeyEvent *event, T *element );

  DrawMode      m_mode;
  ElementBase  *m_hintElement;
  ComponentGraphItem *m_hintComponent;
  int           m_hintDirect;
  int           m_hintCount;
  bool          m_moving;
  pfnMouseEvent m_mousePressEvent;
  pfnMouseEvent m_mouseMoveEvent;
  pfnMouseEvent m_mouseReleaseEvent;
  pfnKeyEvent   m_keyPressEvent;
  pfnResetEvent m_resetEvent;

  friend class WireSegment;
};

}

#include "schemaviewfunctions.hxx"

#endif
