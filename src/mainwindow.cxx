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

#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtGui/QtGui>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QWidget>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QMdiArea>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QLabel>

#include <dsim/error.h>
#include <dsim/version.h>

#include "schemawidget.h"
#include "mainwindow.h"

namespace dsim
{

MainWindow *MainWindow::m_pinstance = 0;

MainWindow::MainWindow()
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
  restoreGeometry( m_settings.value("geometry").toByteArray());
  restoreState( m_settings.value("windowState").toByteArray());
}

void MainWindow::writeSettings()
{
  m_settings.setValue( "geometry", saveGeometry() );
  m_settings.setValue( "windowState", saveState() );
}

void MainWindow::closeEvent( QCloseEvent *event )
{
  // write the settings
  MainWindow::instance()->settings().setValue( "geometry", saveGeometry() );

  event->accept();
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
  connect(fileNewProject, SIGNAL(triggered()), SLOT(slotFileNewProject()));

  fileNewSchema = new QAction(tr("&New Schema file"), this);
  fileNewSchema->setStatusTip(tr("Create a new schema file."));
  connect(fileNewSchema, SIGNAL(triggered()), SLOT(slotFileNewSchema()));

  fileOpen = new QAction(QIcon((":/arts/fileopen.png")), tr("&Open"), this);
  fileOpen->setStatusTip(tr("Open a file."));
  connect(fileOpen, SIGNAL(triggered()), SLOT(slotFileOpen()));

  fileSave = new QAction(QIcon((":/arts/filesave.png")), tr("&Save"), this);
  fileSave->setStatusTip(tr("Save the current file."));
  connect(fileSave, SIGNAL(triggered()), SLOT(slotFileSave()));

  fileQuit = new QAction(tr("&Quit"), this);
  fileQuit->setStatusTip(tr("Quit the dsim."));
  connect(fileQuit, SIGNAL(triggered()), SLOT(slotFileQuit()));

  /*
   * Mode actions
   */
  modeSelection = new QAction(QIcon((":/arts/modeselection.png")), tr("Selection mode"), this);
  modeSelection->setStatusTip(tr("Enter the selection drawing mode."));
  modeDrawComponent = new QAction(QIcon((":/arts/modecomponent.png")), tr("Drawing the component mode"), this);
  modeDrawComponent->setStatusTip(tr("Enter the drawing the component mode."));
  modeDrawJunctionDot = new QAction(QIcon((":/arts/modejunctiondot.png")), tr("Drawing the junction dot mode"), this);
  modeDrawJunctionDot->setStatusTip(tr("Enter the drawing the junction dot mode mode."));
  modeDrawLable = new QAction(QIcon((":/arts/modelabel.png")), tr("Drawing the label mode"), this);
  modeDrawLable->setStatusTip(tr("Enter the drawing the label mode."));
  modeDrawScript = new QAction(QIcon((":/arts/modescript.png")), tr("Drawing the script mode"), this);
  modeDrawScript->setStatusTip(tr("Enter the drawing the script mode mode."));
  modeDrawBus = new QAction(QIcon((":/arts/modebus.png")), tr("Drawing the bus mode"), this);
  modeDrawBus->setStatusTip(tr("Enter the drawing the bus mode mode."));
  modeDrawSubCircuit = new QAction(QIcon((":/arts/modesubcircuit.png")), tr("Drawing the sub circuit mode"), this);
  modeDrawSubCircuit->setStatusTip(tr("Enter the drawing the sub circuit mode mode."));
}

void MainWindow::createMenuBar()
{
  /*
   * Menu entry file.
   */
  fileMenu = new QMenu(tr("&File"));
  fileMenu->addAction(fileNewProject);

  QMenu *fileNewfilesMenu = new QMenu(tr("New..."));
  fileNewfilesMenu->addAction(fileNewSchema);
  fileMenu->addMenu(fileNewfilesMenu);
  fileMenu->addAction(fileOpen);
  fileMenu->addAction(fileSave);
  fileMenu->addSeparator();
  fileMenu->addAction(fileQuit);

  menuBar()->addMenu(fileMenu);
  menuBar()->show();
}

void MainWindow::createToolBars()
{
  /*
   * ToolBar file.
   */
  fileToolBar = new QToolBar(tr("File")); // toolBar file
  fileToolBar->addAction(fileNewProject);
  fileToolBar->addAction(fileOpen);
  fileToolBar->addAction(fileSave);
  fileToolBar->setIconSize(QSize(16, 16));
  this->addToolBar(fileToolBar);
  fileToolBar->show();

  /*
   * ToolBar drawing mode.
   */
  modeToolBar = new QToolBar(tr("Mode"));
  modeToolBar->addAction(modeSelection);
  modeToolBar->addAction(modeDrawComponent);
  modeToolBar->addAction(modeDrawJunctionDot);
  modeToolBar->addAction(modeDrawLable);
  modeToolBar->addAction(modeDrawScript);
  modeToolBar->addAction(modeDrawBus);
  modeToolBar->addAction(modeDrawSubCircuit);
  modeToolBar->addSeparator();
  modeToolBar->setIconSize(QSize(16, 16));
  this->addToolBar(Qt::LeftToolBarArea, modeToolBar);
  modeToolBar->show();
}

void MainWindow::createWorkspace()
{
  /*
   * Workspace
   */
  workspace = new QMdiArea(this);
  this->setCentralWidget(workspace);
  workspace->show();
}

void MainWindow::createStatusBar()
{
  /*
   * StatusBar
   */
  statusLabel = new QLabel(tr("Ready..."));
  statusBar()->addWidget(statusLabel);
  statusBar()->show();
}

/***************************************************
  *****  Slot Responding                       *****
  ***************************************************/

void MainWindow::slotFileNewProject()
{
  newDocument();
}

void MainWindow::slotFileNewSchema()
{
}

void MainWindow::slotFileOpen()
{
}

void MainWindow::slotFileSave()
{
}

void MainWindow::slotFileQuit()
{
}


/***************************************************
  *****  Logical Functions                     *****
  ***************************************************/

/**
 * Create a document and add it to document pool.
 * @return status code
 */
int MainWindow::newDocument()
{
  int rc;
  SchemaWidget *schema = new SchemaWidget( 0 );

  this->workspace->addSubWindow( schema );
  schema->showMaximized();
  return 0;
}


} // namespace dsim
