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

#ifndef ELEMENTABSRTACTPORT_H_
#define ELEMENTABSRTACTPORT_H_

namespace dsim
{

class ElementWire;
class ElementBase;

class ElementAbstractPort
{
public:
  ElementAbstractPort();
  virtual ~ElementAbstractPort();

  virtual QPointF portScenePos() const = 0;
  virtual int parentId() const = 0;
  inline ElementWire *connectedWire() const { return m_connectedWire; }
  inline ElementAbstractPort *oppositePort() const { return m_oppositePort; }
  inline void setOppositePort( ElementAbstractPort *port ) { m_oppositePort = port; }
  virtual void disconnectedEvent() {}

protected: // for ElementWire and ElementJoint only
  virtual void connectWire( ElementWire *wire );
  virtual void disconnectWire( ElementWire *wire, bool boardcast = false );

  friend class ElementWire;
  friend class ElementJoint;

private:
  ElementWire              *m_connectedWire;
  ElementAbstractPort      *m_oppositePort;
};

ElementAbstractPort *elementabstractport_cast( ElementBase *base );

}

#endif
