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

#ifndef INSTRUMENTPICKDIALOG_H_
#define INSTRUMENTPICKDIALOG_H_

#include <QDialog>

class QTreeWidget;
class QTreeWidgetItem;

class InstrumentBase;

namespace dsim
{

class InstrumentManagement;

class InstrumentPickDialog : public QDialog
{
  Q_OBJECT
public:
  InstrumentPickDialog( InstrumentManagement *rack, QWidget *parent = 0l );
  inline QString selectedClassname() const { return m_selectedClassname; }

private slots:
  void itemClicked( QTreeWidgetItem *, int );
  void itemDoubleClicked( QTreeWidgetItem *, int );

private:
  InstrumentManagement *m_rack;
  QTreeWidget   *m_instList;
  QTreeWidgetItem *m_root;
  QString m_selectedClassname;
};

}

#endif
