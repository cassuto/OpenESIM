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

#ifndef INSTRUMENTBASE_H_
#define INSTRUMENTBASE_H_

class IDevice;

class InstrumentBase
{
public:
  explicit InstrumentBase();
  virtual ~InstrumentBase();

  virtual void clockTick()=0; // Asynchronous, called when simulator runs a step

  virtual void open()=0;
  virtual void close()=0;

  void setProbeDevice( IDevice *probe );
  inline IDevice *probeDevice() const { return m_probeDevice; }

private:
  IDevice *m_probeDevice;
};

#endif
