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

#include "elementpin.h"
#include "renderdevice.h"
#include "persistencedom.h"
#include "errtrace.h"
#include "util.h"
#include "elementtext.h"
#include "schematic.h"
#include <cmath>

namespace schematic
{

ElementPin::ElementPin(int x1, int y1) :
    Element(),
    m_x1(x1),
    m_y1(y1),
    m_length(10 * 2),
    m_symbolSpacing(5),
    m_numSpacing(2),
    m_symbol("PIN"),
    m_num("1"),
    m_angle(0),
    m_elecType(util::ELEC_INPUT),
    m_pinType(util::PIN_NONE),
    m_lineX1(0),
    m_lineY1(0),
    m_lineX2(0),
    m_lineY2(0),
    m_lineStyle(*(StyleTemplates::getInstance()->getLineStyleRef("pin"))),
    m_pinSymbolText(new ElementText),
    m_pinNumText(new ElementText)
{
  m_pinSymbolText->setText(m_symbol);
  m_pinSymbolText->setTextStyle(*(StyleTemplates::getInstance()->getTextStyleRef("symbol")));

  m_pinNumText->setText(m_num);
  m_pinNumText->setTextStyle(*(StyleTemplates::getInstance()->getTextStyleRef("pin")));

}

const char *ElementPin::classname() const
{
  return "pin";
}

int ElementPin::locationType() const
{
  return S0_POS_1P | S1_ROTATION_1A;
}

void ElementPin::locate(int opcode, int paramSize, ...)
{
  va_list vlist;
  va_start(vlist, paramSize);
  switch(opcode)
  {
    case locateSetPos:
      ASSERT_PARAMS_RETURN (paramSize == 2 * sizeof(int));

      m_x1 = va_arg(vlist, int);
      m_y1 = va_arg(vlist, int);
      relayout();
      break;
    case locateGetPos:
      ASSERT_PARAMS_RETURN (paramSize == 2 * sizeof(int*));

      *va_arg(vlist, int*) = m_x1;
      *va_arg(vlist, int*) = m_y1;
      break;
    case locateSetRotation:
      ASSERT_PARAMS_RETURN (paramSize == 1 * sizeof(int));
        {
          int ca = va_arg(vlist, int);
          int angle = std::abs(ca);
          int coeff = ca > 0 ? 1 : -1;
          while (angle > 360)
            { angle -= 360; }
          if (0 <= angle && angle < 90) // only do angles in four directios are used
            { angle = 0; }
          else if (90 <= angle && angle < 180)
            { angle = 90; }
          else if (180 <= angle && angle < 270)
            { angle = 180; }
          else if (270 <= angle && angle < 360)
            { angle = 270; }
          angle *= coeff;
          m_angle = (angle < 0) ? (angle + 360) : angle; // normalize the angle to positive
          relayout();
        }
      break;
    case locateGetRotation:
      ASSERT_PARAMS_RETURN (paramSize == 1 * sizeof(int*));

      *va_arg(vlist, int*) = m_angle;
      break;
  }
  va_end(vlist);
  return;
}

/**
 * @brief Layout the pin paintings in four directions
    called when the layout has been changed.
 */
void ElementPin::relayout()
{
  m_pinSymbolText->locate(locateSetRotation, sizeof(int), m_angle);
  m_pinNumText->locate(locateSetRotation, sizeof(int), m_angle);

  m_lineX1 = m_x1; m_lineY1 = m_y1;

  int cx, cy;

  /*
   * Layout:
   *               NumText(center)
   * [ ]  ( ) -----------------------   SymbolText
   *  |    |
   * note port symbol
   */
  switch (m_angle)
  {
    case 180: // left
      m_lineX2 = m_x1 - m_length;
      m_lineY2 = m_y1;

      cx = m_lineX1 - (m_length - m_pinNumText->getTextWidth())/2;
      cy = m_lineY1 - m_pinNumText->getTextHeight() - m_numSpacing;
      m_pinNumText->locate(locateSetPos, 2 * sizeof(int), cx ,cy);

      cx = m_lineX2 - m_symbolSpacing;
      cy = m_lineY2 - (m_pinNumText->getTextHeight()/2);
      m_pinSymbolText->locate(locateSetPos, 2 * sizeof(int), cx ,cy);
      break;

    case 90: // top
      m_lineX2 = m_x1;
      m_lineY2 = m_y1 + m_length;

      cy = m_lineY1 + (m_length - m_pinNumText->getTextWidth())/2;
      cx = m_lineX1 - m_numSpacing;
      m_pinNumText->locate(locateSetPos, 2 * sizeof(int), cx, cy);

      cy = m_lineY1 + m_length + m_symbolSpacing;
      cx = m_lineX1 + (m_pinNumText->getTextHeight()/2);
      m_pinSymbolText->locate(locateSetPos, 2 * sizeof(int), cx, cy);
      break;

    case 0: // right
      m_lineX2 = m_x1 + m_length;
      m_lineY2 = m_y1;

      cx = m_lineX1 + (m_length - m_pinNumText->getTextWidth())/2;
      cy = m_lineY1 - m_pinNumText->getTextHeight() - m_numSpacing;
      m_pinNumText->locate(locateSetPos, 2 * sizeof(int), cx, cy);

      cx = m_lineX2 + m_symbolSpacing;
      cy = m_lineY2 - (m_pinNumText->getTextHeight()/2);
      m_pinSymbolText->locate(locateSetPos, 2 * sizeof(int), cx, cy);
      break;

    case 270:
      m_lineX2 = m_x1;
      m_lineY2 = m_y1 - m_length;

      cy = m_lineY2 + (m_length+m_pinNumText->getTextWidth())/2;
      cx = m_lineX1 - m_pinNumText->getTextHeight() - m_numSpacing;
      m_pinNumText->locate(locateSetPos, 2 * sizeof(int), cx, cy);

      cy = m_lineY2 - m_symbolSpacing;
      cx = m_lineX2 - (m_pinNumText->getTextHeight()/2);
      m_pinSymbolText->locate(locateSetPos, 2 * sizeof(int), cx, cy);
  }
}

void ElementPin::render(RenderDevice *device)
{
  LineStyle oldline = device->setLineStyle(m_lineStyle);

  if (this->isModifiable())
    { device->ellipse(m_lineX1 - 5, m_lineY1 - 5, 10, 10); }
  device->line(m_lineX1, m_lineY1, m_lineX2, m_lineY2);

  device->setLineStyle(oldline);
}

void ElementPin::bounding(int *x1, int *y1, int *x2, int *y2) const
{
  *x1 = m_lineX1; *y1 = m_lineY1;
  *x2 = m_lineX2, *y2 = m_lineY2;

  *x1 = s_min(*x1, *x2); *y1 = s_min(*y1, *y2);
  *x2 = s_max(*x1, *x2), *y2 = s_max(*y1, *y2);
}

int ElementPin::serialize(persistence::PersistenceDom *dom)
{
  RC_CHECK_RETURN(Element::serialize(dom));

  RC_CHECK_RETURN(dom->ser(m_x1));
  RC_CHECK_RETURN(dom->ser(m_y1));
  RC_CHECK_RETURN(dom->ser(m_length));
  RC_CHECK_RETURN(dom->ser(m_symbolSpacing));
  RC_CHECK_RETURN(dom->ser(m_numSpacing));
  RC_CHECK_RETURN(dom->ser(m_symbol));
  RC_CHECK_RETURN(dom->ser(m_num));
  RC_CHECK_RETURN(dom->ser(m_angle));
  RC_CHECK_RETURN(dom->ser((int)m_elecType));
  RC_CHECK_RETURN(dom->ser((int)m_pinType));

  RC_CHECK_RETURN(m_lineStyle.serialize(dom));

  RC_CHECK_RETURN(m_pinNumText->serialize(dom));
  RC_CHECK_RETURN(m_pinSymbolText->serialize(dom));
  return 0;
}

int ElementPin::deserialize(persistence::PersistenceDom *dom)
{
  RC_CHECK_RETURN(Element::deserialize(dom));

  RC_CHECK_RETURN(dom->des(&m_x1));
  RC_CHECK_RETURN(dom->des(&m_y1));
  RC_CHECK_RETURN(dom->des(&m_length));
  RC_CHECK_RETURN(dom->des(&m_symbolSpacing));
  RC_CHECK_RETURN(dom->des(&m_numSpacing));
  RC_CHECK_RETURN(dom->des(&m_symbol));
  RC_CHECK_RETURN(dom->des(&m_num));
  RC_CHECK_RETURN(dom->des(&m_angle));

  int type;
  RC_CHECK_RETURN(dom->des(&type));
  m_elecType = static_cast<util::ElectricalType>(type);
  RC_CHECK_RETURN(dom->des(&type));
  m_pinType = static_cast<util::PinType>(type);

  RC_CHECK_RETURN(m_lineStyle.deserialize(dom));

  RC_CHECK_RETURN(m_pinNumText->deserialize(dom));
  RC_CHECK_RETURN(m_pinSymbolText->deserialize(dom));
  relayout();
  return 0;
}

} // namespace schematic
