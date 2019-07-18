/******************************************************************************
 * OpenESIM (Electrical Engineering Simulator)                                *
 * Copyright (C) 2019, ESIM team. <diyer175@hotmail.com>                      *
 *                                                                            *
 * This project is free software; you can redistribute it and/or              *
 * modify it under the terms of the GNU Lesser General Public License(GPL)    *
 * as published by the Free Software Foundation; either version 2.1           *
 * of the License, or (at your option) any later version.                     *
 *                                                                            *
 * This project is distributed in the hope that it will be useful,            *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of             *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU          *
 * Lesser General Public License for more details.                            *
 ******************************************************************************/

#include "elementorigin.h"
#include "renderdevice.h"
#include "persistencedom.h"
#include "errtrace.h"
#include "util.h"

#define WH (10)
#define HT (10)

namespace schematic
{

ElementOrigin::ElementOrigin(int x1, int y1) :
    Element(),
    m_x1(x1), m_y1(y1)
{
}

const char *ElementOrigin::classname() const
{
  return "origin";
}

int ElementOrigin::locationType() const
{
  return S0_POS_1P;
}

void ElementOrigin::locate(int opcode, int paramSize, ...)
{
  va_list vlist;
  va_start(vlist, paramSize);
  switch(opcode)
  {
    case locateSetPos:
      ASSERT_PARAMS_RETURN (paramSize == 2 * sizeof(int));

      m_x1 = va_arg(vlist, int);
      m_y1 = va_arg(vlist, int);
      break;
    case locateGetPos:
      ASSERT_PARAMS_RETURN (paramSize == 2 * sizeof(int*));

      *va_arg(vlist, int*) = m_x1;
      *va_arg(vlist, int*) = m_y1;
      break;
  }
  va_end(vlist);
  return;
}

void ElementOrigin::render(RenderDevice *device)
{
  device->rect(m_x1-(WH/4), m_y1-(HT/4), WH/2, HT/2);
  device->line(m_x1, m_y1-(HT/2), m_x1, m_y1+HT/2);
  device->line(m_x1-(WH/2), m_y1, m_x1+WH/2, m_y1);
}

void ElementOrigin::bounding(int *x1, int *y1, int *x2, int *y2) const
{
  *x1 = m_x1-(WH/2); *y1 = m_y1-(HT/2);
  *x2 = m_x1+(WH/2), *y2 = m_y1+(HT/2);

  *x1 = s_min(*x1, *x2); *y1 = s_min(*y1, *y2);
  *x2 = s_max(*x1, *x2), *y2 = s_max(*y1, *y2);
}

int ElementOrigin::serialize(persistence::PersistenceDom *dom)
{
  RC_CHECK_RETURN(Element::serialize(dom));

  RC_CHECK_RETURN(dom->ser(m_x1));
  RC_CHECK_RETURN(dom->ser(m_y1));
  return 0;
}

int ElementOrigin::deserialize(persistence::PersistenceDom *dom)
{
  RC_CHECK_RETURN(Element::deserialize(dom));

  RC_CHECK_RETURN(dom->des(&m_x1));
  RC_CHECK_RETURN(dom->des(&m_y1));
  return 0;
}

} // namespace schematic
