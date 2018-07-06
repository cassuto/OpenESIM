/** @file
 * OpenDSIM - MainWindow
 */

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

#define TRACE_UNIT "mainwnd"
#include <dsim/logtrace.h>

#include <fstream>
#include <sstream>
#include <string>

#include <QObject>
#include <QString>
#include <QtGui>
#include <QApplication>
#include <QtWidgets>
#include <dsim/error.h>
#include <dsim/version.h>

#include "domdataset.h"
#include "dompool.h"
#include "schemawidget.h"
#include "schemaeditorform.h"
#include "schsymboleditorform.h"
#include "templatestyle.h"

#include "mainwindow.h"

namespace dsim
{

MainWindow *MainWindow::m_pinstance = 0;

MainWindow::MainWindow()
          : m_template( new TemplateStyle )
          , m_dompool( new DomPool )
{
  this->setWindowTitle( appBanner );

  createActions();
  createMenuBar();
  createToolBars();
  createStatusBar();
  createWorkspace();

  readSettings();

  m_pinstance = this;
}

MainWindow::~MainWindow()
{
  m_pinstance = 0;
}

void MainWindow::readSettings()
{
  restoreGeometry( settings()->value("geometry").toByteArray());
  restoreState( settings()->value("windowState").toByteArray());
}

void MainWindow::writeSettings()
{
  settings()->setValue( "geometry", saveGeometry() );
  //settings()->setValue( "windowState", saveState() );
}

void MainWindow::closeEvent( QCloseEvent *event )
{
  //writeSettings();

  event->accept();
}

QSize MainWindow::sizeHint () const
{
  return QSize( 1087, 572 );
}

/***************************************************
  *****  Create workspace view                 *****
  ***************************************************/

void MainWindow::createActions()
{
  /*
   * File actions
   */
  fileNewProject = new QAction(QIcon((":/arts/filenew.png")), tr("&New project"), this);
  fileNewProject->setShortcut(Qt::CTRL+Qt::Key_N);
  fileNewProject->setStatusTip(tr("Create a new project."));
  connect(fileNewProject, SIGNAL(triggered()), SLOT(onFileNewProject()));

  fileNewSchema = new QAction(tr("&New Schema file"), this);
  fileNewSchema->setStatusTip(tr("Create a new schema file."));
  connect(fileNewSchema, SIGNAL(triggered()), SLOT(onFileNewSchema()));

  fileNewSchemaSymbol = new QAction(tr("&New Schema Symbol file"), this);
  fileNewSchemaSymbol->setStatusTip(tr("Create a new schema symbol file."));
  connect(fileNewSchemaSymbol, SIGNAL(triggered()), SLOT(onFileNewSchemaSymbol()));

  fileOpen = new QAction(QIcon((":/arts/fileopen.png")), tr("&Open"), this);
  fileOpen->setStatusTip(tr("Open a file."));
  connect(fileOpen, SIGNAL(triggered()), SLOT(onFileOpen()));

  fileSave = new QAction(QIcon((":/arts/filesave.png")), tr("&Save"), this);
  fileSave->setStatusTip(tr("Save the current file."));
  connect(fileSave, SIGNAL(triggered()), SLOT(onFileSave()));

  fileQuit = new QAction(tr("&Quit"), this);
  fileQuit->setStatusTip(tr("Quit the dsim."));
  connect(fileQuit, SIGNAL(triggered()), SLOT(onFileQuit()));
}

void MainWindow::createMenuBar()
{
  /*
   * Menu entry file.
   */
  fileMenu = new QMenu( tr("&File") );
  fileMenu->addAction(fileNewProject);

  QMenu *fileNewfilesMenu = new QMenu(tr("New..."));
  fileNewfilesMenu->addAction( fileNewSchema );
  fileNewfilesMenu->addAction( fileNewSchemaSymbol );
  fileMenu->addMenu(fileNewfilesMenu);
  fileMenu->addAction(fileOpen);
  fileMenu->addAction(fileSave);
  fileMenu->addSeparator();
  fileMenu->addAction(fileQuit);

  menuBar()->addMenu(fileMenu);
}

void MainWindow::createToolBars()
{
  /*
   * ToolBar file.
   */
  fileToolBar = new QToolBar( tr("File") ); // toolBar file
  fileToolBar->addAction(fileNewProject);
  fileToolBar->addAction(fileOpen);
  fileToolBar->addAction(fileSave);
  fileToolBar->setIconSize(QSize(16, 16));
  this->addToolBar(fileToolBar);
}

void MainWindow::createWorkspace()
{
  /*
   * Workspace
   */
  workspace = new QMdiArea(this);
  this->setCentralWidget(workspace);
}

void MainWindow::createStatusBar()
{
  /*
   * StatusBar
   */
  statusLabel = new QLabel(tr("Ready..."));
  statusBar()->addWidget(statusLabel);
}

/***************************************************
  *****  Slot Responding                       *****
  ***************************************************/

void MainWindow::onFileNewProject()
{
  newSchemaDocument();
}

void MainWindow::onFileNewSchema()
{
  newSchemaDocument();
}

void MainWindow::onFileNewSchemaSymbol()
{
  newSchemaSymbolDocument();
}

void MainWindow::onFileOpen()
{
  using namespace std;
  QString filename = QFileDialog::getOpenFileName( this, tr("Open a file"), QString(), tr("Schema & Symbol files (*.sch *.ssym)") );
  if( filename.length() )
    {
      QFileInfo file( filename );
      string fn = filename.toStdString();

      ifstream stream( fn.c_str() );
      if( stream.good() )
        {
          if( file.suffix() == "sch" )
            {
              SchemaEditorForm *schema = newSchemaDocument();
              MainWindow::instance()->processRc( schema->openSchemaFile( stream ) );
            }
          else if( file.suffix() == "ssym" )
            {
              SchSymbolEditorForm *symbol = newSchemaSymbolDocument();
              MainWindow::instance()->processRc( symbol->openSymbolFile( stream ) );
            }
          else
            {
              MainWindow::instance()->processRc( -DS_INVALID_FILE_TYPE );
            }
        }
      else
        MainWindow::instance()->processRc( -DS_CREATE_FILE );
    }
}

void MainWindow::onFileSave()
{

}

void MainWindow::onFileQuit()
{
}


/***************************************************
  *****  Logical Functions                     *****
  ***************************************************/

void MainWindow::connectDocument( QObject *form )
{
  connect( fileSave, SIGNAL(triggered()), form, SLOT(onFileSave()) );
}

SchemaEditorForm *MainWindow::newSchemaDocument()
{
  SchemaEditorForm *schema = new SchemaEditorForm( 0l );

  this->workspace->addSubWindow( schema );

  connectDocument( schema );

  schema->showMaximized();
  return schema;
}

SchSymbolEditorForm *MainWindow::newSchemaSymbolDocument()
{
  SchSymbolEditorForm *symbol = new SchSymbolEditorForm( 0l );

  this->workspace->addSubWindow( symbol );

  connectDocument( symbol );

  symbol->showMaximized();
  return symbol;
}

//
// deal with the error code
//
void MainWindow::processRc( int rc )
{
  if( !rc ) return;

  if( rc == -DS_NO_MEMORY ) noMemory();

  std::stringstream ss;
  ss << "An error has occurred.\n" << "rc = " << rc;
  std::string s = ss.str();

  QMessageBox::critical( this, tr("Fault"), s.c_str(), QMessageBox::Abort | QMessageBox::Cancel );
}

void MainWindow::noMemory()
{
  QMessageBox::critical( this, tr("No Memory"),
                       tr("There is no memory left and the program must be terminated unexpectedly.\n"
                          "However, we will try to backup the current document and then crash."),
                       QMessageBox::Ok );
}

} // namespace dsim
