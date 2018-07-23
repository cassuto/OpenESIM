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
#include <dsim/error.h>

#include "lispdataset.h"
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
  insertDeviceButton->setIcon( QIcon(":/bitmaps/insertdevice.png") );
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

  connect(insertDeviceButton, SIGNAL(clicked()), SLOT(onInsertDeviceButton()));
}

void ComponentPickWidget::createActions()
{
}

void ComponentPickWidget::onInsertDeviceButton()
{
  ComponentLibraryDialog *libraryDialog = new ComponentLibraryDialog( this );

  connect( libraryDialog, SIGNAL(insertDevice( const DeviceLibraryEntry * )), this, SLOT(onInsertDevice( const DeviceLibraryEntry * )) );

  libraryDialog->exec();
  libraryDialog->deleteLater();
}

void ComponentPickWidget::onInsertDevice( const DeviceLibraryEntry *entry )
{
  this->componentPickTree->addComponent( "Components", entry );
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

int ComponentPickWidget::serialize( LispDataset *dom )
{
  int rc;

  rc = dom->beginEntry( "picked-devices" );                 UPDATE_RC(rc);
  {
    QTreeWidgetItemIterator it( componentPickTree->treeEntry("Components") );
    while( *it )
      {
        const DeviceLibraryEntry *entry = componentPickTree->deivceEntry(*it);
        if( entry )
          {
            rc = dom->beginNode();                          UPDATE_RC(rc);

            rc = dom->ser( "Components" );                  UPDATE_RC(rc);

            rc = dom->ser( entry->symbol_name );            UPDATE_RC(rc);

            rc = dom->endNode();                            UPDATE_RC(rc);
          }
        it++;
      }
  }
  rc = dom->endEntry();                                     UPDATE_RC(rc);

  return rc;
}

int ComponentPickWidget::deserialize( LispDataset *dom )
{
  int rc = 0;
  std::string symbol, category;

  DomEntry nodes = dom->entry( 0, "picked-devices" );
  {
    for( nodes.begin(); nodes.valid(); ++nodes )
      {
        dom->deserializePush( nodes.child() );
        {
          LispDataset::AutoPop autoPop( dom );

          rc = dom->des( category );                        UPDATE_RC(rc);

          rc = dom->des( symbol );                          UPDATE_RC(rc);

          const DeviceLibraryEntry *entry = device_lib_entry( symbol.c_str() );
          if( entry )
            {
              componentPickTree->addComponent( QString( category.c_str() ), entry );
            }
          else
            {
              return -DS_INVALID_DEVICE_SYMBOL;
            }
        } // auto pop
      }
  }

  return rc;
}

}

#include "componentpickwidget.moc"
