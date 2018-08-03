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

#ifndef PROPERTYCONTAINER_H_
#define PROPERTYCONTAINER_H_

#include <model/model.h>
#include <device/device.h>
#include <map>
#include <string>

namespace dsim
{

class LispDataset;

class PropertyContainerImpl : public IPropertyContainer
{
public:

  class PropertyEntry
  {
  public:
    PropertyEntry();
    ~PropertyEntry();

    PropertyClass                   propertyClass;
    union
    {
      const model_variable_prop_t   *model;
      const device_variable_prop_t  *device;
    } tab;
    int                             tabCount;
    union
    {
      ModelPropertyValue            *model;
      DevicePropertyValue           *device;
    } value;
    union
    {
      ModelPropertyValue            *model;
      DevicePropertyValue           *device;
    } tail;

    void addValue( ModelPropertyValue *value );
    void addValue( DevicePropertyValue *value );
    void removeValues();
  };

public:
  PropertyContainerImpl();
  ~PropertyContainerImpl();

  ModelPropertyValue *propertyModelValue() const { return m_modelEntry ? m_modelEntry->value.model->next : 0l; }
  DevicePropertyValue *propertyDeviceValue() const { return m_deviceEntry ? m_deviceEntry->value.device->next : 0l; }
  const model_variable_prop_t *modelEntry( int index );
  const device_variable_prop_t *deviceEntry( int index );

  int readModel( circ_element_t *element );
  int readDevice( IDevice *device );
  int configModel( circ_element_t *element );
  int configDevice( IDevice *device );

  int serialize( LispDataset *dataset );
  int deserialize( LispDataset *dataset );

private:
  inline PropertyEntry *propertyModel() const { return m_modelEntry; }
  inline PropertyEntry *propertyDevice() const { return m_deviceEntry; }
  void setModelEntries( const model_variable_prop_t *props, int count );
  void setDeviceEntries( const device_variable_prop_t *props, int count );
  void registerModelEntry( ModelPropertyValue *entry, const std::string &symbol );
  void registerDeviceEntry( DevicePropertyValue *entry, const std::string &symbol );
  ModelPropertyValue *findModelEntry( const std::string &symbol );
  DevicePropertyValue *findDeviceEntry( const std::string &symbol );

private:
  PropertyEntry             *m_modelEntry;
  PropertyEntry             *m_deviceEntry;
  std::map<std::string, ModelPropertyValue*> m_modelIndex;
  std::map<std::string, DevicePropertyValue*> m_deviceIndex;
};

}

#endif
