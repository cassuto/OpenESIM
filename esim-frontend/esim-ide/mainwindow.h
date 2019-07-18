/* -*-c++-*- */
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QTabWidget;
class QResizeEvent;

namespace esim
{
class Workspace;
class WorkspaceFixed;

class MainWindow : public QMainWindow
{
  Q_OBJECT
public:
  MainWindow(QWidget *parent = 0);
  ~MainWindow();

  inline QMenu *menuFile() const { return m_menuFile; }
  inline QMenu *menuEdit() const { return m_menuEdit; }
  inline QMenu *menuWindows() const { return m_menuWindows; }
  inline QToolBar *toolbarFile() const { return m_toolbarFile; }

  static inline QSize toolIconSize() { return QSize(20, 29); }

protected:
  void center();
  QSize sizeHint () const;

private:
  void createWorkspace();
  void createWidgets();
  void updateActions();

  WorkspaceFixed *newDocument(const char *filename);
  void closeDocument(int tabIndex);
  void openDocument(const char *filename);
  void saveDocument(int tabIndex);
  void gotoPrevTab();
  void gotoNextTab();
  void activateWorkspace(int index);

protected:
  QTabWidget *m_sdiTab;
  QMenu *m_menuFile,
        *m_menuEdit,
        *m_menuWindows;
  QToolBar *m_toolbarFile;
  Workspace *m_workspace;

  QAction *m_actionNew,
          *m_actionOpen,
          *m_actionSave,
          *m_actionSaveall,
          *m_actionClose,
          *m_actionCloseAll,
          *m_actionCloseAllButThis,
          *m_actionPrint,
          *m_actionPrintSetup,
          *m_actionQuit;
  QAction *m_actionGotoPrevTab,
          *m_actionGotoNextTab;

private slots:
  void slotCurrentDocumentChanged(int index);
  void slotCloseDocument(int index);
  void slotNew(bool checked);
  void slotOpen(bool checked);
  void slotSave(bool checked);
  void slotSaveall(bool checked);
  void slotClose(bool checked);
  void slotCloseAll(bool checked);
  void slotCloseAllButThis(bool checked);
  void slotPrint(bool checked);
  void slotPrintSetup(bool checked);
  void slotQuit(bool checked);
  void slotGotoPrevTab(bool chekced);
  void slotGotoNextTab(bool checked);
};

} // namespace esim

#endif // MAINWINDOW_H
