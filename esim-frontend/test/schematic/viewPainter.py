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

from PyQt5.QtGui import QPainter, QTransform
from PyQt5.QtCore import Qt, QPointF, QLineF, QRectF
from .styleTemplate import Style

GraphAntiAliasing = True
TextAntiAliasing = True

class ViewPainter():
    def __init__(self, painter, scale, dX, dY, fontScale):
        self.painter = painter
        
        self.dX = dX * scale
        self.dY = dY * scale
        self.scale = scale

        f = painter.font()
        if (fontScale == 0.0):
            fontScale = scale
        f.setPointSizeF( fontScale * float(f.pointSize()) )
        painter.setFont(f)
        self.lineSpacing = painter.fontMetrics().lineSpacing()
        painter.setWorldMatrixEnabled(False)   # we use our own coordinate transformation

        hints = 0;
        # Enable antialias drawings if requested
        if (GraphAntiAliasing): hints = hints or QPainter.Antialiasing
        # Enable antialias text if requested
        if (TextAntiAliasing): hints = hints or QPainter.TextAntialiasing
        painter.setRenderHints(hints)

    """
    @brief Map the relative position to absoulte position in our coordinate system.
    """
    def map(self, x1, y1):
        z = x1 * self.scale + self.dX
        x = z
        z = y1 * self.scale + self.dY
        y = z
        return (x,y)
    
    def setLineStyle(self, style):
        self.painter.setPen(style.pen())
    def setFillStyle(self, style):
        self.painter.setPen(style.pen())
        self.painter.setBrush(style.brush())
    def setTextStyle(self, style):
        self.painter.setPen(style.pen())
        self.painter.setFont(style.font())
        
    def lineContext(self):
        return [self.painter.pen()]
    def setLineContext(self, context):
        self.painter.setPen(context[0])
    def fillContext(self):
        return (self.painter.pen(), self.painter.brush())
    def setFillContext(self, context):
        self.painter.setPen(context[0])
        self.painter.setBrush(context[1])
    def textContext(self):
        return (self.painter.pen(), self.painter.font())
    def setTextContext(self, context):
        self.painter.setPen(context[0])
        self.painter.setFont(context[1])
        
    """
    @brief Draw a point on painter.
    """
    def point(self, x, y):
        xr = x * self.scale + self.dX
        yr = y * self.scale + self.dY

        self.painter.drawPoint(QPointF(xr, yr))
    
    """
    @brief Draw a line on painter.
    """
    def line(self, x1, y1, x2, y2):
        x1r = x1 * self.scale + self.dX
        y1r = y1 * self.scale + self.dY
        x2r = x2 * self.scale + self.dX
        y2r = y2 * self.scale + self.dY
        
        self.painter.drawLine(QLineF(x1r, y1r, x2r, y2r))
    
    """
    @brief Draw a rect on painter.
    """
    def rect(self, x, y, dx, dy):
        xr = x * self.scale + self.dX
        yr = y * self.scale + self.dY
        dxr = dx * self.scale
        dyr = dy * self.scale
        
        self.painter.drawRect(QRectF(xr, yr, dxr, dyr))
    
    """
    @brief Draw an ellipse on painter.
    """
    def ellipse(self, x, y, dx, dy):
        xr = x * self.scale + self.dX
        yr = y * self.scale + self.dY
        dxr = dx * self.scale
        dyr = dy * self.scale
        
        self.painter.drawEllipse(QRectF(xr, yr, dxr, dyr))
    
    """
    @brief Draw an arc on painter.
    """
    def arc(self, x, y, dx, dy, startAngle, spanAngle):
        xr = x * self.scale + self.dX
        yr = y * self.scale + self.dY
        dxr = dx * self.scale
        dyr = dy * self.scale
        
        self.painter.drawArc(QRectF(xr, yr, dxr, dyr), startAngle, spanAngle)
    
    """
    @brief Draw a chord on painter.
    """
    def chord(self, x, y, dx, dy, startAngle, spanAngle):
        xr = x * self.scale + self.dX
        yr = y * self.scale + self.dY
        dxr = dx * self.scale
        dyr = dy * self.scale
        
        self.painter.drawChord(QRectF(xr, yr, dxr, dyr), startAngle, spanAngle)
    
    """
    @brief Draw a text on painter.
    @return (Width, Height) of the text.
    """
    def text(self, text, x, y, angle=0):
        x1 = x * self.scale + self.dX
        y1 = y * self.scale + self.dY
        rf = self.painter.boundingRect(QRectF(x1, y1, 0, 0), Qt.TextDontClip, text)
        
        self.painter.save()

        if angle != 180:
            self.painter.translate(x1, y1)
            self.painter.scale(self.scale, self.scale)
            self.painter.rotate(angle)
        else:
            self.painter.translate(x1 - rf.width(), y1)
            self.painter.scale(self.scale, self.scale)
            self.painter.rotate(0)
        
        self.painter.drawText(QRectF(0, 0, 0, 0), Qt.TextDontClip, text)

        self.painter.restore();
        
        return (rf.width(), rf.height())
    