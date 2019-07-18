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

from PyQt5.QtCore import Qt
from PyQt5.QtGui import QColor, QPainter, QPen, QBrush, QFont
from enum import IntEnum, unique
from util.persistence import save, get
import copy

@unique
class Style(IntEnum):
    LINE_NONE = 0
    LINE_SOLID = 1
    LINE_DASH = 2
    LINE_DOT = 3
    LINE_DASH_DOT = 4
    LINE_DASH_DOTDOT = 5

    BRUSH_NONE = 10
    BRUSH_SOLID = 11
    BRUSH_HOR = 12
    BRUSH_VER = 13
    BRUSH_CROSS = 14

class LineStyle():
    def __init__(self, styleName='', color=QColor(0,0,0), width=0, line=Style.LINE_NONE):
        self.styleName = styleName
        self.set(color, width, line)
        
    def set(self, color, width, line):
        self.color = color
        self.width = width
        self.line = line
        
        if self.line == Style.LINE_NONE:
            attr = Qt.NoPen
        elif self.line == Style.LINE_SOLID:
            attr = Qt.SolidLine
        elif self.line == Style.LINE_DASH:
            attr = Qt.DashLine
        elif self.line == Style.LINE_DOT:
            attr = Qt.DotLine
        elif self.line == Style.LINE_DASH_DOT:
            attr = Qt.DashDotLine
        elif self.line == Style.LINE_DASH_DOTDOT:
            attr = Qt.DashDotDotLine
        else:
            attr = Qt.SolidLine
        self.penI = QPen(self.color, self.width, attr)
        
    def pen(self):
        return self.penI
        
    def serialize(self, node):
        cust = (self != RefStyle(lineStyles, self.styleName))
        save(node, self.styleName)
        save(node, cust)
        if cust:
            save(node, [self.color.red(), self.color.green(), self.color.blue(),
                        self.width,
                        int(self.line) ])
    
    def deserialize(self, node):
        self.styleName = get(node)
        self.cust = get(node)
        if self.cust:
            red, green, blue, width, line = get(node)
            self.set(QColor(red, green, blue), width, Style(line))
        else:
            src = RefStyle(lineStyles, self.styleName) # use default
            self.set(src.color, src.width, src.line)
            
    def __eq__(self, src):
        return (self.color == src.color and
                self.width == src.width and
                self.line == src.line)
                
class FillStyle(LineStyle):
    def __init__(self, styleName='', color=QColor(0,0,0), bkcolor=QColor(0,0,0), width=0, line=Style.LINE_NONE, brusht=Style.BRUSH_NONE):
        self.styleName = styleName
        self.set(color, bkcolor, width, line, brusht)
        
    # @Override
    def set(self, color, bkcolor, width, line, brusht):
        super().set(color, width, line)
        self.bkcolor = bkcolor
        self.brusht = brusht
        
        if self.brusht == Style.BRUSH_NONE:
            attr = Qt.NoBrush
        elif self.brusht == Style.BRUSH_SOLID:
            attr = Qt.SolidPattern;
        elif self.brusht == Style.BRUSH_HOR:
            attr = Qt.HorPattern;
        elif self.brusht == Style.BRUSH_VER:
            attr = Qt.VerPattern;
        elif self.brusht == Style.BRUSH_CROSS:
            attr = Qt.CrossPattern;
        else:
            attr = Qt.NoBrush;
        self.brushI = QBrush(self.bkcolor, attr)
        
    def brush(self):
        return self.brushI

    # @Override
    def serialize(self, node):
        cust = (self != RefStyle(fillStyles, self.styleName))
        save(node, self.styleName)
        save(node, cust)
        if cust:
            save(node, [self.color.red(), self.color.blue(), self.color.green(),
                        self.bkcolor.red(),self.bkcolor.blue(), self.bkcolor.green(),
                        self.width,
                        int(self.line),
                        int(self.brusht)])

    # @Override
    def deserialize(self, node):
        self.styleName = get(node)
        self.cust = get(node)
        if self.cust:
            red, green, blue, bk_red, bk_green, bk_blue, width, line, brusht = get(node)
            self.set(QColor(red, green, blue), QColor(bk_red, bk_green, bk_blue), width, Style(line), Style(brusht))
        else:
            src = RefStyle(fillStyles, self.styleName) # use default
            self.set(src.color, src.bkcolor, src.width, src.line, src.brusht)

    # @Override
    def __eq__(self, src):
        return (super().__eq__(src) and
                (self.bkcolor == src.bkcolor and
                self.brusht == src.brusht))
            
class TextStyle():
    def __init__(self, styleName='', color=QColor(0,0,0), fontName='', size=0, bold=False, italic=False, underline=False):
        self.styleName = styleName
        self.set(color, fontName, size, bold, italic, underline)
        
    def set(self, color, fontName, size, bold, italic, underline):
        self.color = color
        self.fontName = fontName
        self.size = size
        self.bold = bold
        self.italic = italic
        self.underline = underline
        self.fontI = QFont(fontName, size)
        self.fontI.setBold(bold)
        self.fontI.setItalic(italic)
        self.fontI.setUnderline(underline)
        self.penI = QPen(self.color)
        
    def pen(self):
        return self.penI
    def font(self):
        return self.fontI
        
    def serialize(self, node):
        cust = (self != RefStyle(textStyles, self.styleName))
        save(node, self.styleName)
        save(node, cust)
        if cust:
            save(node, [self.color.red(), self.color.green(), self.color.blue(),
                        self.fontName,
                        self.size,
                        self.bold,
                        self.italic,
                        self.underline])
                    
    def deserialize(self, node):
        self.styleName = get(node)
        cust = get(node)
        if cust:
            red, green, blue, fontName, size, bold, italic, underline = get(node)
            self.set(QColor(red, green, blue), fontName, size, bold, italic, underline)
        else:
            src = RefStyle(textStyles, self.styleName) # use default
            self.set(src.color, src.fontName, src.size, src.bold, src.italic, src.underline)

    def __eq__(self, src):
        return (self.color == src.color and
                self.fontName == src.fontName and
                self.size == src.size and
                self.bold == src.bold and
                self.italic == src.italic and
                self.underline == src.underline)


lineStyles = {}
lineStyles['none']      = LineStyle('none', QColor(0, 0, 0), 1, Style.LINE_SOLID)
lineStyles['wire']      = LineStyle('wire', QColor(0, 128, 255), 1, Style.LINE_SOLID)
lineStyles['component'] = LineStyle('component', QColor(128, 0, 0), 1, Style.LINE_SOLID)
lineStyles['pin']       = LineStyle('pin', QColor(128, 0, 0), 1, Style.LINE_SOLID)

fillStyles = {}
fillStyles['none']      = FillStyle('none', QColor(0, 0, 0), QColor(0, 0, 0), 1, Style.LINE_SOLID, Style.BRUSH_NONE)
fillStyles['component'] = FillStyle('component', QColor(128, 0, 0), QColor(0, 0, 0), 1, Style.LINE_SOLID, Style.BRUSH_NONE)

textStyles = {}
textStyles['none']      = TextStyle('none', QColor(0, 0, 0), 'Times New Roman', 8, False, False, False)
textStyles['component'] = TextStyle('component', QColor(128, 0, 0), 'Times New Roman', 8, False, False, False)
textStyles['pin']       = TextStyle('pin', QColor(128, 0, 0), 'Times New Roman', 8, False, False, False)
textStyles['symbol']       = TextStyle('symbol', QColor(0, 132, 132), 'Times New Roman', 8, True, False, False)


def RefStyle(entry, styleName):
    style = entry.get(styleName)
    if style is not None:
        return style
    return entry['none']
 
def CreateLineStyle(styleName):
    return copy.copy(RefStyle(lineStyles, styleName))
def CreateFillStyle(styleName):
    return copy.copy(RefStyle(fillStyles, styleName))
def CreateTextStyle(styleName):
    return copy.copy(RefStyle(textStyles, styleName))
