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

#ifndef SCHSYMBOLEDITORFORM_H_
#define SCHSYMBOLEDITORFORM_H_

#include <fstream>
#include <QMainWindow>

#include <device/libentry.h>

class QAction;
class QToolBar;

namespace dsim
{

class SchemaWidget;
class DomDataset;
class SchemaSheet;

/***************************************************
  *****  Schema editor MDI sub window object   *****
  ***************************************************/

class SchSymbolEditorForm : public QMainWindow/*QMdiSubWindow*/
{
  Q_OBJECT

public:
  SchSymbolEditorForm( QWidget *parent );
  ~SchSymbolEditorForm();

  int openSymbolFile( std::ifstream & stream );

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
          *modeDrawPin,
          *modeDrawLine,
          *modeDrawRect,
          *modeDrawRound,
          *modeDrawText,
          *modeDrawScript;

  QToolBar *modeToolBar;

  SchemaWidget *schema;

  DomDataset *dom;
  SchemaSheet *schsheet;
};

} // namespace dsim

#endif //!defined(SCHSYMBOLEDITORFORM_H_)
