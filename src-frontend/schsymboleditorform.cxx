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

#include <QAction>
#include <QToolBar>
#include <QFileDialog>

#include <fstream>
#include <dsim/error.h>

#include "domdataset.h"
#include "schemasheet.h"
#include "mainwindow.h"
#include "schemawidget.h"
#include "schsymboleditorform.h"

namespace dsim
{

SchSymbolEditorForm::SchSymbolEditorForm( QWidget *parent_ )
                : QMainWindow( parent_ )
                , dom( new DomDataset( DOM_SCHEMA_SYMBOL ) )
                , schsheet( new SchemaSheet() )
{
  createActions();
  createToolbars();
  createWidgets();
}

SchSymbolEditorForm::~SchSymbolEditorForm()
{
  delete schsheet;
  delete dom;
}

void SchSymbolEditorForm::createActions()
{
  /*
   * Mode actions
   */
  modeSelection = new QAction(QIcon((":/bitmaps/modeselection.png")), tr("Selection mode"), this);
  modeSelection->setStatusTip(tr("Enter the selection drawing mode."));
  modeDrawPin = new QAction(QIcon((":/bitmaps/modepin.png")), tr("Drawing a component pin"), this);
  modeDrawPin->setStatusTip(tr("Drawing a component pin..."));
  modeDrawLine = new QAction(QIcon((":/bitmaps/modeline.png")), tr("Drawing a line"), this);
  modeDrawLine->setStatusTip(tr("Drawing a line..."));
  modeDrawRect = new QAction(QIcon((":/bitmaps/moderect.png")), tr("Drawing a rectangle"), this);
  modeDrawRect->setStatusTip(tr("Drawing a rectangle..."));
  modeDrawRound = new QAction(QIcon((":/bitmaps/moderound.png")), tr("Drawing a round"), this);
  modeDrawRound->setStatusTip(tr("Drawing a round..."));
  modeDrawText = new QAction(QIcon((":/bitmaps/modetext.png")), tr("Drawing a text"), this);
  modeDrawText->setStatusTip(tr("Drawing a text..."));
  modeDrawScript = new QAction(QIcon((":/bitmaps/modescript.png")), tr("Drawing a script block"), this);
  modeDrawScript->setStatusTip(tr("Drawing a script block..."));

  connect( modeSelection, SIGNAL(triggered()), this, SLOT(onModeSelection()) );

  connect( modeDrawPin, SIGNAL(triggered()), this, SLOT(onModePin()) );

  connect( modeDrawLine, SIGNAL(triggered()), this, SLOT(onModeLine()) );

  connect( modeDrawRect, SIGNAL(triggered()), this, SLOT(onModeRect()) );

  connect( modeDrawRound, SIGNAL(triggered()), this, SLOT(onModeRound()) );

  connect( modeDrawText, SIGNAL(triggered()), this, SLOT(onModeText()) );

  connect( modeDrawScript, SIGNAL(triggered()), this, SLOT(onModeScript()) );
}

void SchSymbolEditorForm::createToolbars()
{
  /*
   * ToolBar drawing mode.
   */
  modeToolBar = new QToolBar( tr("Mode") );
  modeToolBar->addAction( modeSelection );
  modeToolBar->addAction( modeDrawPin );
  modeToolBar->addAction( modeDrawRect );
  modeToolBar->addAction( modeDrawRound );
  modeToolBar->addAction( modeDrawText );
  modeToolBar->addAction( modeDrawScript );
  modeToolBar->addSeparator();
  modeToolBar->setIconSize( QSize(16, 16) );
  this->addToolBar( Qt::LeftToolBarArea, modeToolBar );
}

void SchSymbolEditorForm::createWidgets()
{
  /*
   * Schema Editor View
   */
  schema = new SchemaWidget( this, schsheet, dom );
  this->setCentralWidget( schema );
  dom->addItem( schema->view() );
}

void SchSymbolEditorForm::onFileSave()
{
  using namespace std;

  QString filename = QFileDialog::getSaveFileName( this, tr("Save schema..."), QString(), tr("Schema Symbol file(*.ssym)") );
  if( filename.length() )
    {
      std::string fn = filename.toStdString();
      MainWindow::instance()->processRc( saveSymbolFile( fn.c_str() ) );
    }
}

void SchSymbolEditorForm::onModeSelection()
{ schema->setMode( MODE_SELECTION ); }
void SchSymbolEditorForm::onModePin()
{ schema->setMode( MODE_PIN ); }
void SchSymbolEditorForm::onModeLine()
{ schema->setMode( MODE_LINE ); }
void SchSymbolEditorForm::onModeRect()
{ schema->setMode( MODE_RECT ); }
void SchSymbolEditorForm::onModeRound()
{ schema->setMode( MODE_ROUND ); }
void SchSymbolEditorForm::onModeText()
{ schema->setMode( MODE_TEXT ); }
void SchSymbolEditorForm::onModeScript()
{ schema->setMode( MODE_SCRIPT ); }

int SchSymbolEditorForm::openSymbolFile( const char *filename )
{ using namespace std;

  int rc = dom->init();                 UPDATE_RC(rc);

  ifstream instream( filename );

  if( instream.bad() ) return -DS_OPEN_FILE;

  rc = dom->deserialize( instream );    UPDATE_RC(rc);

  dom->uninit();

  return rc;
}

int SchSymbolEditorForm::saveSymbolFile( const char *filename )
{ using namespace std;

  int rc = dom->init();                 UPDATE_RC(rc);

  ofstream outstream( filename );

  if( outstream.bad() ) return -DS_CREATE_FILE;

  rc = dom->serialize( outstream );     UPDATE_RC(rc);

  dom->uninit();

  return 0;
}


} // namespace dsim
