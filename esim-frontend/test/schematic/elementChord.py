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

from .element import Element, ElementSizeType
from .styleTemplate import CreateLineStyle
from .elementArc import ElementArc
from util.persistence import save, saveObj, get, getObj

class ElementChord(ElementArc):
    def __init__(self, doc, id=-1, x1=0, y1=0, x2=0, y2=0, startAngle=0, spanAngle=0):
        super().__init__(doc, id, x1, y1, x2, y2, startAngle, spanAngle)
        
    # @Override
    def classname(self):
        return "chord"
        
    # @Override
    def render(self, p):
        lineContext = p.lineContext()
        p.setLineStyle(self.lineStyle)
        
        cx1 = self.x1 if (self.x1 < self.x2) else self.x2
        cx2 = self.x1 if (self.x1 > self.x2) else self.x2
        cy1 = self.y1 if (self.y1 < self.y2) else self.y2
        cy2 = self.y1 if (self.y1 > self.y2) else self.y2
        
        p.chord(cx1, cy1, cx2-cx1+1, cy2-cy1+1, self.a1, self.a2)

        p.setLineContext(lineContext)
