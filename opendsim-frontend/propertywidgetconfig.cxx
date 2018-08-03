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

#include "propertywidgetconfig.h"

#include <QLayout>
#include <QLabel>

#include "propertycontainerimpl.h"
#include "propertyeditorwidget.h"

namespace dsim
{

PropertyWidgetConfig::PropertyWidgetConfig( PropertyContainerImpl *properties, QWidget *parent )
                     : PropertyWidget( properties, parent )
{
  resize( 743, 392 );

  QVBoxLayout *layout = new QVBoxLayout( this );

  QLabel *label = new QLabel( tr("Double-click to edit a property, whose value will be stored automatically."), this );
  layout->addWidget( label );
  m_editorWidget = new PropertyEditorWidget( this );
  layout->addWidget( m_editorWidget );

  PropertyModel *tabModel = m_editorWidget->model();

  for( DevicePropertyValue *device = properties->propertyDeviceValue(); device; device = device->next )
    {
      const device_variable_prop_t *prop = properties->deviceEntry( device->index );
      tabModel->addItem( device, prop->name, prop->symbol );
    }

  for( ModelPropertyValue *model = properties->propertyModelValue(); model; model = model->next )
    {
      const model_variable_prop_t *prop = properties->modelEntry( model->index );
      tabModel->addItem( model, prop->name, prop->symbol );
    }
}

}
