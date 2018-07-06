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

typedef struct circ_element_s circ_element_t;
typedef struct circuit_s circuit_t;
typedef struct circ_pin_s circ_pin_t;

class IDeviceGraph;

enum device_type
{
  DEV_ANALOG = 0,
  DEV_DIGITAL
};

struct IRECT
{
public:
  int x, y, w, h;

public:
  IRECT(int left, int top, int width, int height)
    : x(left), y(top), w(width), h(height)
  {}

};

class IDevice
{
public:
  IDevice( const char *reference, int id, circuit_t *circuit, void *reserved_ );
  virtual ~IDevice();

  //
  // Component Interface
  //
  virtual int           create() {return 0;}
  virtual int           init() {return 0;}
  virtual int           pin_count();
  virtual circ_pin_t *  pin( int index );
  virtual struct IRECT  get_bound();
  virtual int           render_frame( IDeviceGraph *graph ) {return 0;}
  virtual int           uninit() { return 0; }

  //
  // Inline model Interface
  //
  int baseinit( const char *mdel_symbol );
  int baseinit( device_type type );
  virtual int mdel_init();
  virtual int mdel_stamp();
  virtual int mdel_vchanged();
  virtual int mdel_event();
  virtual void mdel_reset();
  virtual void mdel_uninit();

  virtual int reserved0() { return 0; }
  virtual int reserved1() { return 0; }
  virtual int reserved2() { return 0; }
  virtual int reserved3() { return 0; }
  virtual int reserved4() { return 0; }
  virtual int reserved5() { return 0; }
  virtual int reserved6() { return 0; }
  virtual int reserved7() { return 0; }
  virtual int reserved8() { return 0; }

protected:
  void           *reserved;
  const char     *m_reference;
  int             m_id;
  circ_element_t *m_mdel;
  circuit_t      *m_circuit;
};

#endif //!defined(CIRC_DEVICE_H_)
