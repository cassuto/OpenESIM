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

#include "workspaceschematic.h"
#include "mainwindow.h"
#include "schematicview.h"
#include "schematic.h"
#include "preview.h"
#include "errtrace.h"
#include <QAction>
#include <QActionGroup>
#include <QIcon>
#include <QUndoStack>
#include <QUndoView>
#include <QToolBar>
#include <QMenu>
#include <QMenuBar>
#include <QDockWidget>
#include <fstream>
#include <cstring>
#include <cassert>

namespace esim
{

////////////////////////////////////////////////////////////////////////////////
// Fixed widgets of schematic workspace                                       //
////////////////////////////////////////////////////////////////////////////////

WorkspaceSchematic::WorkspaceSchematic(MainWindow *mainWindow, bool symbolMode, const char *filename) :
    WorkspaceFixed(mainWindow, filename),
    m_symbolMode(symbolMode),
    m_scaleMax(5),
    m_scaleMin(0.5),
    m_scaleStep(0.2)
{
  /*
   * Schematic Editor
   */
  m_schematicView = this->addWidget(new schematic::SchematicView(m_symbolMode));
  connect(m_schematicView->view(), SIGNAL(modeChanged(int, const char *)), this, SLOT(slotModeChanged(int, const char *)));

  /*
   * Actions - edit
   */
  QUndoStack *undoStack = m_schematicView->undoStack();
  QAction *actionUndo = this->addAction(undoStack->createUndoAction(this, tr("&Undo")));
  actionUndo->setIcon(QIcon(":/assets/undo.png"));
  actionUndo->setStatusTip(tr("Undo."));

  QAction *actionRedo = this->addAction(undoStack->createRedoAction(this, tr("&Redo")));
  actionRedo->setIcon(QIcon(":/assets/redo.png"));
  actionRedo->setStatusTip(tr("Redo."));

  QAction *actionCopy = this->addAction(new QAction(QIcon(":/assets/copy.png"), tr("&Copy"), this));
  actionCopy->setIcon(QIcon(":/assets/copy.png"));
  actionCopy->setStatusTip(tr("Copy"));

  QAction *actionCut = this->addAction(new QAction(QIcon(":/assets/cut.png"), tr("&Cut"), this));
  actionCut->setIcon(QIcon(":/assets/cut.png"));
  actionCut->setStatusTip(tr("Cut"));

  QAction *actionPaste = this->addAction(new QAction(QIcon(":/assets/paste.png"), tr("&Paste"), this));
  actionPaste->setIcon(QIcon(":/assets/paste.png"));
  actionPaste->setStatusTip(tr("Paste"));

  /*
   * Actions View
   */
  QAction *actionZoomCenter = this->addAction(new QAction(QIcon(":/assets/zoomcenter.png"), tr("&Center at cursor"), this));
  actionZoomCenter->setStatusTip(tr("Center at cursor."));
  connect(actionZoomCenter, SIGNAL(triggered(bool)), this, SLOT(slotZoomCenter(bool)));

  QAction *actionZoomIn = this->addAction(new QAction(QIcon(":/assets/zoomin.png"), tr("&Zoom In"), this));
  actionZoomIn->setStatusTip(tr("Zoom In."));
  connect(actionZoomIn, SIGNAL(triggered(bool)), this, SLOT(slotZoomIn(bool)));

  QAction *actionZoomOut = this->addAction(new QAction(QIcon(":/assets/zoomout.png"), tr("&Zoom Out"), this));
  actionZoomOut->setStatusTip(tr("Zoom Out."));
  connect(actionZoomOut, SIGNAL(triggered(bool)), this, SLOT(slotZoomOut(bool)));

  QAction *actionZoomFit = this->addAction(new QAction(QIcon(":/assets/zoomfit.png"), tr("&Zoom Fit"), this));
  actionZoomFit->setStatusTip(tr("Zoom Out."));
  connect(actionZoomFit, SIGNAL(triggered(bool)), this, SLOT(slotZoomFit(bool)));

  /*
   * Actions - mode
   */
  QActionGroup *actionGroupModes = new QActionGroup(this);

  m_actionSelection = this->addAction(new QAction(QIcon(":/assets/modeselection.png"), tr("&Enter selection mode."), this));
  m_actionSelection->setStatusTip(tr("Enter selection mode."));
  m_actionSelection->setCheckable(true);
  connect(m_actionSelection, SIGNAL(triggered(bool)), this, SLOT(slotSelection(bool)));
  actionGroupModes->addAction(m_actionSelection);

  if (m_symbolMode)
    {
      m_actionInsertPin = this->addAction(new QAction(QIcon(":/assets/insertpin.png"), tr("&Insert a pin to schematic."), this));
      m_actionInsertPin->setStatusTip(tr("Insert a pin to schematic."));
      m_actionInsertPin->setCheckable(true);
      connect(m_actionInsertPin, SIGNAL(triggered(bool)), this, SLOT(slotInsertPin(bool)));
      actionGroupModes->addAction(m_actionInsertPin);
    }
  else
    {
      m_actionInsertDevice = this->addAction(new QAction(QIcon(":/assets/insertdevice.png"), tr("&Insert a device to schematic."), this));
      m_actionInsertDevice->setStatusTip(tr("Insert a device to schematic."));
      m_actionInsertDevice->setCheckable(true);
      connect(m_actionInsertDevice, SIGNAL(triggered(bool)), this, SLOT(slotInsertDevice(bool)));
      actionGroupModes->addAction(m_actionInsertDevice);
    }

  m_actionInsertLine = this->addAction(new QAction(QIcon(":/assets/insertline.png"), tr("&Insert a line to schematic."), this));
  m_actionInsertLine->setStatusTip(tr("Insert a line to schematic."));
  m_actionInsertLine->setCheckable(true);
  connect(m_actionInsertLine, SIGNAL(triggered(bool)), this, SLOT(slotInsertLine(bool)));
  actionGroupModes->addAction(m_actionInsertLine);

  m_actionInsertRect = this->addAction(new QAction(QIcon(":/assets/insertrect.png"), tr("&Insert a rectangle to schematic."), this));
  m_actionInsertRect->setStatusTip(tr("Insert a rectangle to schematic."));
  m_actionInsertRect->setCheckable(true);
  connect(m_actionInsertRect, SIGNAL(triggered(bool)), this, SLOT(slotInsertRect(bool)));
  actionGroupModes->addAction(m_actionInsertRect);

  m_actionInsertCircle = this->addAction(new QAction(QIcon(":/assets/insertellipse.png"), tr("&Insert a ellipse to schematic."), this));
  m_actionInsertCircle->setStatusTip(tr("Insert a ellipse to schematic."));
  m_actionInsertCircle->setCheckable(true);
  connect(m_actionInsertCircle, SIGNAL(triggered(bool)), this, SLOT(slotInsertCircle(bool)));
  actionGroupModes->addAction(m_actionInsertCircle);

  m_actionInsertArc = this->addAction(new QAction(QIcon(":/assets/insertarc.png"), tr("&Insert an arc to schematic."), this));
  m_actionInsertArc->setStatusTip(tr("Insert an arc to schematic."));
  m_actionInsertArc->setCheckable(true);
  connect(m_actionInsertArc, SIGNAL(triggered(bool)), this, SLOT(slotInsertArc(bool)));
  actionGroupModes->addAction(m_actionInsertArc);

  m_actionInsertChord = this->addAction(new QAction(QIcon(":/assets/insertchord.png"), tr("&Insert a chord to schematic."), this));
  m_actionInsertChord->setStatusTip(tr("Insert a chord to schematic."));
  m_actionInsertChord->setCheckable(true);
  connect(m_actionInsertChord, SIGNAL(triggered(bool)), this, SLOT(slotInsertChord(bool)));
  actionGroupModes->addAction(m_actionInsertChord);

  m_actionInsertText = this->addAction(new QAction(QIcon(":/assets/inserttext.png"), tr("&Insert a text to schematic."), this));
  m_actionInsertText->setStatusTip(tr("Insert a text to schematic."));
  m_actionInsertText->setCheckable(true);
  connect(m_actionInsertText, SIGNAL(triggered(bool)), this, SLOT(slotInsertText(bool)));
  actionGroupModes->addAction(m_actionInsertText);

  m_actionInsertOrigin = this->addAction(new QAction(QIcon(":/assets/insertorigin.png"), tr("&Insert an origin to schematic."), this));
  m_actionInsertOrigin->setStatusTip("Insert an origin to schematic.");
  m_actionInsertOrigin->setCheckable(true);
  connect(m_actionInsertOrigin, SIGNAL(triggered(bool)), this ,SLOT(slotInsertOrigin(bool)));
  actionGroupModes->addAction(m_actionInsertOrigin);

  m_actionSelection->setChecked(true);

  /*
   * Menu - edit
   */
  QMenu *m_menuEdit = mainWindow->menuEdit();
  m_menuEdit->addAction(actionUndo);
  m_menuEdit->addAction(actionRedo);
  m_menuEdit->addSeparator();
  m_menuEdit->addAction(actionCopy);
  m_menuEdit->addAction(actionCut);
  m_menuEdit->addAction(actionPaste);

  /*
   * Toolbar - edit
   */
  QToolBar *toolbarEdit = this->addWidget(new QToolBar(tr("Mode")));
  toolbarEdit->addAction(actionUndo);
  toolbarEdit->addAction(actionRedo);
  toolbarEdit->addSeparator();
  toolbarEdit->addAction(actionCopy);
  toolbarEdit->addAction(actionCut);
  toolbarEdit->addAction(actionPaste);
  toolbarEdit->setIconSize(MainWindow::toolIconSize());
  mainWindow->addToolBar(toolbarEdit);

  /*
   * Toolbar - view
   */
  QToolBar *toolbarView = this->addWidget(new QToolBar(tr("Mode")));
  toolbarView->addAction(actionZoomCenter);
  toolbarView->addAction(actionZoomIn);
  toolbarView->addAction(actionZoomOut);
  toolbarView->addAction(actionZoomFit);
  toolbarView->setIconSize(MainWindow::toolIconSize());
  mainWindow->addToolBar(toolbarView);

  /*
   * Toolbar - mode
   */
  QToolBar *toolbarMode = this->addWidget(new QToolBar(tr("Mode")));
  toolbarMode->addAction(m_actionSelection);
  toolbarMode->addSeparator();
  if (m_symbolMode)
    { toolbarMode->addAction(m_actionInsertPin); }
  else
    { toolbarMode->addAction(m_actionInsertDevice); }
  toolbarMode->addSeparator();
  toolbarMode->addAction(m_actionInsertLine);
  toolbarMode->addAction(m_actionInsertRect);
  toolbarMode->addAction(m_actionInsertCircle);
  toolbarMode->addAction(m_actionInsertArc);
  toolbarMode->addAction(m_actionInsertChord);
  toolbarMode->addAction(m_actionInsertText);
  toolbarMode->addSeparator();
  toolbarMode->addAction(m_actionInsertOrigin);
  toolbarMode->setIconSize(QSize(20, 20));
  mainWindow->addToolBar(Qt::LeftToolBarArea, toolbarMode);

  /*
   * Docker - Navigation
   */
  QDockWidget *navigation = this->addWidget(new QDockWidget);
  navigation->setWindowTitle(tr("Navigation"));
  schematic::PreviewSchematic *preview = new schematic::PreviewSchematic(m_schematicView->document(), navigation);
  navigation->setWidget(preview);
  mainWindow->addDockWidget(Qt::RightDockWidgetArea, navigation);

  /*
   * Docker - History
   */
  QDockWidget *history = this->addWidget(new QDockWidget);
  history->setWindowTitle(tr("History"));
  QUndoView *undoView = new QUndoView(undoStack, history);
  history->setWidget(undoView);
  mainWindow->addDockWidget(Qt::RightDockWidgetArea, history);
}

QWidget *WorkspaceSchematic::view() const
{
  return m_schematicView;
}

int WorkspaceSchematic::open()
{
  std::ifstream stream(this->filename().toLatin1().data());
  if (stream.is_open())
    {
      return m_schematicView->document()->open(stream);
    }
  return -RC_OPENFILE;
}

int WorkspaceSchematic::save()
{
  std::ofstream stream(this->filename().toLatin1().data());
  if (stream.is_open())
    {
      return m_schematicView->document()->save(stream);
    }
  return -RC_OPENFILE;
}

int WorkspaceSchematic::close()
{
  return 0;
}

bool WorkspaceSchematic::modified()
{
  return false;
}


void WorkspaceSchematic::slotSelection(bool checked)
{
  m_schematicView->view()->switchMode(schematic::EDIT_MODE_SELECT);
}
void WorkspaceSchematic::slotInsertPin(bool checked)
{

}
void WorkspaceSchematic::slotInsertDevice(bool checked)
{

}
void WorkspaceSchematic::slotInsertLine(bool checked)
{

}
void WorkspaceSchematic::slotInsertRect(bool checked)
{
  m_schematicView->view()->switchMode(schematic::EDIT_MODE_INSERT_ELEMENT, "rect");
}
void WorkspaceSchematic::slotInsertCircle(bool checked)
{
  m_schematicView->view()->switchMode(schematic::EDIT_MODE_INSERT_ELEMENT, "circle");
}
void WorkspaceSchematic::slotInsertArc(bool checked)
{
  m_schematicView->view()->switchMode(schematic::EDIT_MODE_INSERT_ELEMENT, "arc");
}
void WorkspaceSchematic::slotInsertChord(bool checked)
{

}
void WorkspaceSchematic::slotInsertText(bool checked)
{
  m_schematicView->view()->switchMode(schematic::EDIT_MODE_INSERT_ELEMENT, "text");
}
void WorkspaceSchematic::slotInsertOrigin(bool checked)
{
  m_schematicView->view()->switchMode(schematic::EDIT_MODE_INSERT_ELEMENT, "origin");
}

void WorkspaceSchematic::slotModeChanged(int mode, const char *classname)
{
  switch (mode)
  {
    case schematic::EDIT_MODE_SELECT:
      m_actionSelection->setChecked(true);
      break;
    default:
      return;
  }
}

void WorkspaceSchematic::slotZoomCenter(bool checked)
{

}

void WorkspaceSchematic::slotZoomIn(bool checked)
{
  if (m_schematicView->document()->getScale() < m_scaleMax)
    {
      m_schematicView->view()->scale(m_schematicView->document()->getScale() + m_scaleStep);
      m_schematicView->view()->update();
    }
}

void WorkspaceSchematic::slotZoomOut(bool checked)
{
  if (m_schematicView->document()->getScale() > m_scaleMin)
    {
      m_schematicView->view()->scale(m_schematicView->document()->getScale() - m_scaleStep);
      m_schematicView->view()->update();
    }
}

void WorkspaceSchematic::slotZoomFit(bool checked)
{
  m_schematicView->document()->setScale(1.0);
  m_schematicView->view()->update();
}

} // namespace esim
