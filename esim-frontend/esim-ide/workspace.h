#ifndef WORKSPACE_H_
#define WORKSPACE_H_

#include "decorator.h"
#include <QVector>
#include <QString>
#include <QObject>

class QWidget;
class QAction;
class QMenu;
class QToolBar;

namespace esim
{

enum WorkspaceClass
{
  WORKSPACE_UNKNOWN,
  WORKSPACE_SCHEMATIC,
  WORKSPACE_SCHEMATIC_SYMBOL
};

class WorkspaceWidgets : public QObject
{
  Q_OBJECT
public:
  ~WorkspaceWidgets();
  void activate(bool visible);
  template <class Widget> Widget *addWidget(Widget *widget)
    {
      m_widgets.push_back(widget);
      return widget;
    }
  template <class Action> Action *addAction(Action *action)
    {
      m_actions.push_back(action);
      return action;
    }
protected:
  /** Dynamic widgets */
  QVector<QWidget *> m_widgets;
  /** Dynamic widgets */
  QVector<QAction *> m_actions;
};

class MainWindow;

class WorkspaceFixed : public WorkspaceWidgets
{
public:
  WorkspaceFixed(MainWindow *mainWindow, const char *filename);
  virtual ~WorkspaceFixed();

  virtual QWidget *view() const=0;

  virtual int open()=0;

  virtual int save()=0;

  virtual int close()=0;

  virtual bool modified()=0;

  const QString &filename() const { return m_filename; }

protected:
  /** Pointer to main window */
  MainWindow *m_mainWindow;
  /** Document filename */
  QString m_filename;
};

class Workspace : public QObject
{
public:
  Workspace(MainWindow *parent = 0l);

  WorkspaceClass getClassByFilename(const char *filename);
  WorkspaceFixed *create(WorkspaceClass wclass, const char *filename);
  WorkspaceFixed *workspace(int index) const;
  int count() const;
  void remove(WorkspaceFixed *workspace);

protected:
  QList<WorkspaceFixed *> m_workspaces;
  MainWindow *m_mainWindow;
};

} // namespace esim

#endif // WORKSPACE_H_
