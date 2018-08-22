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

#ifndef PROBEPICKDIALOG_H_
#define PROBEPICKDIALOG_H_

#include <QDialog>
#include <instrument/pointerproxy.h>

class QTreeWidget;
class QTreeWidgetItem;
class IDevice;

namespace dsim
{

class SchemaView;

class ProbePickDialog : public QDialog
{
  Q_OBJECT
public:
  ProbePickDialog( SchemaView *schemaView, QWidget *parent = 0l );
  inline PointerProxy<IDevice> *selectedProbe() const { return m_selectedProbe; }

private slots:
  void itemClicked( QTreeWidgetItem *, int );
  void itemDoubleClicked( QTreeWidgetItem *, int );

private:
  QTreeWidget   *m_instList;
  QTreeWidgetItem *m_currentRoot;
  QTreeWidgetItem *m_voltageRoot;
  PointerProxy<IDevice> *m_selectedProbe;
};

}

#endif
