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

#ifndef MAINWINDOW_H_
#define MAINWINDOW_H_

#include <QtGui>
#include <QMainWindow>
#include <QList>

class QMenuBar;
class QToolBar;
class QMdiArea;
class QAction;
class QLabel;

namespace dsim
{

class DomPool;
class TemplateStyle;
class SchemaEditorForm;
class SchSymbolEditorForm;

/***************************************************
  *****     MainWindow object                  *****
  ***************************************************/

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  MainWindow();
  ~MainWindow();

  void processRc( int rc );
  void noMemory();

public:
  static MainWindow* instance() { return m_pinstance; }

  inline QSettings *settings() { return &m_settings; }
  inline DomPool *dompool() { return m_dompool; }
  inline TemplateStyle *templatestyle() { return m_template; }

protected:
  void closeEvent( QCloseEvent *event );
  QSize sizeHint () const;

private:
  QSettings m_settings;

  TemplateStyle *m_template;

  DomPool *m_dompool;

  QAction *fileNewProject,
          *fileNewSchema,
          *fileNewSchemaSymbol,
          *fileOpen,
          *fileSave,
          *fileQuit;
  QMenu *fileMenu;
  QToolBar *fileToolBar;
  QMdiArea *workspace;
  QLabel *statusLabel;

  static MainWindow *m_pinstance;

  void readSettings();
  void writeSettings();
  void createActions();
  void createMenuBar();
  void createToolBars();
  void createWorkspace();
  void createStatusBar();

  void connectDocument( QObject *form );
  SchemaEditorForm *newSchemaDocument();
  SchSymbolEditorForm *newSchemaSymbolDocument();

private slots:
  void onFileNewProject();
  void onFileNewSchema();
  void onFileNewSchemaSymbol();
  void onFileOpen();
  void onFileSave();
  void onFileQuit();

signals:
  void saveDocument( const QString &filename );
};

} // namespace dsim

#endif //!defined(MAINWINDOW_H_)
