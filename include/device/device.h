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

#ifndef CIRC_DEVICE_H_
#define CIRC_DEVICE_H_

#if !defined(__cplusplus)
#error cplusplus only
#endif

#include <dsim/types.h>
#include <dsim/list.h>

#include <model/model.h>
#include <model/circ-element.h>

typedef struct circ_element_s circ_element_t;
typedef struct circuit_s circuit_t;
typedef struct circ_pin_s circ_pin_t;

class IDeviceGraph;
class ISchematic;

enum device_type
{
  DEV_ANALOG = 0,
  DEV_DIGITAL
};

typedef enum io_type
{
  IOTYPE_PASSIVE = 0,
  IOTYPE_INPUT,
  IOTYPE_OUTPUT,
  IOTYPE_IO,
  IOTYPE_POWER,
  IOTYPE_PULLUP,
  IOTYPE_PULLDOWN
} io_type_t;

typedef enum probe_type
{
  PROBE_NONE = 0,
  PROBE_VOLTAGE,
  PROBE_CURRENT
} probe_type_t;

typedef enum
{
  DEV_VAR_UNKNOWN = 0,
  DEV_VAR_DOUBLE,     /* double */
  DEV_VAR_FLOAT,      /* float */
  DEV_VAR_INTEGER,    /* int */
  DEV_VAR_BOOL,       /* int (NOT bool type variable) */
  DEV_VAR_STRPTR,     /* char * */
} device_variable_type_t;

typedef struct
{
  const char *symbol;
  const char *name;
  device_variable_type_t type;
} device_variable_prop_t;

enum PropertyClass
{
  PROPERTY_MODEL = 0,
  PROPERTY_DEVICE
};

class ModelPropertyValue
{
public:
  ModelPropertyValue();
  ~ModelPropertyValue();

  model_variable_type_t type;
  union
  {
    double vd;
    float  vf;
    int    vi;
    int    vb;
    const char *vsp;
    /** ignored pointer type, they should be assigned by device model internally */
  } u;
  int index;
  ModelPropertyValue *next;
};

class DevicePropertyValue
{
public:
  DevicePropertyValue();
  ~DevicePropertyValue();

  device_variable_type_t type;
  union
  {
    double vd;
    float  vf;
    int    vi;
    int    vb;
    const char *vsp;
  } u;
  int index;
  struct DevicePropertyValue *next;
};

class IDevice;

class IPropertyContainer
{
public:
  virtual ~IPropertyContainer() {}

  virtual ModelPropertyValue *propertyModelValue() const=0;
  virtual DevicePropertyValue *propertyDeviceValue() const=0;
  virtual int readModel( circ_element_t *element, int valueid = -1 )=0;
  virtual int readDevice( IDevice *device, int valueid = -1 )=0;
  virtual int configModel( circ_element_t *element )=0;
  virtual int configDevice( IDevice *device )=0;
};

class IDevice
{
public:
  IDevice( const char *reference, int id, void *reserved_ );
  virtual ~IDevice();

  //
  // Component Interface
  //
  virtual int           create( ISchematic *, IPropertyContainer * ) { return 0; }
  virtual int           init( ISchematic *, IPropertyContainer * ) { return 0; }
  virtual int           pin_count();
  virtual circ_pin_t *  pin( int index );
  virtual int           config( int op, ... );
  virtual int           render_frame( ISchematic *, IDeviceGraph * ) { return 0; }
  virtual int           uninit() { return 0; }

  //
  // Probe Interface
  //
  virtual probe_type_t  probe_type() { return PROBE_NONE; }
  virtual const char   *probe_attrs() { return 0l; }
  virtual double        probe_value() { return 0; }

  //
  // Inline Model Interface
  //
  circ_element_t *createModel( const char *mdel_symbol, circuit_t *circuit );
  int baseinit( const char *mdel_symbol, circuit_t *circuit );
  int baseinit( device_type type, circuit_t *circuit );
  void baseuninit();
  virtual int mdel_init();
  virtual int mdel_stamp();
  virtual int mdel_vchanged();
  virtual void mdel_reset();
  virtual void mdel_uninit();
  virtual int mdel_clock();
  virtual int mdel_event();

  virtual int reserved0() { return 0; }
  virtual int reserved1() { return 0; }
  virtual int reserved2() { return 0; }
  virtual int reserved3() { return 0; }
  virtual int reserved4() { return 0; }
  virtual int reserved5() { return 0; }
  virtual int reserved6() { return 0; }
  virtual int reserved7() { return 0; }
  virtual int reserved8() { return 0; }

public:
  void setCircuit( circuit_t *circuit );

protected:
  void           *reserved;
  const char     *m_reference;
  int             m_id;
  circ_element_t *m_mdel;
  circuit_t      *m_circuit;
};

#endif //!defined(CIRC_DEVICE_H_)
