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

#include "elementcircle.h"
#include "renderdevice.h"
#include "persistencedom.h"
#include "errtrace.h"
#include "util.h"
#include "dialogs/schematic-graphicstyledialog.h"
#include <cmath>

namespace schematic
{

ElementCircle::ElementCircle(int x1, int y1, int radius) :
    Element(),
    m_x1(x1), m_y1(y1),
    m_radius(radius),
    m_fillStyle(*(StyleTemplates::getInstance()->getFillStyleRef("component")))
{
}

const char *ElementCircle::classname() const
{
  return "circle";
}

int ElementCircle::locationType() const
{
  return S0_POS_1P1R;
}

void ElementCircle::locate(int opcode, int paramSize, ...)
{
  va_list vlist;
  va_start(vlist, paramSize);
  switch(opcode)
  {
    case locateSetPos:
      ASSERT_PARAMS_RETURN (paramSize == 3 * sizeof(int));

      m_x1 = va_arg(vlist, int);
      m_y1 = va_arg(vlist, int);
      m_radius = std::abs(va_arg(vlist, int));
      break;
    case locateGetPos:
      ASSERT_PARAMS_RETURN (paramSize == 3 * sizeof(int*));

      *va_arg(vlist, int*) = m_x1;
      *va_arg(vlist, int*) = m_y1;
      *va_arg(vlist, int*) = m_radius;
      break;
  }
  va_end(vlist);
  return;
}

void ElementCircle::render(RenderDevice *device)
{
  FillStyle oldfill = device->setFillStyle(m_fillStyle);

  int cx1 = m_x1 - m_radius;
  int cy1 = m_y1 - m_radius;

  device->ellipse(cx1, cy1, m_radius << 1, m_radius << 1);

  device->setFillStyle(oldfill);
}

void ElementCircle::bounding(int *x1, int *y1, int *x2, int *y2) const
{
  *x1 = m_x1 - m_radius; *y1 = m_y1 - m_radius;
  *x2 = m_x1 + m_radius, *y2 = m_y1 + m_radius;
}

int ElementCircle::serialize(persistence::PersistenceDom *dom)
{
  RC_CHECK_RETURN(Element::serialize(dom));

  RC_CHECK_RETURN(dom->ser(m_x1));
  RC_CHECK_RETURN(dom->ser(m_y1));
  RC_CHECK_RETURN(dom->ser(m_radius));

  RC_CHECK_RETURN(m_fillStyle.serialize(dom));
  return 0;
}

int ElementCircle::deserialize(persistence::PersistenceDom *dom)
{
  RC_CHECK_RETURN(Element::deserialize(dom));

  RC_CHECK_RETURN(dom->des(&m_x1));
  RC_CHECK_RETURN(dom->des(&m_y1));
  RC_CHECK_RETURN(dom->des(&m_radius));

  RC_CHECK_RETURN(m_fillStyle.deserialize(dom));
  return 0;
}

void ElementCircle::config()
{
  SchematicGraphicsStyleDialog *dialog = new SchematicGraphicsStyleDialog(m_fillStyle);
  if (dialog->exec() == QDialog::Accepted)
    {
      m_fillStyle = dialog->fillStyle();
    }
  dialog->deleteLater();
}

} // namespace schematic
