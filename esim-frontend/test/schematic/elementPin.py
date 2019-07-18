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
from .element import Element, ElementSizeType, ElementDirection
from .styleTemplate import CreateLineStyle, CreateTextStyle
from util.persistence import save, saveObj, get, getObj
from util.ieeeTypes import ElectricalType, PinType

class ElementPin(Element):
    def __init__(self, doc, id=-1, x1=0, y1=0):
        super().__init__(doc, id)
        self.lineStyle = CreateLineStyle("pin")
        self.length = doc.GridX * 2
        self.symbolSpacing = 5
        self.numSpacing = 2
        self.symbol = "PIN"
        self.num = "1"
        self.angle = 0
        self.dir = ElementDirection.DIR_LEFT
        self.elecType = ElectricalType.ELEC_INPUT
        self.pinType = PinType.PIN_NONE
        self.x1 = x1
        self.y1 = y1
        self.lineX1 = self.lineY1 = self.lineX2 = self.lineY2 = 0
        self.pinSymbolText = None
        self.pinNumText = None
        
    # @Override
    def resolveSubElements(self, doc):
        if super().resolveSubElements(doc):
            # Deserialize from stream
            self.pinSymbolText = self.subElements[0]
            self.pinNumText = self.subElements[1]
        else:
            # Create text element for port symbol
            self.pinSymbolText = doc.createElement("text")
            self.pinSymbolText.setText(self.symbol)
            self.pinSymbolText.textStyle = CreateTextStyle("symbol")
            
            self.pinNumText = doc.createElement("text")
            self.pinNumText.setText(self.num)
            self.pinNumText.textStyle = CreateTextStyle("pin")
            
            self.attachElement(self.pinSymbolText)
            self.attachElement(self.pinNumText)

        self.setDirection(self.dir) # __updateLayout() as well
        
    def setPos(self, x1, y1):
        self.x1 = x1
        self.y1 = y1
        self.__updateLayout()
        
    def setDirection(self, dir):
        self.dir = dir
        if dir == ElementDirection.DIR_RIGHT:
            self.angle = 0
        elif dir == ElementDirection.DIR_TOP:
            self.angle = 90
        elif dir == ElementDirection.DIR_LEFT:
            self.angle = 180
        elif dir == ElementDirection.DIR_BOTTOM:
            self.angle = 270
        self.__updateLayout()
        
    def setSymbolSpacing(self, symbolSpacing):
        self.symbolSpacing = symbolSpacing
        self.__updateLayout()
    def setNumSpacing(self, numSpacing):
        self.numSpacing = numSpacing
        self.__updateLayout()
        
    def setLength(self, length):
        self.length = length
        self.__updateLayout()
        
    def setSymbol(self, symbol):
        self.symbol = symbol
        self.__updateLayout()
    def setNum(self, num):
        self.num = num
        self.__updateLayout()

    def setElecType(self, type):
        self.elecType = type
    def setPinType(self, type):
        self.pinType = type

    """
    Layout the pin paintings in four directions
    called when the layout has been changed.
    """
    def __updateLayout(self):
        self.pinSymbolText.setRotation(self.angle)
        self.pinNumText.setRotation(self.angle)
        
        self.lineX1, self.lineY1 = self.x1, self.y1
        
        # Layout:
        #            NumText(center)
        # [ ]  ( ) -----------------------   SymbolText
        #  |    |
        # note port symbol
        
        if self.dir == ElementDirection.DIR_LEFT:
            self.lineX2 = self.x1 - self.length
            self.lineY2 = self.y1
            
            cx = self.lineX1 - (self.length - self.pinNumText.textWidth)/2
            cy = self.lineY1 - self.pinNumText.textHeight - self.numSpacing
            self.pinNumText.setPos(cx, cy)
            
            cx = self.lineX2 - self.symbolSpacing
            cy = self.lineY2 - (self.pinNumText.textHeight/2)
            self.pinSymbolText.setPos(cx, cy)

        elif self.dir == ElementDirection.DIR_TOP:
            self.lineX2 = self.x1
            self.lineY2 = self.y1 + self.length
            
            cy = self.lineY1 + (self.length - self.pinNumText.textWidth)/2
            cx = self.lineX1 - self.numSpacing
            self.pinNumText.setPos(cx, cy)
            
            cy = self.lineY1 + self.length + self.symbolSpacing
            cx = self.lineX1 + (self.pinNumText.textHeight/2)
            self.pinSymbolText.setPos(cx, cy)

        elif self.dir == ElementDirection.DIR_RIGHT:
            self.lineX2 = self.x1 + self.length
            self.lineY2 = self.y1
            
            cx = self.lineX1 + (self.length - self.pinNumText.textWidth)/2
            cy = self.lineY1 - self.pinNumText.textHeight - self.numSpacing
            self.pinNumText.setPos(cx, cy)
            
            cx = self.lineX2 + self.symbolSpacing
            cy = self.lineY2 - (self.pinNumText.textHeight/2)
            self.pinSymbolText.setPos(cx, cy)
            
        elif self.dir == ElementDirection.DIR_BOTTOM:
            self.lineX2 = self.x1
            self.lineY2 = self.y1 - self.length
            
            cy = self.lineY2 + (self.length+self.pinNumText.textWidth)/2
            cx = self.lineX1 - self.pinNumText.textHeight - self.numSpacing
            self.pinNumText.setPos(cx, cy)
            
            cy = self.lineY2 - self.symbolSpacing
            cx = self.lineX2 - (self.pinNumText.textHeight/2)
            self.pinSymbolText.setPos(cx, cy)

        self._invalidBounding()
            
    # @Override
    def classname(self):
        return "pin"
        
    # @Override
    def serialize(self, node):
        super().serialize(node)
        save(node, [self.x1, self.y1, self.dir, self.symbol, self.num, self.elecType, self.pinType] )
        saveObj(node, self.lineStyle)
        
    # @Override
    def deserialize(self, node):
        super().deserialize(node)
        (self.x1, self.y1, self.dir, self.symbol, self.num, self.elecType, self.pinType) = get(node)
        getObj(node, self.lineStyle)
        
    # @Override
    def sizeType(self):
        return ElementSizeType.RECT_1P
        
    # @Override
    def bounding(self):
        return super().bounding()

    # @Override
    def render(self, p):
        lineContext = p.lineContext()
        p.setLineStyle(self.lineStyle)

        if self.modifiable():
            p.ellipse(self.lineX1 - 5, self.lineY1 - 5, 10, 10);
        p.line(self.lineX1, self.lineY1, self.lineX2, self.lineY2)
        
        p.setLineContext(lineContext)
        