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

from PyQt5.QtGui import QFontMetrics
from .element import Element, ElementSizeType
from .styleTemplate import CreateTextStyle
from util.persistence import save, saveObj, get, getObj

class ElementText(Element):
    def __init__(self, doc, id=-1, x1=0, y1=0, text=''):
        super().__init__(doc, id)
        self.textStyle = CreateTextStyle("component")
        self.setPos(x1, y1)
        self.setRotation(0)
        self.setText(text)

    def setPos(self, x1, y1):
        self.x1 = x1
        self.y1 = y1
    def setRotation(self, ca):
        angle = abs(ca)
        coeff = 1 if ca > 0 else -1
        while angle > 360: angle -= 360
        if 0 <= angle < 90: # only do angles in four directios are used
            angle = 0
        elif 90 <= angle < 180:
            angle = 90
        elif 180 <= angle < 270:
            angle = 180
        elif 270 <= angle < 360:
            angle = 270
        angle *= coeff
        self.angle = (angle + 360) if angle < 0 else angle # normalize the angle to positive
        
    def setText(self, text):
        self.text = text
        self.textChanged()
    
    """
    @brief must be called after the text or font has been changed
    """
    def textChanged(self):
        m = QFontMetrics(self.textStyle.font());
        s = m.size(0, self.text); # get size of text
        self.textWidth = s.width();
        self.textHeight = s.height();
        
    # @Override
    def classname(self):
        return "text"
        
    # @Override
    def serialize(self, node):
        super().serialize(node)
        save(node, [self.x1, self.y1, self.angle, self.text] )
        saveObj(node, self.textStyle)
        
    # @Override
    def deserialize(self, node):
        super().deserialize(node)
        (self.x1, self.y1, self.angle, self.text) = get(node)
        getObj(node, self.textStyle)
        
        self.setRotation(self.angle)
        self.textChanged()

    # @Override
    def sizeType(self):
        return ElementSizeType.RECT_1P
        
    # @Override
    def bounding(self):
        if self.angle == 0:
            return (self.x1, self.y1, self.x1 + self.textWidth, self.y1 + self.textHeight)
        elif self.angle == 90:
            return (self.x1 - self.textHeight, self.y1, self.x1 , self.y1 + self.textWidth)
        elif self.angle == 270:
            return (self.x1, self.y1 - self.textWidth, self.x1 + self.textHeight, self.y1)
        elif self.angle == 180:
            return (self.x1 - self.textWidth, self.y1, self.x1 , self.y1 + self.textHeight)
            
    # @Override
    def render(self, p):
        textContext = p.textContext()
        p.setTextStyle(self.textStyle)
        
        p.text(self.text, self.x1, self.y1, self.angle)

        p.setTextContext(textContext)
        