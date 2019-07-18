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

#include "workspace.h"
#include "mainwindow.h"
#include "workspaceschematic.h"
#include <QWidget>
#include <QAction>
#include <cstring>
#include <iostream>

namespace esim
{

////////////////////////////////////////////////////////////////////////////////
// Base of workspace widgets                                                  //
////////////////////////////////////////////////////////////////////////////////

void WorkspaceWidgets::activate(bool active)
{
  QVectorIterator<QWidget *> i(m_widgets); // Set visible of created widgets
  while(i.hasNext())
    {
      QWidget *widget = i.next();
      widget->setEnabled(active);
      widget->setVisible(active);
    }
  QVectorIterator<QAction *> a(m_actions); // Set visible of created actions
    while(a.hasNext())
      {
        QAction *action = a.next();
        action->setEnabled(active);
        action->setVisible(active);
      }
}

WorkspaceWidgets::~WorkspaceWidgets()
{
  QVectorIterator<QWidget *> i(m_widgets); // release from heap later
  while(i.hasNext())
    { i.next()->deleteLater(); }
  QVectorIterator<QAction *> a(m_actions);
  while(a.hasNext())
    { a.next()->deleteLater(); }
}

////////////////////////////////////////////////////////////////////////////////
// Workspace - Fixed widgets                                                  //
////////////////////////////////////////////////////////////////////////////////

WorkspaceFixed::WorkspaceFixed(MainWindow *mainWindow, const char *filename) :
    m_mainWindow(mainWindow),
    m_filename(filename)
{

}

WorkspaceFixed::~WorkspaceFixed()
{
}

////////////////////////////////////////////////////////////////////////////////
// Workspace Management                                                       //
////////////////////////////////////////////////////////////////////////////////

Workspace::Workspace(MainWindow *parent) :
    QObject(parent),
    m_mainWindow(parent)
{
}

/**
 * @brief Workspace type based on filename.
 * @retval WorkspaceClass type.
 * @retval WORKSPACE_UNKNOWN if given filename is unsupported.
 */
WorkspaceClass Workspace::getClassByFilename(const char *filename)
{
  const char *p = filename + std::strlen(filename);
  while(p > filename)
    {
      if (*p == '.')
        break;
      --p;
    }
  if (p > filename)
    {
      if (std::strcmp(p, ".sch") == 0)
        { return WORKSPACE_SCHEMATIC; }
      else if (std::strcmp(p, ".ssym") == 0)
        { return WORKSPACE_SCHEMATIC_SYMBOL; }
    }
  return WORKSPACE_UNKNOWN;
}

WorkspaceFixed *Workspace::create(WorkspaceClass wclass, const char *filename)
{
  WorkspaceFixed *workspace = 0l;
  switch (wclass)
  {
    case WORKSPACE_SCHEMATIC:
      workspace = new WorkspaceSchematic(m_mainWindow, /*symbolMode*/false, filename);
      break;
    case WORKSPACE_SCHEMATIC_SYMBOL:
      workspace = new WorkspaceSchematic(m_mainWindow, /*symbolMode*/true, filename);
      break;
    case WORKSPACE_UNKNOWN:
      break;
  }
  if (workspace)
    {
      m_workspaces.push_back(workspace);
    }
  return workspace;
}

WorkspaceFixed *Workspace::workspace(int index) const
{
  return m_workspaces[index];
}

int Workspace::count() const
{
  return m_workspaces.count();
}

void Workspace::remove(WorkspaceFixed *workspace)
{
  workspace->close();
  m_workspaces.removeOne(workspace);
  workspace->deleteLater();
}

} // namespace esim
