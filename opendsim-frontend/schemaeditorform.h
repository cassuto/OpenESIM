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

#ifndef SCHEMAEDITORFORM_H_
#define SCHEMAEDITORFORM_H_

#include <fstream>
#include <QMainWindow>

#include "lispdataset.h"

class QAction;
class QToolBar;

namespace dsim
{

class ComponentPickWidget;
class SchemaWidget;
class SchemaSheet;

/***************************************************
  *****  Schema editor MDI sub window object   *****
  ***************************************************/

class SchemaEditorForm : public QMainWindow/*QMdiSubWindow*/ {
  Q_OBJECT

public:
  SchemaEditorForm( DomType type, QWidget *parent );
  ~SchemaEditorForm();

  int open( const char *filename );
  int save( const char *filename );

private:
  void createActions();
  void createToolbars();
  void createWidgets();

private slots:
  void onFileSave();
  void onModeSelection();
  void onModePin();
  void onModeLine();
  void onModeRect();
  void onModeRound();
  void onModeText();
  void onModeScript();

private:
  QAction *modeSelection,
          *modeDrawComponent,
          *modeDrawJunctionDot,
          *modeDrawLable,
          *modeDrawScript,
          *modeDrawBus,
          *modeDrawSubCircuit;

  QAction *modeDrawPin,
          *modeDrawLine,
          *modeDrawRect,
          *modeDrawRound,
          *modeDrawText;

  QToolBar *modeToolBar;
  ComponentPickWidget *componentPick;

  SchemaWidget *schema;

  LispDataset *dom;

  SchemaSheet *schsheet;
};

} // namespace dsim

#endif //!defined(SCHEMAEDITORFORM_H_)
