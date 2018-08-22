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

#ifndef INSTRUMENTRACKFORM_H_
#define INSTRUMENTRACKFORM_H_

#include <QMainWindow>

class QAction;
class QToolBox;
class QVBoxLayout;

class InstrumentBase;

namespace dsim
{

class SchemaView;
class InstrumentManagement;

class InstrumentRackForm : public QMainWindow
{
  Q_OBJECT
public:
  InstrumentRackForm( SchemaView *schemaView, InstrumentManagement *rack, QWidget *parent = 0l );
  ~InstrumentRackForm();
  inline void setMdiWnd( QMdiSubWindow *mdisub ) { m_mdiWnd = mdisub; }

private:
  void createActions();
  void createToolBar();
  void createRackBox();

  void insertInstrument( const InstrumentPair &inst );

signals:
  void closed();
private slots:
  void onAddInstrument();
  void onSchemaEditorClosed();

protected:
  QSize minimumSizeHint() const;
  void closeEvent( QCloseEvent *closeEvent );

private:
  QWidget *m_parent;
  SchemaView *m_schemaView;
  InstrumentManagement *m_rack;
  QMdiSubWindow *m_mdiWnd;
  QAction *actAddInstrument;
  QToolBox *toolBox;
};

}

#endif
