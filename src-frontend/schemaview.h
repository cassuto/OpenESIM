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

#include "domdataset.h"
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
class ComponentGraphItem;
class SchemaSheet;
class ElementGraphItem;

class SchemaView : public QGraphicsView, public DomItem
{
  Q_OBJECT

public:
  SchemaView( SchemaSheet *sheet, QWidget *parent );
  ~SchemaView();

  void clear();

  qreal scaleFactor() { return m_scalefactor; }

  void setMode( DrawMode mode );

  ElementGraphItem *createElement( const char *classname, const QPoint &pos, bool editable = true, bool deser = false );
  void removeElement( ElementGraphItem *element );

  int serialize( DomDataset *dom );
  int deserialize( DomDataset *dom );

  ComponentGraphItem *loadSymbol( std::ifstream &instream );

  ElementGraphItem * element( int id );
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

  void wheelEvent( QWheelEvent *event );

  void keyPressEvent( QKeyEvent *event );
  void keyReleaseEvent( QKeyEvent *event );

  void resizeEvent( QResizeEvent *event );

private:
  void scaleView(qreal scaleFactor);
  bool hintSize( const QPointF &cpos );
  void clearStack( int i );

  qreal         m_scalefactor;
  QString       m_file;
  SchemaGraph  *m_schemaGraph;
  ElementGraphItem *m_hintElement;
  ComponentGraphItem *m_hintComponent;
  int           m_hintSize;
  QPointF       m_hintOrigPos;
  int           m_hintDirect;
  SchemaSheet  *m_sheet;
  DrawMode      m_mode;
  int           stack;
  QList<ElementGraphItem *>m_elements[2];
  IdAllocator   m_id[2];
  bool          m_editable[2];
};

}

#endif
