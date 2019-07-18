#
#  OpenESIM (Electrical Engineering Simulator)
#  Copyright (C) 2019, ESIM team. <diyer175@hotmail.com>
#
#  This project is free software; you can redistribute it and/or
#  modify it under the terms of the GNU Lesser General Public License(GPL)
#  as published by the Free Software Foundation; either version 2.1
#  of the License, or (at your option) any later version.
#
#  This project is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
#  Lesser General Public License for more details.
#

from enum import IntEnum

class ElectricalType(IntEnum):
    ELEC_INPUT = 1
    ELEC_OUTPUT = 2
    ELEC_BIDIRECTIONAL = 3
    ELEC_TRI_STATE = 4
    ELEC_PASSIVE = 5
    ELEC_POWER_INPUT = 6
    ELEC_POWER_OUTPUT = 7
    ELEC_OPEN_COLLECTOR = 8
    ELEC_OPEN_EMITTER = 9
    ELEC_NOT_CONNECTED = 10
    
    def getName(self, val):
        if val == ELEC_INPUT:
            return "input"
        elif val == ELEC_OUTPUT:
            return "output"
        elif val == ELEC_BIDIRECTIONAL:
            return "bidirectional"
        elif val == ELEC_TRI_STATE:
            return "tri-state"
        elif val == ELEC_PASSIVE:
            return "passive"
        elif val == ELEC_POWER_INPUT:
            return "power input"
        elif val == ELEC_POWER_OUTPUT:
            return "power output"
        elif val == ELEC_OPEN_COLLECTOR:
            return "open collector"
        elif val == ELEC_OPEN_EMITTER:
            return "open emitter"
        elif val == ELEC_NOT_CONNECTED:
            return "not connected"

class PinType(IntEnum):
    PIN_NONE = 1
    PIN_REVERSED = 2
    PIN_CLOCK = 3
    PIN_REVERSED_CLOCK = 4
    PIN_LOW_INPUT = 5
    PIN_LOW_CLOCK = 6
    PIN_LOW_OUTPUT = 7
    PIN_CLOCK_FALL_EDGE = 8
    PIN_LOGIC_NOT = 9
    
    def getName(self, val):
        if val == PIN_NONE:
            return "none"
        elif val == PIN_REVERSED:
            return "reversed"
        elif val == PIN_CLOCK:
            return "clock"
        elif val == PIN_REVERSED_CLOCK:
            return "reversed clock"
        elif val == PIN_LOW_INPUT:
            return "low input"
        elif val == PIN_LOW_CLOCK:
            return "low clock"
        elif val == PIN_LOW_OUTPUT:
            return "low output"
        elif val == PIN_CLOCK_FALL_EDGE:
            return "clock fall edge"
        elif val == PIN_LOGIC_NOT:
            return "logic not"
