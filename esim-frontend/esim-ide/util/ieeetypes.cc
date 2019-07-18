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

#include "ieeetypes.h"

namespace util
{

const char *getElectricalTypeName(ElectricalType type)
{
  switch(type)
  {
    case ELEC_INPUT:
      return "input";
    case ELEC_OUTPUT:
      return "output";
    case ELEC_BIDIRECTIONAL:
      return "bidirectional";
    case ELEC_TRI_STATE:
      return "tri-state";
    case ELEC_PASSIVE:
      return "passive";
    case ELEC_POWER_INPUT:
      return "power input";
    case ELEC_POWER_OUTPUT:
      return "power output";
    case ELEC_OPEN_COLLECTOR:
      return "open collector";
    case ELEC_OPEN_EMITTER:
      return "open emitter";
    case ELEC_NOT_CONNECTED:
      return "not connected";
  }
  return "unknown";
}

const char *getPinTypeName(PinType type)
{
  switch (type)
  {
    case PIN_NONE:
      return "none";
    case PIN_REVERSED:
      return "reversed";
    case PIN_CLOCK:
      return "clock";
    case PIN_REVERSED_CLOCK:
      return "reversed clock";
    case PIN_LOW_INPUT:
      return "low input";
    case PIN_LOW_CLOCK:
      return "low clock";
    case PIN_LOW_OUTPUT:
      return "low output";
    case PIN_CLOCK_FALL_EDGE:
      return "clock fall edge";
    case PIN_LOGIC_NOT:
      return "logic not";
  }
  return "unknown";
}

} //namespace util
