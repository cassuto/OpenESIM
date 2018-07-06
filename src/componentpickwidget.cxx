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

#include <QtWidgets>

#include "componentlibrarydialog.h"
#include "componentpicktree.h"
#include "componentpickwidget.h"

namespace dsim
{

ComponentPickWidget::ComponentPickWidget( QWidget *parent )
  : QDockWidget( parent )
{
  createWidgets();
  createActions();
}

ComponentPickWidget::~ComponentPickWidget()
{
}

void ComponentPickWidget::createWidgets()
{
  QWidget *central = new QWidget( this );
  QVBoxLayout *layout = new QVBoxLayout( central );

  QWidget *filter = new QWidget( central );
  QHBoxLayout *filterLayout = new QHBoxLayout( filter );
  QPushButton *insertDeviceButton = new QPushButton( filter );
  insertDeviceButton->setIcon( QIcon(":/arts/insertdevice.png") );
  insertDeviceButton->setFlat( true );
  filterText = new QLineEdit( filter );
  filterText->setPlaceholderText( tr("Filter...") );
  filterLayout->addWidget( filterText );
  filterLayout->addWidget( insertDeviceButton );
  filterLayout->setMargin( 0 );
  filterLayout->setSpacing( 0 );
  layout->addWidget( filter );

  componentPickTree = new ComponentPickTree( central );
  clearDevices();
  layout->addWidget( componentPickTree );

  layout->setMargin( 1 );
  layout->setSpacing( 1 );
  central->setLayout( layout );

  this->setWidget( central );
  this->setWindowTitle(tr("Pick a device"));

  connect(insertDeviceButton, SIGNAL(clicked()), SLOT(onInsertDevice()));
}

void ComponentPickWidget::createActions()
{
}

void ComponentPickWidget::onInsertDevice()
{
  ComponentLibraryDialog *libraryDialog = new ComponentLibraryDialog( this );

  connect( libraryDialog, SIGNAL(insertDevice( const DeviceLibraryEntry * )), this, SLOT(slotInsertDevice( const DeviceLibraryEntry * )) );

  libraryDialog->exec();
  libraryDialog->deleteLater();
}

void ComponentPickWidget::slotInsertDevice( const DeviceLibraryEntry *entry )
{
  this->addComponent( entry );

  emit deviceInserted( entry );
}

void ComponentPickWidget::addComponent( const DeviceLibraryEntry *entry )
{
  componentPickTree->addComponent( tr("Components"), entry );
}

void ComponentPickWidget::clearDevices()
{
  componentPickTree->clear();
  componentPickTree->addCategory(tr("Components"));
}

}
