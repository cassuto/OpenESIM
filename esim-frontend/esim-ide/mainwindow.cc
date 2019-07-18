/* ****************************************************************************
 * OpenESIM (Electrical Engineering Simulator)                                *
 * Copyright (C) 2019, ESIM team. <diyer175@hotmail.com>                      *
 *                                                                            *
 * This project is free software; you can redistribute it and/or              *
 * modify it under the terms of the GNU Lesser General Public License(GPL)    *
 * as published by the Free Software Foundation; either version 2.1           *
 * of the License, or (at your option) any later version.                     *
 *                                                                            *
 * This project is distributed in the hope that it will be useful,            *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of             *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU          *
 * Lesser General Public License for more details.                            *
 * ****************************************************************************/

#include "mainwindow.h"
#include "schematicview.h"
#include "workspace.h"
#include <QWidget>
#include <QHBoxLayout>
#include <QDesktopWidget>
#include <QRect>
#include <QPoint>
#include <QAction>
#include <QMenu>
#include <QMenuBar>
#include <QToolBar>
#include <QMessageBox>
#include <QFileDialog>
#include <QCursor>
#include <iostream>

namespace esim
{

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
  m_workspace = new Workspace(this);

  setWindowTitle("OpenESIM (Electrical Engineering Simulator)");

  createWorkspace();
  createWidgets();
  updateActions();
}

MainWindow::~MainWindow()
{
}

void MainWindow::createWorkspace()
{
  /*
   * File Actions
   */
  m_actionNew = new QAction(QIcon(":/assets/filenew.png"), tr("&New document."), this);
  m_actionNew->setShortcut(QKeySequence::New);
  m_actionNew->setStatusTip(tr("New document."));
  connect(m_actionNew, SIGNAL(triggered(bool)), this, SLOT(slotNew(bool)));

  m_actionOpen = new QAction(QIcon(":/assets/fileopen.png"), tr("&Open a document."), this);
  m_actionOpen->setShortcut(QKeySequence::Open);
  m_actionOpen->setStatusTip(tr("Open a document."));
  connect(m_actionOpen, SIGNAL(triggered(bool)), this, SLOT(slotOpen(bool)));

  m_actionSave = new QAction(QIcon(":/assets/filesave.png"), tr("&Save the current file."), this);
  m_actionSave->setShortcut(QKeySequence::Save);
  m_actionSave->setStatusTip(tr("Save the current file."));
  connect(m_actionSave, SIGNAL(triggered(bool)), this, SLOT(slotSave(bool)));

  m_actionSaveall = new QAction(QIcon(":/assets/filesaveall.png"), tr("&Save All the files."), this);
  m_actionSaveall->setStatusTip(tr("Save All the files."));
  connect(m_actionSaveall, SIGNAL(triggered(bool)), this, SLOT(slotSaveall(bool)));

  m_actionClose = new QAction(QIcon(":/assets/fileclose.png"), tr("&Close the current file."), this);
  m_actionClose->setShortcut(QKeySequence::Close);
  m_actionClose->setStatusTip(tr("Close the current file."));
  connect(m_actionClose, SIGNAL(triggered(bool)), this, SLOT(slotClose(bool)));

  m_actionCloseAll = new QAction(QIcon(":/assets/filecloseall.png"), "&Close All the files.", this);
  m_actionCloseAll->setStatusTip(tr("Close All the files."));
  connect(m_actionCloseAll, SIGNAL(triggered(bool)), this, SLOT(slotCloseAll(bool)));

  m_actionCloseAllButThis = new QAction(tr("&Close All but this file."), this);
  m_actionCloseAllButThis->setStatusTip(tr("Close All the files."));
  connect(m_actionCloseAllButThis, SIGNAL(triggered(bool)), this, SLOT(slotCloseAllButThis(bool)));

  m_actionPrint = new QAction(QIcon(":/assets/print.png"), tr("&Print"), this);
  m_actionPrint->setStatusTip(tr("Print"));
  connect(m_actionPrint, SIGNAL(triggered(bool)), this, SLOT(slotPrint(bool)));

  m_actionPrintSetup = new QAction(QIcon(":/assets/printsetup.png"), tr("&Print Setup"), this);
  m_actionPrintSetup->setStatusTip(tr("Print Setup"));
  connect(m_actionPrintSetup, SIGNAL(triggered(bool)), this, SLOT(slotPrintSetup(bool)));

  m_actionQuit = new QAction(tr("&Exit"), this);
  m_actionQuit->setStatusTip(tr("Exit application"));
  connect(m_actionQuit, SIGNAL(triggered(bool)), this, SLOT(slotQuit(bool)));

  /*
   * Windows Actions
   */
  m_actionGotoPrevTab = new QAction(QIcon(":/assets/gotoprev.png"), tr("&Goto prev Table"), this);
  m_actionGotoPrevTab->setStatusTip(tr("Goto prev Table..."));
  connect(m_actionGotoPrevTab, SIGNAL(triggered(bool)), this, SLOT(slotGotoPrevTab(bool)));
  m_actionGotoNextTab = new QAction(QIcon(":/assets/gotonext.png"), tr("&Goto next Table"), this);
  m_actionGotoNextTab->setStatusTip(tr("Goto next Table..."));
  connect(m_actionGotoNextTab, SIGNAL(triggered(bool)), this, SLOT(slotGotoNextTab(bool)));

  QMenuBar *menubar = menuBar();
  /*
   * Menu - File
   */
  m_menuFile = menubar->addMenu(tr("&File"));
  m_menuFile->addAction(m_actionNew);
  m_menuFile->addSeparator();
  m_menuFile->addAction(m_actionOpen);
  m_menuFile->addAction(m_actionSave);
  m_menuFile->addAction(m_actionSaveall);
  m_menuFile->addSeparator();
  m_menuFile->addAction(m_actionClose);
  m_menuFile->addAction(m_actionCloseAll);
  m_menuFile->addSeparator();
  m_menuFile->addAction(m_actionPrint);
  m_menuFile->addAction(m_actionPrintSetup);
  m_menuFile->addSeparator();
  m_menuFile->addAction(m_actionQuit);

  /*
   * Menu - Edit
   */
  m_menuEdit = menubar->addMenu(tr("&Edit"));

  /*
   * Menu - Windows
   */
  m_menuWindows = menubar->addMenu(tr("&Windows"));
  m_menuWindows->addAction(m_actionClose);
  m_menuWindows->addAction(m_actionCloseAllButThis);
  m_menuWindows->addAction(m_actionCloseAll);
  m_menuWindows->addSeparator();
  m_menuWindows->addAction(m_actionGotoPrevTab);
  m_menuWindows->addAction(m_actionGotoNextTab);

  /*
   * File ToolBar
   */
  m_toolbarFile = addToolBar(tr("File"));
  m_toolbarFile->addAction(m_actionNew);
  m_toolbarFile->addAction(m_actionOpen);
  m_toolbarFile->addAction(m_actionSave);
  m_toolbarFile->addAction(m_actionSaveall);
  m_toolbarFile->addSeparator();
  m_toolbarFile->addAction(m_actionClose);
  m_toolbarFile->addAction(m_actionCloseAll);
  m_toolbarFile->addSeparator();
  m_toolbarFile->addAction(m_actionPrint);
  m_toolbarFile->addAction(m_actionPrintSetup);
  m_toolbarFile->setIconSize( toolIconSize() );
}

void MainWindow::createWidgets()
{
  /*
   * SDI Table
   */
  m_sdiTab = new QTabWidget(this);
  m_sdiTab->setTabsClosable(true);
  connect(m_sdiTab, SIGNAL(currentChanged(int)), this, SLOT(slotCurrentDocumentChanged(int)));
  connect(m_sdiTab, SIGNAL(tabCloseRequested(int)), this, SLOT(slotCloseDocument(int)));

  QWidget *centralWidget = new QWidget(this);
  QHBoxLayout *centralLayout = new QHBoxLayout(centralWidget);
  centralLayout->setContentsMargins(0, 0, 0, 0);
  centralLayout->setSpacing(0);
  centralLayout->addWidget(m_sdiTab);
  setCentralWidget(centralWidget);
}

void MainWindow::updateActions()
{
  bool hasDocuments = (m_workspace->count() > 0);
  bool oneMoreThanDocuments = (m_workspace->count() > 1);
  m_actionSave->setEnabled(hasDocuments);
  m_actionSaveall->setEnabled(hasDocuments);
  m_actionPrint->setEnabled(hasDocuments);

  m_actionClose->setEnabled(hasDocuments);
  m_actionCloseAll->setEnabled(oneMoreThanDocuments);
  m_actionCloseAllButThis->setEnabled(oneMoreThanDocuments);
  m_actionGotoPrevTab->setEnabled(oneMoreThanDocuments);
  m_actionGotoNextTab->setEnabled(oneMoreThanDocuments);
}

QSize MainWindow::sizeHint () const {
  return QSize(1400, 770);
}

void MainWindow::center()
{
  QRect qr(frameGeometry());
  QPoint cp(QDesktopWidget().availableGeometry().center());
  qr.moveCenter(cp);
  move(qr.topLeft());
}

WorkspaceFixed *MainWindow::newDocument(const char *filename)
{
  WorkspaceClass wclass = m_workspace->getClassByFilename(filename);
  if (wclass != WORKSPACE_UNKNOWN)
    {
      WorkspaceFixed *ws = m_workspace->create(wclass, filename);
      m_sdiTab->addTab(ws->view(), QString("%1 - [%2]").arg(filename).arg(m_workspace->count()-1));
      m_sdiTab->setCurrentWidget(ws->view());
      updateActions();
      return ws;
    }
  return 0l;
}

void MainWindow::closeDocument(int tabIndex)
{
  WorkspaceFixed *ws = m_workspace->workspace(tabIndex);
  if (ws->modified())
    {
      int btn = QMessageBox::warning(this,
          tr("Close document"),
          QString(tr("The document '%1' has been modified, would you want to save it?")).arg(ws->filename()),
          QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);

      if (btn == QMessageBox::Yes)
        { ws->save(); }
    }
  m_workspace->remove(ws);
  m_sdiTab->removeTab(m_sdiTab->indexOf(ws->view()));
  updateActions();
}

void MainWindow::openDocument(const char *filename)
{
  if (WorkspaceFixed *ws = newDocument(filename))
    { ws->open(); }
}

void MainWindow::saveDocument(int tabIndex)
{
  WorkspaceFixed *ws = m_workspace->workspace(tabIndex);
  ws->save();
}

void MainWindow::gotoPrevTab()
{
  int last = m_workspace->count();
  int current = m_sdiTab->currentIndex();
  if (last)
    {
      last -= 1;
      current = (current == 0) ? last : current - 1;
      m_sdiTab->setCurrentIndex(current);
    }
}

void MainWindow::gotoNextTab()
{
  int last = m_workspace->count();
  int current = m_sdiTab->currentIndex();
  if (last)
    {
      last -= 1;
      current = (current == last) ? 0 : current + 1;
      m_sdiTab->setCurrentIndex(current);
    }
}

/**
 * @brief Activate workspace.
 * @param index Index of target workspace
 */
void MainWindow::activateWorkspace(int index)
{
  for(int i=0; i < m_workspace->count(); i++)
    {
      WorkspaceFixed *currentDocument = m_workspace->workspace(i);
      currentDocument->activate(i==index); // Activate the current workspace while disabling others.
    }
  updateActions();
}

/*
 * Slot handlers
 */
void MainWindow::slotCurrentDocumentChanged(int index)
{
  if (index != -1)
    { activateWorkspace(index); }
  updateActions();
}

void MainWindow::slotCloseDocument(int index)
{
  if (index != -1)
    { closeDocument(index); }
}

void MainWindow::slotNew(bool checked)
{
  newDocument("./sch1.sch");
}
void MainWindow::slotOpen(bool checked)
{
  QString filename = QFileDialog::getOpenFileName(this, tr("Open file"));
  if (filename.length())
    { openDocument(filename.toLatin1().data()); }
}
void MainWindow::slotSave(bool checked)
{
  saveDocument(m_sdiTab->currentIndex());
}
void MainWindow::slotSaveall(bool checked)
{
  for (int i=0; i < m_workspace->count(); i++)
    saveDocument(i);
}
void MainWindow::slotClose(bool checked)
{
  closeDocument(m_sdiTab->currentIndex());
}
void MainWindow::slotCloseAll(bool checked)
{
  while (m_workspace->count() > 0)
    { closeDocument(0); }
}
void MainWindow::slotCloseAllButThis(bool checked)
{
  int thisId = m_sdiTab->currentIndex();
  int i = 0;
  while (m_sdiTab->count() > 1)
    {
      if (i < thisId)
        { closeDocument(0); }
      else if (i > thisId)
        { closeDocument(1); }
      i += 1;
    }
}
void MainWindow::slotPrint(bool checked)
{
}
void MainWindow::slotPrintSetup(bool checked)
{
}
void MainWindow::slotQuit(bool checked)
{
}
void MainWindow::slotGotoPrevTab(bool chekced)
{
  gotoPrevTab();
}
void MainWindow::slotGotoNextTab(bool checked)
{
  gotoNextTab();
}

} // namespace esim
