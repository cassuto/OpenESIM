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

namespace dsim
{

class SchemaGraph;

class SchemaView : public QGraphicsView
{
  Q_OBJECT

public:
  SchemaView( QWidget *parent );
  ~SchemaView();

  void clear();

  qreal scaleFactor() { return m_scalefactor; }

public slots:

private:
  void wheelEvent(QWheelEvent *event);


  void keyPressEvent( QKeyEvent *event );
  void keyReleaseEvent( QKeyEvent *event );

  void resizeEvent(QResizeEvent *event);
  void scaleView(qreal scaleFactor);

  qreal         m_scalefactor;
  QString       m_file;
  SchemaGraph  *m_schemaGraph;
};

}

#endif
