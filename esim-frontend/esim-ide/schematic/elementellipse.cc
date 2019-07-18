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

#include "elementellipse.h"
#include "renderdevice.h"
#include "persistencedom.h"
#include "errtrace.h"
#include "util.h"
#include "dialogs/schematic-graphicstyledialog.h"

namespace schematic
{

ElementEllipse::ElementEllipse(int x1, int y1, int x2, int y2) :
    Element(),
    m_x1(x1), m_y1(y1),
    m_x2(x2), m_y2(y2),
    m_fillStyle(*(StyleTemplates::getInstance()->getFillStyleRef("component")))
{
}

const char *ElementEllipse::classname() const
{
  return "ellipse";
}

int ElementEllipse::locationType() const
{
  return S0_POS_4P;
}

void ElementEllipse::locate(int opcode, int paramSize, ...)
{
  va_list vlist;
  va_start(vlist, paramSize);
  switch(opcode)
  {
    case locateSetPos:
      ASSERT_PARAMS_RETURN (paramSize == 4 * sizeof(int));

      m_x1 = va_arg(vlist, int);
      m_y1 = va_arg(vlist, int);
      m_x2 = va_arg(vlist, int);
      m_y2 = va_arg(vlist, int);
      break;
    case locateGetPos:
      ASSERT_PARAMS_RETURN (paramSize == 4 * sizeof(int*));

      *va_arg(vlist, int*) = m_x1;
      *va_arg(vlist, int*) = m_y1;
      *va_arg(vlist, int*) = m_x2;
      *va_arg(vlist, int*) = m_y2;
      break;
  }
  va_end(vlist);
  return;
}

void ElementEllipse::render(RenderDevice *device)
{
  FillStyle oldfill = device->setFillStyle(m_fillStyle);

  int cx1 = s_min(m_x1, m_x2);
  int cx2 = s_max(m_x1, m_x2);
  int cy1 = s_min(m_y1, m_y2);
  int cy2 = s_max(m_y1, m_y2);

  device->ellipse(cx1, cy1, cx2-cx1+1, cy2-cy1+1);

  device->setFillStyle(oldfill);
}

void ElementEllipse::bounding(int *x1, int *y1, int *x2, int *y2) const
{
  *x1 = s_min(m_x1, m_x2); *y1 = s_min(m_y1, m_y2);
  *x2 = s_max(m_x1, m_x2), *y2 = s_max(m_y1, m_y2);
}

int ElementEllipse::serialize(persistence::PersistenceDom *dom)
{
  RC_CHECK_RETURN(Element::serialize(dom));

  RC_CHECK_RETURN(dom->ser(m_x1));
  RC_CHECK_RETURN(dom->ser(m_y1));
  RC_CHECK_RETURN(dom->ser(m_x2));
  RC_CHECK_RETURN(dom->ser(m_y2));

  RC_CHECK_RETURN(m_fillStyle.serialize(dom));
  return 0;
}

int ElementEllipse::deserialize(persistence::PersistenceDom *dom)
{
  RC_CHECK_RETURN(Element::deserialize(dom));

  RC_CHECK_RETURN(dom->des(&m_x1));
  RC_CHECK_RETURN(dom->des(&m_y1));
  RC_CHECK_RETURN(dom->des(&m_x2));
  RC_CHECK_RETURN(dom->des(&m_y2));

  RC_CHECK_RETURN(m_fillStyle.deserialize(dom));
  return 0;
}

void ElementEllipse::config()
{
  SchematicGraphicsStyleDialog *dialog = new SchematicGraphicsStyleDialog(m_fillStyle);
  if (dialog->exec() == QDialog::Accepted)
    {
      m_fillStyle = dialog->fillStyle();
    }
  dialog->deleteLater();
}

} // namespace schematic
