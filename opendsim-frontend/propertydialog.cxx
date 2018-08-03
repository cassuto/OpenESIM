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

#include <QTabWidget>
#include <QIcon>
#include <QLayout>
#include <QDialogButtonBox>
#include "propertywidget.h"
#include "propertydialog.h"

namespace dsim
{

PropertyDialog::PropertyDialog( QWidget *parent /*= 0l*/ ) : QDialog( parent )
{
  resize( 649, 384 );
  QVBoxLayout *layout = new QVBoxLayout( this );

  m_tabWidget = new QTabWidget( this );
  layout->addWidget( m_tabWidget );

  QDialogButtonBox *buttons = new QDialogButtonBox( this );
  buttons->addButton( tr("OK"), QDialogButtonBox::AcceptRole );
  buttons->addButton( tr("Cancel"), QDialogButtonBox::AcceptRole );
  layout->addWidget( buttons );

  connect( buttons, SIGNAL(accepted()), this, SLOT(onAccept()));

  connect( buttons, SIGNAL(rejected()), this, SLOT(onReject()));
}

void PropertyDialog::addPropertyWidget( PropertyWidget *widget )
{
  m_tabWidget->addTab( widget, QIcon(), widget->name() );
  m_properties.append( widget );
}

void PropertyDialog::onAccept()
{
  foreach( PropertyWidget *property, m_properties )
    {
      property->accept();
    }
  accept();
}

void PropertyDialog::onReject()
{
  foreach( PropertyWidget *property, m_properties )
    {
      property->reject();
    }
  reject();
}

int PropertyDialog::exec()
{
  if( m_properties.count()>0 ) m_properties.at( 0 )->firstPage();
  return QDialog::exec();
}

}

#include "propertydialog.moc"
