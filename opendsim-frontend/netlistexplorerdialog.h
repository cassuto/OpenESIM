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

#ifndef NETLISTEXPLORERDIALOG_H_
#define NETLISTEXPLORERDIALOG_H_

#include <QtWidgets>
#include "schemasheet.h"

namespace dsim
{

class NetlistExplorerDialog : public QDialog
{
  Q_OBJECT
public:
  NetlistExplorerDialog( const QList<SchemaSheet::SchemaNode *> *nodes, QWidget *parent = 0 );

  void createDataModels();
  void createWidgets();
  void loadNodes( const QList<SchemaSheet::SchemaNode *> *nodes );

private slots:
  void onNodesListClicked( QTreeWidgetItem *item, int column );
  void onPortsListClicked( QTreeWidgetItem *item, int column );

private:
  QTreeWidget *nodesList;
  QTreeWidgetItem *nodesRoot;
  QTreeWidget *portsList;
};

}

#endif
