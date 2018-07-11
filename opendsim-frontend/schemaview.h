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
  MODE_PIN,
  MODE_LINE,
  MODE_TEXT,
  MODE_RECT,
  MODE_ROUND,
  MODE_SCRIPT,
  MODE_COMPONENT
};

class SchemaGraph;
class SchemaSheet;
class ElementBase;
class ComponentGraphItem;
class SchemaView;

typedef bool (SchemaView::*pfnMouseEvent)( QMouseEvent* );
typedef bool (SchemaView::*pfnkeyEvent)( QKeyEvent *event );

class SchemaView : public QGraphicsView, public DomItem
{
  Q_OBJECT

public:
  SchemaView( SchemaSheet *sheet, QWidget *parent );
  ~SchemaView();

  void clear();

  qreal scaleFactor() { return m_scalefactor; }

  void setMode( DrawMode mode );

  ElementBase *createElement( const char *classname, const QPoint &pos, bool editable = true, bool deser = false );
  void removeElement( ElementBase *element );

  int serialize( LispDataset *dom );
  int deserialize( LispDataset *dom );

  ComponentGraphItem *loadSymbol( std::ifstream &instream );

  ElementBase * element( int id );
  void addId( int id );
  bool idUnused( int id );

public:
  inline SchemaSheet *sheet() const { return m_sheet; }

public slots:

protected:
  void dragMoveEvent( QDragMoveEvent *event );
  void dragEnterEvent( QDragEnterEvent *event );
  void dragLeaveEvent( QDragLeaveEvent *event );
  void dropEvent( QGraphicsSceneDragDropEvent *event );

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

  qreal         m_scalefactor;
  QString       m_file;
  SchemaGraph  *m_schemaGraph;
  SchemaSheet  *m_sheet;
  int           stack;
  QList<ElementBase *>m_elements[2];
  IdAllocator   m_id[2];
  bool          m_editable[2];

private: // schemaviewactions.cxx
  bool mousePressSelect( QMouseEvent *event );
  bool mouseMoveSelect( QMouseEvent *event );
  bool mouseReleaseSelect( QMouseEvent *event );
  bool mousePressComponent( QMouseEvent *event );
  bool mouseMoveComponent( QMouseEvent *event );
  bool mousePressLine( QMouseEvent *event );
  bool mouseMoveLine( QMouseEvent *event );
  bool mouseReleaseLine( QMouseEvent *event );
  bool mousePressRect( QMouseEvent *event );
  bool mouseMoveRect( QMouseEvent *event );
  bool mouseReleaseRect( QMouseEvent *event );
  bool mousePressPin( QMouseEvent *event );
  bool mouseMovePin( QMouseEvent *event );
  bool keyPressComponent( QKeyEvent *event );
  bool keyPressPin( QKeyEvent *event );

  DrawMode      m_mode;
  ElementBase  *m_hintElement;
  ComponentGraphItem *m_hintComponent;
  int           m_hintDirect;
  bool          m_moving;
  pfnMouseEvent m_mousePressEvent;
  pfnMouseEvent m_mouseMoveEvent;
  pfnMouseEvent m_mouseReleaseEvent;
  pfnkeyEvent   m_keyPressEvent;
};

}

#endif
