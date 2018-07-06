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

#ifndef COMPONENTPICKWIDGET_H
#define COMPONENTPICKWIDGET_H

#include <QDockWidget>

class DeviceLibraryEntry;

class QLineEdit;
class QAction;

namespace dsim
{

class ComponentPickTree;

class ComponentPickWidget : public QDockWidget
{
  Q_OBJECT

public:
  ComponentPickWidget( QWidget *parent );
  ~ComponentPickWidget();

  void addComponent( const DeviceLibraryEntry *entry );
  void clearDevices();

private:
  void createWidgets();
  void createActions();

private slots:
  void onInsertDevice();
  void slotInsertDevice( const DeviceLibraryEntry *entry );

signals:
  void deviceInserted( const DeviceLibraryEntry *entry );

private:
  QLineEdit         *filterText;
  ComponentPickTree *componentPickTree;
};

} // namespace dsim

#endif
