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
#include <frontend/error.h>
#include <device/libentry.h>

#include "lispdataset.h"
#include "schemasheet.h"
#include "mainwindow.h"
#include "componentpickwidget.h"
#include "schemawidget.h"
#include "schemaeditorform.h"

namespace dsim
{

SchemaEditorForm::SchemaEditorForm( DomType type, QWidget *parent_ )
                : QMainWindow( parent_ )
                , dom( new LispDataset( type ) )
                , schsheet( new SchemaSheet() )
{
  createActions();
  createToolbars();
  createWidgets();
}

SchemaEditorForm::~SchemaEditorForm()
{
  delete schsheet;
}

void SchemaEditorForm::createActions()
{
  /*
   * Mode actions
   */
  modeSelection = new QAction(QIcon((":/bitmaps/modeselection.png")), tr("Selection mode"), this);
  modeSelection->setStatusTip(tr("Enter the selection drawing mode."));

  if( dom->type() == DOM_SCHEMA )
    {
      modeDrawComponent = new QAction(QIcon((":/bitmaps/modecomponent.png")), tr("Drawing the component mode"), this);
      modeDrawComponent->setStatusTip(tr("Enter the drawing the component mode."));
      modeDrawJunctionDot = new QAction(QIcon((":/bitmaps/modejunctiondot.png")), tr("Drawing the junction dot mode"), this);
      modeDrawJunctionDot->setStatusTip(tr("Enter the drawing the junction dot mode mode."));
      modeDrawLable = new QAction(QIcon((":/bitmaps/modelabel.png")), tr("Drawing the label mode"), this);
      modeDrawLable->setStatusTip(tr("Enter the drawing the label mode."));
      modeDrawBus = new QAction(QIcon((":/bitmaps/modebus.png")), tr("Drawing the bus mode"), this);
      modeDrawBus->setStatusTip(tr("Enter the drawing the bus mode mode."));
      modeDrawSubCircuit = new QAction(QIcon((":/bitmaps/modesubcircuit.png")), tr("Drawing the sub circuit mode"), this);
      modeDrawSubCircuit->setStatusTip(tr("Enter the drawing the sub circuit mode mode."));
    }
  else if( dom->type() == DOM_SCHEMA_SYMBOL )
    {
      modeDrawPin = new QAction(QIcon((":/bitmaps/modepin.png")), tr("Drawing a component pin"), this);
      modeDrawPin->setStatusTip(tr("Drawing a component pin..."));

      modeDrawOrigin = new QAction(QIcon((":/bitmaps/modeorigin.png")), tr("Drawing a origin"), this);
      modeDrawOrigin->setStatusTip(tr("Drawing a origin..."));

      connect( modeDrawPin, SIGNAL(triggered()), this, SLOT(onModePin()) );

      connect( modeDrawOrigin, SIGNAL(triggered()), this, SLOT(onModeOrigin()) );
    }

  modeDrawScript = new QAction(QIcon((":/bitmaps/modescript.png")), tr("Drawing the script mode"), this);
  modeDrawScript->setStatusTip(tr("Enter the drawing the script mode mode."));
  modeDrawLine = new QAction(QIcon((":/bitmaps/modeline.png")), tr("Drawing a line"), this);
  modeDrawLine->setStatusTip(tr("Drawing a line..."));
  modeDrawRect = new QAction(QIcon((":/bitmaps/moderect.png")), tr("Drawing a rectangle"), this);
  modeDrawRect->setStatusTip(tr("Drawing a rectangle..."));
  modeDrawEllipse = new QAction(QIcon((":/bitmaps/modeellipse.png")), tr("Drawing a ellipse"), this);
  modeDrawEllipse->setStatusTip(tr("Drawing a ellipse..."));
  modeDrawText = new QAction(QIcon((":/bitmaps/modetext.png")), tr("Drawing a text"), this);
  modeDrawText->setStatusTip(tr("Drawing a text..."));

  connect( modeSelection, SIGNAL(triggered()), this, SLOT(onModeSelection()) );

  connect( modeDrawLine, SIGNAL(triggered()), this, SLOT(onModeLine()) );

  connect( modeDrawRect, SIGNAL(triggered()), this, SLOT(onModeRect()) );

  connect( modeDrawEllipse, SIGNAL(triggered()), this, SLOT(onModeEllipse()) );

  connect( modeDrawText, SIGNAL(triggered()), this, SLOT(onModeText()) );

  connect( modeDrawScript, SIGNAL(triggered()), this, SLOT(onModeScript()) );

}

void SchemaEditorForm::createToolbars()
{
  /*
   * ToolBar drawing mode.
   */
  modeToolBar = new QToolBar( tr("Mode") );
  modeToolBar->addAction( modeSelection );

  if( dom->type() == DOM_SCHEMA )
    {
      modeToolBar->addAction( modeDrawComponent );
      modeToolBar->addAction( modeDrawJunctionDot );
      modeToolBar->addAction( modeDrawLable );
      modeToolBar->addAction( modeDrawScript );
      modeToolBar->addAction( modeDrawBus );
      modeToolBar->addAction( modeDrawSubCircuit );
      modeToolBar->addSeparator();
    }
  else if( dom->type() == DOM_SCHEMA_SYMBOL )
    {
      modeToolBar->addAction( modeDrawPin );
    }
  modeToolBar->addAction( modeDrawLine );
  modeToolBar->addAction( modeDrawRect );
  modeToolBar->addAction( modeDrawEllipse );
  modeToolBar->addAction( modeDrawText );
  modeToolBar->addAction( modeDrawScript );
  if( dom->type() == DOM_SCHEMA_SYMBOL )
    {
      modeToolBar->addAction( modeDrawOrigin );
    }

  modeToolBar->setIconSize( QSize(16, 16) );
  this->addToolBar( Qt::LeftToolBarArea, modeToolBar );
}

void SchemaEditorForm::createWidgets()
{
  /*
   * Device Pick Dock
   */
  if( dom->type() == DOM_SCHEMA )
    {
      componentPick = new ComponentPickWidget( this );
      componentPick->setAllowedAreas( Qt::AllDockWidgetAreas );
      this->addDockWidget( Qt::RightDockWidgetArea, componentPick );

      dom->addItem( componentPick );
    }

  /*
   * Schema Editor View
   */
  schema = new SchemaWidget( this, schsheet, dom );
  this->setCentralWidget( schema );

  dom->addItem( schema->view() );
}

void SchemaEditorForm::onModeSelection()
{ schema->setMode( MODE_SELECTION ); }
void SchemaEditorForm::onModePin()
{ schema->setMode( MODE_PIN ); }
void SchemaEditorForm::onModeLine()
{ schema->setMode( MODE_LINE ); }
void SchemaEditorForm::onModeRect()
{ schema->setMode( MODE_RECT ); }
void SchemaEditorForm::onModeEllipse()
{ schema->setMode( MODE_ELLIPSE ); }
void SchemaEditorForm::onModeText()
{ schema->setMode( MODE_TEXT ); }
void SchemaEditorForm::onModeScript()
{ schema->setMode( MODE_SCRIPT ); }
void SchemaEditorForm::onModeOrigin()
{ schema->setMode( MODE_ORIGIN ); }

void SchemaEditorForm::gotoCenter()
{ schema->view()->gotoCenter(); }

int SchemaEditorForm::init()
{ return schsheet->init(); }

int SchemaEditorForm::open( const char *filename )
{ using namespace std;

  int rc = dom->init();                 UPDATE_RC(rc);

  ifstream instream( filename );

  if( !instream.is_open() ) return -DS_OPEN_FILE;

  rc = dom->deserialize( instream );    UPDATE_RC(rc);

  dom->uninit();

  return rc;
}

int SchemaEditorForm::save( const char *filename )
{ using namespace std;

  int rc = dom->init();                 UPDATE_RC(rc);

  ofstream outstream( filename );

  if( !outstream.is_open() ) return -DS_CREATE_FILE;

  rc = dom->serialize( outstream );     UPDATE_RC(rc);

  dom->uninit();

  return 0;
}

int SchemaEditorForm::compileNetlist()
{ return schsheet->generateNetlist(); }

int SchemaEditorForm::debugStep()
{
  int rc;
  if( !(rc = schsheet->compile()) )
    {
      return schsheet->runStep();
    }
  return rc;
}

int SchemaEditorForm::debugRun()
{
  int rc;
  if( !(rc = schsheet->compile()) )
    {
      return schsheet->run();
    }
  return rc;
}

void SchemaEditorForm::debugEnd()
{ schsheet->end(); }

} // namespace dsim

#include "schemaeditorform.moc"
