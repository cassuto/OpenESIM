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

#ifndef COMPONENTPICKTREE_H_
#define COMPONENTPICKTREE_H_

#include <QtWidgets>
#include <QHash>
#include <QString>

class DeviceLibraryEntry;

namespace dsim
{

class ComponentPickTree : public QTreeWidget
{
  Q_OBJECT

public:
  ComponentPickTree( QWidget* parent );
  ~ComponentPickTree();

  void addCategory( const QString &category );
  void addComponent( const QString &category, const DeviceLibraryEntry *entry );

protected:
  void mouseReleaseEvent(QMouseEvent*);

private slots:
  void onItemClicked( QTreeWidgetItem* item, int column );
  void onContextMenu(const QPoint&);
  void onRemoveComponent();

private:
  QHash<QString, QTreeWidgetItem *> m_categories;

};

}

#endif
