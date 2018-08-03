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

#define TRACE_UNIT "propcon"
#include <dsim/logtrace.h>
#include <frontend/error.h>
#include <dsim/string.h>
#include <dsim/circuit.h>
#include <string>

#include "lispdataset.h"
#include "propertycontainerimpl.h"

namespace dsim
{

PropertyContainerImpl::PropertyEntry::PropertyEntry()
                                 : propertyClass( PROPERTY_MODEL )
                                 , tabCount( 0 )
{
  value.model = new ModelPropertyValue;
  value.model->next = 0l;
  tail.model = value.model;

  value.device = new DevicePropertyValue;
  value.device->next = 0l;
  tail.device = value.device;
}

template <class propertyValue>
  inline void removePropertyValues( propertyValue *root )
    {
      propertyValue *node = root, *cur;
      while( node )
        {
          cur = node; node = node->next;
          delete cur;
        }
    }

PropertyContainerImpl::PropertyEntry::~PropertyEntry()
{
  removePropertyValues( value.model );
  removePropertyValues( value.device );
}

void PropertyContainerImpl::PropertyEntry::removeValues()
{
  removePropertyValues( value.model );
  removePropertyValues( value.device );
}

void PropertyContainerImpl::PropertyEntry::addValue( ModelPropertyValue *src )
{
  src->next = 0l;
  tail.model->next = src;
  tail.model = src;
}

void PropertyContainerImpl::PropertyEntry::addValue( DevicePropertyValue *src )
{
  src->next = 0l;
  tail.device->next = src;
  tail.device = src;
}

PropertyContainerImpl::PropertyContainerImpl()
                  : m_modelEntry( 0l )
                  , m_deviceEntry( 0l )
{}

PropertyContainerImpl::~PropertyContainerImpl()
{
  delete m_modelEntry;
  delete m_deviceEntry;
}

void PropertyContainerImpl::setModelEntries( const model_variable_prop_t *props, int count )
{
  trace_assert( 0l==m_modelEntry );

  m_modelEntry = new PropertyEntry;
  m_modelEntry->propertyClass = PROPERTY_MODEL;
  m_modelEntry->tab.model = props;
  m_modelEntry->tabCount = count;
}

void PropertyContainerImpl::setDeviceEntries( const device_variable_prop_t *props, int count )
{
  trace_assert( 0l==m_deviceEntry );

  m_deviceEntry = new PropertyEntry;
  m_deviceEntry->propertyClass = PROPERTY_DEVICE;
  m_deviceEntry->tab.device = props;
  m_deviceEntry->tabCount = count;
}

void PropertyContainerImpl::registerModelEntry( ModelPropertyValue *entry, const std::string &symbol )
{ m_modelIndex[symbol] = entry; }

void PropertyContainerImpl::registerDeviceEntry( DevicePropertyValue *entry, const std::string &symbol )
{ m_deviceIndex[symbol] = entry; }

ModelPropertyValue *PropertyContainerImpl::findModelEntry( const std::string &symbol )
{ return m_modelIndex[symbol]; }

DevicePropertyValue *PropertyContainerImpl::findDeviceEntry( const std::string &symbol )
{ return m_deviceIndex[symbol]; }

const model_variable_prop_t *PropertyContainerImpl::modelEntry( int index )
{ return &(propertyModel()->tab.model[index]); }

const device_variable_prop_t *PropertyContainerImpl::deviceEntry( int index )
{ return &(propertyDevice()->tab.device[index]); }

int PropertyContainerImpl::serialize( LispDataset *dataset )
{
  int rc = 0;

  if( m_modelEntry )
    {
      rc = dataset->beginNode(true, "model");       UPDATE_RC(rc);

      for( ModelPropertyValue *entry = m_modelEntry->value.model->next; entry; entry = entry->next )
        {
          const char *symbol = m_modelEntry->tab.model[entry->index].symbol;

          rc = dataset->beginNode( true, symbol );  UPDATE_RC(rc);
          switch( entry->type )
          {
            case MDEL_VAR_DOUBLE:
              {
                rc = dataset->ser( entry->u.vd );   UPDATE_RC(rc);
                break;
              }
            case MDEL_VAR_FLOAT:
              {
                rc = dataset->ser( entry->u.vf );   UPDATE_RC(rc);
                break;
              }
            case MDEL_VAR_INTEGER:
              {
                rc = dataset->ser( entry->u.vi );   UPDATE_RC(rc);
                break;
              }
            case MDEL_VAR_BOOL:
              {
                rc = dataset->ser( entry->u.vb );   UPDATE_RC(rc);
                break;
              }
            case MDEL_VAR_STRPTR:
              {
                if( entry->u.vsp )
                  { rc = dataset->ser( entry->u.vsp );  UPDATE_RC(rc); }
                else
                  { rc = dataset->ser( "null" );        UPDATE_RC(rc); }
                break;
              }
            case MDEL_VAR_VOIDPTR:
              {
                break;
              }
            case MDEL_VAR_CONST_VOIDPTR:
              {
                break;
              }
            default:
              return -DS_INVALID_PROPERTY_TYPE;
          }
          rc = dataset->endNode(true);              UPDATE_RC(rc);
        }

      rc = dataset->endNode(true);                  UPDATE_RC(rc);
    }

  if( m_deviceEntry )
    {
      rc = dataset->beginNode(true, "device");      UPDATE_RC(rc);

      for( DevicePropertyValue *entry = m_deviceEntry->value.device->next; entry; entry = entry->next )
        {
          const char *symbol = m_deviceEntry->tab.device[entry->index].symbol;

          rc = dataset->beginNode( true, symbol );  UPDATE_RC(rc);
          switch( entry->type )
          {
            case DEV_VAR_DOUBLE:
              {
                rc = dataset->ser( entry->u.vd );   UPDATE_RC(rc);
                break;
              }
            case DEV_VAR_FLOAT:
              {
                rc = dataset->ser( entry->u.vf );   UPDATE_RC(rc);
                break;
              }
            case DEV_VAR_INTEGER:
              {
                rc = dataset->ser( entry->u.vi );   UPDATE_RC(rc);
                break;
              }
            case DEV_VAR_BOOL:
              {
                rc = dataset->ser( entry->u.vb );   UPDATE_RC(rc);
                break;
              }
            case DEV_VAR_STRPTR:
              {
                if( entry->u.vsp )
                  { rc = dataset->ser( entry->u.vsp );  UPDATE_RC(rc); }
                else
                  { rc = dataset->ser( "null" );        UPDATE_RC(rc); }
                break;
              }
            default:
              return -DS_INVALID_PROPERTY_TYPE;
          }
          rc = dataset->endNode(true);              UPDATE_RC(rc);
        }

      rc = dataset->endNode(true);                  UPDATE_RC(rc);
    }
  return rc;
}

int PropertyContainerImpl::deserialize( LispDataset *dataset )
{
  int           rc = 0;
  DomEntry      entry;
  std::string   symbol;
  std::string   vcsp;

  if( m_modelEntry )
    {
      ModelPropertyValue *model;
      rc = dataset->des( entry, "model" );                      UPDATE_RC(rc);

      for( entry.begin(); entry.valid(); ++entry )
        {
          dataset->deserializePush( entry.child() );
            {
              LispDataset::AutoPop autoPop( dataset );

              rc = dataset->des( symbol, true );               UPDATE_RC(rc);

              if( !(model = findModelEntry( symbol )) )
                return -DS_UNKNOWN_PROPERTY_ENTRY;

              switch( model->type )
                {
                  case MDEL_VAR_DOUBLE:
                    {
                      rc = dataset->des( model->u.vd );         UPDATE_RC(rc);
                      break;
                    }
                  case MDEL_VAR_FLOAT:
                    {
                      rc = dataset->des( model->u.vf );         UPDATE_RC(rc);
                      break;
                    }
                  case MDEL_VAR_INTEGER:
                    {
                      rc = dataset->des( model->u.vi );         UPDATE_RC(rc);
                      break;
                    }
                  case MDEL_VAR_BOOL:
                    {
                      rc = dataset->des( model->u.vb );         UPDATE_RC(rc);
                      break;
                    }
                  case MDEL_VAR_STRPTR:
                    {
                      rc = dataset->des( vcsp );                UPDATE_RC(rc);

                      model->u.vsp = ds_strdup( vcsp.c_str() );
                      if( !model->u.vsp )
                        return -DS_NO_MEMORY;
                      break;
                    }
                  case MDEL_VAR_VOIDPTR:
                    {
                      break;
                    }
                  case MDEL_VAR_CONST_VOIDPTR:
                    {
                      break;
                    }
                  default:
                    return -DS_INVALID_PROPERTY_TYPE;
                }

                m_modelEntry->addValue( model );

            } // auto pop
        }
    }

  if( m_deviceEntry )
    {
      DevicePropertyValue *device;
      rc = dataset->des( entry, "device" );                     UPDATE_RC(rc);

      for( entry.begin(); entry.valid(); ++entry )
        {
          dataset->deserializePush( entry.child() );
            {
              LispDataset::AutoPop autoPop( dataset );

              rc = dataset->des( symbol, true );                UPDATE_RC(rc);

              if( !(device = findDeviceEntry( symbol )) )
                return -DS_UNKNOWN_PROPERTY_ENTRY;

              switch( device->type )
                {
                  case DEV_VAR_DOUBLE:
                    {
                      rc = dataset->des( device->u.vd );        UPDATE_RC(rc);
                      break;
                    }
                  case DEV_VAR_FLOAT:
                    {
                      rc = dataset->des( device->u.vf );        UPDATE_RC(rc);
                      break;
                    }
                  case DEV_VAR_INTEGER:
                    {
                      rc = dataset->des( device->u.vi );        UPDATE_RC(rc);
                      break;
                    }
                  case DEV_VAR_BOOL:
                    {
                      rc = dataset->des( device->u.vb );        UPDATE_RC(rc);
                      break;
                    }
                  case DEV_VAR_STRPTR:
                    {
                      rc = dataset->des( vcsp );                UPDATE_RC(rc);

                      device->u.vsp = ds_strdup( vcsp.c_str() );
                      if( !device->u.vsp )
                        return -DS_NO_MEMORY;
                      break;
                    }
                  default:
                    return -DS_INVALID_PROPERTY_TYPE;
                }

                m_deviceEntry->addValue( device );

            } // auto pop
        }
    }

  return rc;
}

int PropertyContainerImpl::readModel( circ_element_t *element )
{
  if( m_modelEntry ) return 0;
  int count;
  const model_variable_prop_t *props = 0l;
  int rc = circ_element_config(( element, ELM_CONFIG_LIST_COUNT, &count )); UPDATE_RC(rc);

  if( count )
    rc = circ_element_config(( element, ELM_CONFIG_LIST_PROP, &props )); UPDATE_RC(rc);
  setModelEntries( props, count );

  if( props ) // read default values
    {
      for( int i=0; i<count; i++ )
        {
          const model_variable_prop_t *var = &(propertyModel()->tab.model[i]);

          ModelPropertyValue *val = new ModelPropertyValue;
          val->type = var->type;
          val->index = i;

          switch( var->type )
          {
            case MDEL_VAR_DOUBLE:
              {
                rc = circ_element_config(( element, ELM_CONFIG_GET, i, &val->u.vd )); UPDATE_RC(rc);
                break;
              }
            case MDEL_VAR_FLOAT:
              {
                rc = circ_element_config(( element, ELM_CONFIG_GET, i, &val->u.vf )); UPDATE_RC(rc);
                break;
              }
            case MDEL_VAR_INTEGER:
              {
                rc = circ_element_config(( element, ELM_CONFIG_GET, i, &val->u.vi )); UPDATE_RC(rc);
                break;
              }
            case MDEL_VAR_BOOL:
              {
                rc = circ_element_config(( element, ELM_CONFIG_GET, i, &val->u.vb )); UPDATE_RC(rc);
                break;
              }
            case MDEL_VAR_STRPTR:
              {
                rc = circ_element_config(( element, ELM_CONFIG_GET, i, &val->u.vsp )); UPDATE_RC(rc);
                break;
              }
            case MDEL_VAR_VOIDPTR:
              {
                break;
              }
            case MDEL_VAR_CONST_VOIDPTR:
              {
                break;
              }
            default:
              return -DS_INVALID_PROPERTY_TYPE;
          }

          propertyModel()->addValue( val );

          registerModelEntry( val, propertyModel()->tab.model[i].symbol );
        }
    }

  return rc;
}

int PropertyContainerImpl::readDevice( IDevice *device )
{
  if( m_modelEntry ) return 0;
  int count;
  const device_variable_prop_t *props = 0l;
  int rc = device->config( ELM_CONFIG_LIST_COUNT, &count ); UPDATE_RC(rc);

  if( count )
    rc = device->config( ELM_CONFIG_LIST_PROP, &props ); UPDATE_RC(rc);
  setDeviceEntries( props, count );

  if( props ) // read default values
    {
      for( int i=0; i<count; i++ )
        {
          const device_variable_prop_t *var = &(propertyDevice()->tab.device[i]);

          DevicePropertyValue *val = new DevicePropertyValue;
          val->type = var->type;
          val->index = i;

          switch( var->type )
          {
            case DEV_VAR_DOUBLE:
              {
                rc = device->config( ELM_CONFIG_GET, i, &val->u.vd ); UPDATE_RC(rc);
                break;
              }
            case DEV_VAR_FLOAT:
              {
                rc = device->config( ELM_CONFIG_GET, i, &val->u.vf ); UPDATE_RC(rc);
                break;
              }
            case DEV_VAR_INTEGER:
              {
                rc = device->config( ELM_CONFIG_GET, i, &val->u.vi ); UPDATE_RC(rc);
                break;
              }
            case DEV_VAR_BOOL:
              {
                rc = device->config( ELM_CONFIG_GET, i, &val->u.vb ); UPDATE_RC(rc);
                break;
              }
            case DEV_VAR_STRPTR:
              {
                rc = device->config( ELM_CONFIG_GET, i, &val->u.vsp ); UPDATE_RC(rc);
                break;
              }
            default:
              return -DS_INVALID_PROPERTY_TYPE;
          }

          propertyDevice()->addValue( val );

          registerDeviceEntry( val, propertyDevice()->tab.device[i].symbol );
        }
    }

  return rc;
}

int PropertyContainerImpl::configModel( circ_element_t *element )
{
  int rc = 0;

  if( propertyModel()->tabCount )
    {
      for( ModelPropertyValue *val = propertyModel()->value.model; val; val=val->next )
        {
          int index = val->index;
          switch( val->type )
          {
            case MDEL_VAR_DOUBLE:
              {
                rc = circ_element_config(( element, ELM_CONFIG_SET, index, val->u.vd )); UPDATE_RC(rc);
                break;
              }
            case MDEL_VAR_FLOAT:
              {
                rc = circ_element_config(( element, ELM_CONFIG_SET, index, val->u.vf )); UPDATE_RC(rc);
                break;
              }
            case MDEL_VAR_INTEGER:
              {
                rc = circ_element_config(( element, ELM_CONFIG_SET, index, val->u.vi )); UPDATE_RC(rc);
                break;
              }
            case MDEL_VAR_BOOL:
              {
                rc = circ_element_config(( element, ELM_CONFIG_SET, index, val->u.vb )); UPDATE_RC(rc);
                break;
              }
            case MDEL_VAR_STRPTR:
              {
                rc = circ_element_config(( element, ELM_CONFIG_SET, index, val->u.vsp )); UPDATE_RC(rc);
                break;
              }
            case MDEL_VAR_VOIDPTR:
              {
                break;
              }
            case MDEL_VAR_CONST_VOIDPTR:
              {
                break;
              }
            default:
              return -DS_INVALID_PROPERTY_TYPE;
          }
        }
    }
  return rc;
}

int PropertyContainerImpl::configDevice( IDevice *device )
{
  int rc = 0;

  if( propertyDevice()->tabCount )
    {
      for( DevicePropertyValue *val = propertyDevice()->value.device; val; val=val->next )
        {
          int index = val->index;
          switch( val->type )
          {
            case DEV_VAR_DOUBLE:
              {
                rc = device->config( ELM_CONFIG_SET, index, val->u.vd ); UPDATE_RC(rc);
                break;
              }
            case DEV_VAR_FLOAT:
              {
                rc = device->config( ELM_CONFIG_SET, index, val->u.vf ); UPDATE_RC(rc);
                break;
              }
            case DEV_VAR_INTEGER:
              {
                rc = device->config( ELM_CONFIG_SET, index, val->u.vi ); UPDATE_RC(rc);
                break;
              }
            case DEV_VAR_BOOL:
              {
                rc = device->config( ELM_CONFIG_SET, index, val->u.vb ); UPDATE_RC(rc);
                break;
              }
            case DEV_VAR_STRPTR:
              {
                rc = device->config( ELM_CONFIG_SET, index, val->u.vsp ); UPDATE_RC(rc);
                break;
              }
            default:
              return -DS_INVALID_PROPERTY_TYPE;
          }
        }
    }
  return rc;
}

}
