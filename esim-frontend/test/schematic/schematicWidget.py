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

from PyQt5.QtWidgets import QOpenGLWidget, QMenu, QUndoStack
from PyQt5.QtGui import QPainter, QColor, QPen, QBrush, QSurfaceFormat
from PyQt5.QtCore import Qt
from .viewPainter import ViewPainter
from .exceptions import ExceptionSchematicId
from .element import ElementSizeType
from .elementLine import ElementLine
from .elementText import ElementText
from .elementRect import ElementRect
from .elementEllipse import ElementEllipse
from .elementArc import ElementArc
from .elementChord import ElementChord
from .elementPin import ElementPin
from .undoCommands import EditElementCommand
from util.persistence import saveIns, getIns, getObj, get
from enum import IntEnum
import json, math

SSYM_VERSION = 1000000
SCH_VERSION = 1000000

class ActionMode(IntEnum):
    MODE_SELECT = 1
    MODE_INSERT = 2

class AnchorType(IntEnum):
    ANCHOR_POINT_POINT = 0
    ANCHOR_POINT_CENTER_ANGLE = 1

class SchematicWidget(QOpenGLWidget):
    def __init__(self, symbolMode, parent=None):
        super().__init__(parent)
        
        self.dX = 0
        self.dY = 0
        self.scale = 1
        self.fontScale = 0
        self.GridX  = self.GridY  = 10
        width, height = self.schemaSize(1)
        self.schemaSizeId = 0
        self.setSchemaSize(self.schemaSizeId, 0, width, height)
        self.Frame_Text0 = ("Title")
        self.Frame_Text1 = ("Drawn By:")
        self.Frame_Text2 = ("Date:")
        self.Frame_Text3 = ("Revision:")
        self.background = Qt.white
        self.MAx1 = self.MAy1 = 0
        self.currentAnchorPoint = None
        self.pad_dR = self.GridX
        self.symbolMode = symbolMode
        
        self.setContextMenuPolicy(Qt.CustomContextMenu)

        self.postedPaintEvents = []
        self.anchorPoints = []

        self.elements = {}
        
        self.gridColor = QColor(200,200,200);
        
        self.renderSamples = 0
        if self.renderSamples > 0:
            surfaceFormat = QSurfaceFormat()
            surfaceFormat.setSamples(self.renderSamples)
            self.setFormat(surfaceFormat)
            
        #
        # Action-related attributes
        #
        self.MAx1 = self.MAy1 = 0
        self.MAx2 = self.MAy2 = 0
        self.currentElements = []
        
        self.assureSelectionMenu = QMenu(self)
        self.assureSelectionMenu.triggered.connect(self.slotAssureSelectionMenuTriggered)
        self.assureSelectId = -1
        
        self.undoStack = QUndoStack(self)
        self.oldNodeStreams = []
        self.newNodeStreams = []
        self.modified = False

        self.mode = ActionMode.MODE_SELECT
        self.mousePressAction = None
        self.mouseMoveAction = None
        self.mouseReleaseAction = None
        self.switchMode(self.mode)
    
    """
    @brief This must be called to release resources out of gc.
    """
    def close(self):
        self.mousePressAction = None
        self.mouseMoveAction = None
        self.mouseReleaseAction = None
    
    def createElement(self, classname, allocId=True, resolveSubElement=True):
        instance = None
        if classname == "line":
            instance = ElementLine(self)
        elif classname == "text":
            instance = ElementText(self)
        elif classname == "rect":
            instance = ElementRect(self)
        elif classname == "ellipse":
            instance = ElementEllipse(self)
        elif classname == "arc":
            instance = ElementArc(self)
        elif classname == 'chord':
            instance = ElementChord(self)
        elif classname == 'pin':
            instance = ElementPin(self)
        if resolveSubElement:
            instance.createSubElements(self)
        if allocId and instance is not None: # allocate global id and append to element list
            instance.id = self.__allocId()
            self.__appendElement(instance)
        return instance
        
    def __appendElement(self, instance):
        if self.elements.get(instance.id) is not None:
            raise ExceptionSchematicId()
        self.elements[instance.id] = instance # append to element list
        
    def getElement(self, id):
        return self.elements.get(id)
        
    def __allocId(self):
        freeId = len(self.elements)
        for id, element in enumerate(self.elements):
            if element is None:
                freeId = id
                break
        else:
            self.elements[freeId] = None # reserve position for the new
        return freeId

    def clear(self):
        self.undoStack.clear()
        self.elements.clear()

    """
    @brief Save the current document to JSONfile. #Exception
    """
    def save(self, stream):
        node = []
        if self.symbolMode:
            node.append(['ssym', SSYM_VERSION])
        else:
            node.append(['sch', SCH_VERSION,
                        self.schemaSizeId,
                        self.ViewX1, self.ViewY1, self.ViewX2, self.ViewY2,
                        self.Frame_Text0, self.Frame_Text1,
                        self.Frame_Text2, self.Frame_Text3])
            
        for id, element in self.elements.items():
            saveIns(node, element)
        json.dump(node, stream)

    """
    @brief Open a shcmeatic document. #Exception
    """
    def open(self, stream):
        node = json.load(stream)
        
        if self.symbolMode:
            hdr = get(node)
            if hdr[0] != 'ssym':
                raise ExceptionDocumentType
            if hdr[1] > SSYM_VERSION:
                raise ExceptionDocumentVersion
        else:
            (magic, version,
                self.schemaSizeId,
                self.ViewX1, self.ViewY1, self.ViewX2, self.ViewY2,
                self.Frame_Text0, self.Frame_Text1,
                self.Frame_Text2, self.Frame_Text3) = get(node)
            if magic != 'sch':
                raise ExceptionDocumentType
            if version > SCH_VERSION:
                raise ExceptionDocumentVersion
                
            self.setSchemaSize(self.ViewX1, self.ViewY1, self.ViewX2, self.ViewY2)
                
        self.clear()
        
        numElements = len(node)
        for i in range(numElements):
            classname, nodeMid = getIns(node)
            element = self.createElement(classname, False, False) # create instance of element
            getObj(nodeMid, element)
            self.__appendElement(element)
            
        for id, element in self.elements.items():
            element.resolveSubElements(self)
        
    def exportNodeStream(self, element):
        node = []
        saveIns(node, element)
        return node

    def importNodeStreams(self, nodes, ids):
        modElements = []
        for idx, elementId in enumerate(ids):
            created = False
            instance = self.getElement(elementId)
            if instance is None:
                instance = self.createElement(classname, False, False) # create instance of element
                created = True
            
            classname, nodeMid = getIns(nodes[idx])
            getObj(nodeMid, instance)
            modElements.append(instance)
            
            if created:
                self.__appendElement(instance)

        for element in modElements:
            element.resolveSubElements(self)
            
    """
    @brief Sets an arbitrary coordinate onto the next grid coordinate.
    """
    def togridX(self, x):
        if(x<0): x -= (self.GridX >> 1) - 1;
        else: x += self.GridX >> 1;
        x -= x % self.GridX;
        return x
        
    def togridY(self, y):
        if(y<0): y -= (self.GridY >> 1) - 1;
        else: y += self.GridY >> 1;
        y -= y % self.GridY;
        return y
        
    def togrid(self, x, y):
        return (self.togridX(x), self.togridY(y))
        
    """
    @brief Get the size of schematic.
    @param type Type of schematic size.
    @return (width, height)
    """
    def schemaSize(self, type):
        # Values exclude border of 1.5cm at each side.
        if type == 1:
            xall = 1020; yall =  765;  # DIN A5 landscape
        elif type == 2:
            xall =  765; yall = 1020;  # DIN A5 portrait
        elif type == 3:
            xall = 1530; yall = 1020;  # DIN A4 landscape
        elif type == 4:
            xall = 1020; yall = 1530;  # DIN A4 portrait
        elif type == 5:
            xall = 2295; yall = 1530;  # DIN A3 landscape
        elif type == 6:
            xall = 1530; yall = 2295;  # DIN A3 portrait
        elif type == 7:
            xall = 1414; yall = 1054;  # letter landscape
        elif type == 8:
            xall = 1054; yall = 1414;  # letter portrait
        else:
            return (0, 0)
        return (xall, yall)
        
    def setSchemaSize(self, x1, y1, x2, y2):
        self.ViewX1, self.ViewY1 = x1, y1
        self.ViewX2, self.ViewY2 = x2, y2
        self.setFixedSize(self.mapAbsW(self.schemaWidth()), self.mapAbsH(self.schemaHeight()))

    """
    Paint dot grid of the schematic.
    """
    def __paintGrid(self, p, cX, cY, width, height):
        p.painter.setPen(QPen(self.gridColor, 1))

        x1  = cX / self.scale + self.ViewX1
        y1  = cY / self.scale + self.ViewY1

        (x1, y1) = self.togrid(x1, y1);
        if (x1<0): x1 -= self.GridX - 1
        else: x1 += self.GridX
        x1 -= x1 % (self.GridX << 1)

        if(y1<0): y1 -= self.GridY - 1
        else: y1 += self.GridY
        y1 -= y1 % (self.GridY << 1)

        X = x1 * self.scale + p.dX
        Y = Y0 = y1 * self.scale + p.dY
        x1 = (X + 0.5) if X > 0.0 else (X - 0.5)
        y1 = (Y + 0.5) if Y > 0.0 else (Y - 0.5)
 
        dx = self.mapAbsX(self.togridX(self.schemaWidth()/2 - self.ViewX1))
        dy = self.mapAbsY(self.togridY(self.schemaHeight()/2 - self.ViewY1))
        
        if self.symbolMode:
            schWidthAbs = self.mapAbsW(self.schemaWidth())
            schHeightAbs = self.mapAbsW(self.schemaHeight())
            p.painter.drawLine(0, dy, schWidthAbs, dy) # origin cross
            p.painter.drawLine( dx,0, dx, schHeightAbs)
        else:
            p.painter.drawLine(-3+dx, dy, 3+dx, dy) # small cross at origin
            p.painter.drawLine( dx,-3+dy, dx, 3+dy)
        
        xEnd = x1 + width;
        yEnd = y1 + height;
        DX = (self.GridX << 1) * self.scale;   # every second grid a point
        DY = (self.GridY << 1) * self.scale;
        while(DX <= 8.0):  DX *= 1.5;  # if too narrow, every third grid a point
        while(DY <= 8.0):  DY *= 1.5;  # if too narrow, every third grid a point

        while(x1 < xEnd):
            Y = Y0;
            y1 = Y + 0.5 if Y > 0.0 else Y - 0.5;
            while (y1 < yEnd):
                p.painter.drawPoint(x1, y1);    # paint grid
                Y += DY;
                y1 =  Y + 0.5 if Y > 0.0 else Y - 0.5;
            X += DX;
            x1 = (X + 0.5) if X > 0.0 else (X - 0.5);


    """
    Paint the document frame.
    """
    def __paintFrame(self, p):
        if self.symbolMode:
            return
        
        # dimensions:  X cm / 2.54 * 144
        xall = self.schemaWidth()
        yall = self.schemaHeight()

        p.painter.setPen(QPen(Qt.darkGray, 1));

        d = p.lineSpacing + int(4.0 * p.scale);
        (x1_, y1_) = p.map(xall, yall);
        x2_ = int(xall * p.scale) + 1;
        y2_ = int(yall * p.scale) + 1;
        p.painter.drawRect(x1_, y1_, -x2_, -y2_);
        p.painter.drawRect(x1_-d, y1_-d, 2*d-x2_, 2*d-y2_);

        step = xall // ((xall+127) // 255);
        for z in range(step, xall-step+1, step):
            (x2_, y2_) = p.map(z, 0);
            p.painter.drawLine(x2_, y2_, x2_, y2_+d);
            p.painter.drawLine(x2_, y1_-d, x2_, y1_);
        
        Letter = ord('1');
        for z in range(step//2+5, xall, step):
            (x2_, y2_) = p.map(z, 3);
            p.painter.drawText(x2_, y2_, 0, 0, Qt.TextDontClip, chr(Letter));
            (x2_, y2_) = p.map(z, yall+3);
            p.painter.drawText(x2_, y2_-d, 0, 0, Qt.TextDontClip, chr(Letter));
            Letter+=1;
            
        step = yall // ((yall+127) // 255);
        for z in range(step, yall-step+1, step):
            (x2_, y2_) = p.map(0, z);
            p.painter.drawLine(x2_, y2_, x2_+d, y2_);
            p.painter.drawLine(x1_-d, y2_, x1_, y2_);
            
        Letter = ord('A');
        for z in range(step//2+5, yall, step):
            (x2_, y2_) = p.map(5, z);
            p.painter.drawText(x2_, y2_, 0, 0, Qt.TextDontClip, chr(Letter));
            (x2_, y2_) = p.map(xall+5, z);
            p.painter.drawText(x2_-d, y2_, 0, 0, Qt.TextDontClip, chr(Letter));
            Letter+=1;

        # draw text box with text
        (x1_, y1_) = p.map(xall-340, yall-3);
        (x2_, y2_) = p.map(xall-3,   yall-3);
        x1_ -= d;  x2_ -= d;
        y1_ -= d;  y2_ -= d;
        d = int(6.0 * p.scale);
        z = int(200.0 * p.scale);
        y1_ -= p.lineSpacing + d;
        p.painter.drawLine(x1_, y1_, x2_, y1_);
        p.painter.drawText(x1_+d, y1_+(d>>1), 0, 0, Qt.TextDontClip, self.Frame_Text2);
        p.painter.drawLine(x1_+z, y1_, x1_+z, y1_ + p.lineSpacing+d);
        p.painter.drawText(x1_+d+z, y1_+(d>>1), 0, 0, Qt.TextDontClip, self.Frame_Text3);
        y1_ -= p.lineSpacing + d;
        p.painter.drawLine(x1_, y1_, x2_, y1_);
        p.painter.drawText(x1_+d, y1_+(d>>1), 0, 0, Qt.TextDontClip, self.Frame_Text1);
        y1_ -= (self.Frame_Text0.count('\n')+1) * p.lineSpacing + d;
        p.painter.drawRect(x2_, y2_, x1_-x2_-1, y1_-y2_-1);
        p.painter.drawText(x1_+d, y1_+(d>>1), 0, 0, Qt.TextDontClip, self.Frame_Text0);

    def __paintEvents(self, p):
        for event in self.postedPaintEvents:
            (op, x1, y1, a1, a2, a, b) = event
            x1 = self.mapAbsX(x1); y1 = self.mapAbsY(y1);
            
            if op == 0: # Rect
                p.painter.drawRect(x1, y1, self.mapAbsW(a1), self.mapAbsW(a2));
            elif op == 1: # Dot Rect
                pen = p.painter.pen()
                p.painter.setPen(Qt.DotLine);
                p.painter.drawRect(x1, y1, self.mapAbsW(a1), self.mapAbsW(a2));
                p.painter.setPen(pen)
            elif op == 2: # Line
                y1 = self.mapAbsY(a1); y2 = self.mapAbsY(a2);
                p.painter.drawLine(x1, y1, x2, y2);
            elif op == 3: # Ellipse
                p.painter.drawEllipse(x1, y1, self.mapAbsW(a1), self.mapAbsW(a2));
            elif op == 4: # Arc
                p.painter.drawArc(x1, y1, self.mapAbsW(a1), self.mapAbsW(a2), a, b);

    """
    @brief Post event of painting. Recv related position.
    """
    def postPaintEvent(self, op, x1, y1, a1, a2, update=False, a=0, b=0):
        event = (op, x1, y1, a1, a2, a, b)
        self.postedPaintEvents.append(event)
        if update: self.update()
            
    def clearPaintEvent(self, update=False):
        self.postedPaintEvents.clear()
        if update: self.update()

    
    """
    @brief Select elements in specified rectangle area.
    """
    def selectElements(self, x1, y1, x2, y2, update=False):
        # exchange rectangle coordinates to obtain x1 < x2 and y1 < y2
        cx1 = x1 if (x1 < x2) else x2
        cx2 = x1 if (x1 > x2) else x2
        cy1 = y1 if (y1 < y2) else y2
        cy2 = y1 if (y1 > y2) else y2
        x1 = cx1; x2 = cx2;
        y1 = cy1; y2 = cy2;
        
        for id, element in self.elements.items():
            if element.modifiable():
                (cx1, cy1, cx2, cy2) = element.bounding()
                if(cx1 >= x1):
                    if(cx2 <= x2):
                        if(cy1 >= y1):
                            if(cy2 <= y2):
                                element.selected = True
        if update: self.update()
   
    def selectElement(self, id, update=False):
        self.elements[id].selected = True
        if update: self.update()
   
    """
    @brief get all the elements that is located at the specified point
    """
    def getElememntsByPos(self, x, y, result):
        result.clear()
        for id, element in self.elements.items():
            if element.modifiable():
                (x1, y1, x2, y2) = element.bounding()
                cx1 = x1 if (x1 < x2) else x2
                cx2 = x1 if (x1 > x2) else x2
                cy1 = y1 if (y1 < y2) else y2
                cy2 = y1 if (y1 > y2) else y2
                
                if(cx1 <= x):
                    if(cx2 >= x):
                        if(cy1 <= y):
                            if(cy2 >= y):
                                result.append(element)
        
   
    def getSelectedElements(self, result):
        result.clear()
        for id, element in self.elements.items():
            if element.selected:
                result.append(element)
   
    """
    @brief Clear flags of all the selected elements
    """
    def clearSelectedElements(self, update=False):
        for id, element in self.elements.items():
            element.selected = False
        if update: self.update()
   
    """
    @brief Get the string of element position
    @return strings
    """
    def elementPosString(self, elementIns):
        sizeType = elementIns.sizeType()
        if sizeType & ElementSizeType.LINE_2P:
            return "(%d, %d)-(%d, %d)" % (elementIns.x1, elementIns.y1, elementIns.x2, elementIns.y2)
        elif sizeType & ElementSizeType.RECT_1P:
            return "(%d, %d)" % (elementIns.x1, elementIns.y1)
        elif sizeType & ElementSizeType.RECT_4P:
            return "(%d, %d)-(%d, %d)" % (elementIns.x1, elementIns.y1, elementIns.x2, elementIns.y2)
        else:
            return ''
    
    """
    @brief Get origin point of element
    @return (x,y)
    """
    def elementOrigin(self, elementIns):
        (x1, y1, x2, y2) = elementIns.bounding()
        
        return ((x1 + x2)/2, (y1 + y2)/2)
    
    def schemaWidth(self):
        return abs(self.ViewX2 - self.ViewX1)
    def schemaHeight(self):
        return abs(self.ViewY2 - self.ViewY1)
        
    """
    @brief Map the relative position to absoulte position in our coordinate system.
    """
    def map(self, x1, y1):
        return ((x1 - self.dX) / self.scale, (y1 - self.dY) / self.scale)
    def mapX(self, x1):
        return (x1 - self.dX) / self.scale
    def mapY(self, y1):
        return (y1 - self.dY) / self.scale
    def mapW(self, w):
        return w / self.scale
    def mapH(self, h):
        return h / self.scale
        
    def mapAbsX(self, x1):
        return x1 * self.scale + self.dX
    def mapAbsY(self, y1):
        return y1 * self.scale + self.dY
    def mapAbsW(self, w):
        return w * self.scale
    def mapAbsH(self, h):
        return h * self.scale
        
    def setMousePressAction(self, action):
        self.mousePressAction = action
    def setMouseMoveAction(self, action):
        self.mouseMoveAction = action
    def setMouseReleaseAction(self, action):
        self.mouseReleaseAction = action
    
    """
    Helper function of __appendAnchorPoint()
    @param type 0 = Blue rectangle pad.
        1 = Red ellipse filled pad.
        2 = Green ellipse pad.
    """
    def __paintAnchorPoint(self, p, type, x1, y1):
        pen = p.painter.pen()
        if type == 0:
            p.painter.setPen(QColor(0, 0, 128))
            p.painter.drawRect(x1-5, y1-5, 10, 10)
        elif type == 1:
            p.painter.setPen(QColor(128, 0, 0))
            p.painter.drawEllipse(x1-5, y1-5, 10, 10)
        elif type == 2:
            p.painter.setPen(QColor(0, 128, 0));
            p.painter.drawEllipse(x1-5, y1-5, 10, 10)
        p.painter.setPen(pen)
       
    def __appendAnchorPoint(self, p, element, anchorType, numPoint):
        if anchorType == AnchorType.ANCHOR_POINT_POINT: # pad for point resizing
            if numPoint == 2:
                x1 = element.x1; x2 = element.x2;
                y1 = element.y1; y2 = element.y2;
                cx1 = self.mapAbsX(x1); cx2 = self.mapAbsX(x2);
                cy1 = self.mapAbsY(y1); cy2 = self.mapAbsY(y2);
                
                self.__paintAnchorPoint(p, 0, cx1, cy1)
                self.__paintAnchorPoint(p, 0, cx2, cy2)
                self.anchorPoints.append([element, 0, 0, x1, y1])
                self.anchorPoints.append([element, 0, 1, x2, y2])
                
            elif numPoint == 4:
                x1 = element.x1; x2 = element.x2;
                y1 = element.y1; y2 = element.y2;
                cx1 = self.mapAbsX(x1); cx2 = self.mapAbsX(x2);
                cy1 = self.mapAbsY(y1); cy2 = self.mapAbsY(y2);
                
                self.__paintAnchorPoint(p, 0, cx1, cy1)
                self.__paintAnchorPoint(p, 0, cx2, cy1)
                self.__paintAnchorPoint(p, 0, cx1, cy2)
                self.__paintAnchorPoint(p, 0, cx2, cy2)
                self.anchorPoints.append([element, anchorType, 0, x1, y1])
                self.anchorPoints.append([element, anchorType, 1, x2, y1])
                self.anchorPoints.append([element, anchorType, 2, x1, y2])
                self.anchorPoints.append([element, anchorType, 3, x2, y2])
                
        elif anchorType == AnchorType.ANCHOR_POINT_CENTER_ANGLE: # pad for arc resizing
            (x1, y1, x2, y2) = element.bounding()
            cx, cy = (x1+x2)/2, (y1+y2)/2
            cx = self.mapAbsX(cx); cy = self.mapAbsY(cy);
            p.painter.drawLine(-3+cx, cy, 3+cx, cy) # small cross at origin
            p.painter.drawLine( cx,-3+cy, cx, 3+cy)
            
            rX = self.mapAbsW( abs(x2 - x1) / 2 + self.pad_dR )
            rY = self.mapAbsH( abs(y2 - y1) / 2 + self.pad_dR )

            if numPoint == 2:
                startAngle = element.a1 / 16 / 180 * math.pi
                sX = cx + rX * math.cos(-startAngle)
                sY = cy + rY * math.sin(-startAngle)
                self.__paintAnchorPoint(p, 1, sX, sY)
                self.anchorPoints.append([element, anchorType, 0, self.mapX(sX), self.mapY(sY)])

                spanAngle = element.a2 / 16 / 180 * math.pi
                eX = cx + rX * math.cos(-startAngle -spanAngle)
                eY = cy + rY * math.sin(-startAngle - spanAngle)
                self.__paintAnchorPoint(p, 2, eX, eY)
                self.anchorPoints.append([element, anchorType, 1, self.mapX(eX), self.mapY(eY)])

                pen = p.painter.pen()
                p.painter.setPen(QPen(QColor(0, 128, 0), 1, Qt.DotLine));
                p.painter.drawLine(cx, cy, sX, sY);
                p.painter.drawLine(cx, cy, eX, eY);
                p.painter.setPen(pen)
                
    
    """
    Moving the anchor point
    """
    def __moveElementPosPoint(self, element, point, dX, dY):
        sizeType = element.sizeType()
        if sizeType & ElementSizeType.LINE_2P:
            if point == 0:
                element.setPos( element.x1 + dX, element.y1 + dY, element.x2, element.y2 )
            elif point == 1:
                element.setPos( element.x1, element.y1, element.x2 + dX, element.y2 + dY )
                
        elif sizeType & ElementSizeType.RECT_4P:
            if point == 0:
                element.setPos( element.x1 + dX, element.y1 + dY, element.x2, element.y2 )
            elif point == 1:
                element.setPos( element.x1, element.y1 + dY, element.x2 + dX, element.y2 )
            elif point == 2:
                element.setPos( element.x1 + dX, element.y1, element.x2, element.y2 + dY )
            elif point == 3:
                element.setPos( element.x1, element.y1, element.x2 + dX, element.y2 + dY )
                
        self.update()
    
    def __moveElementAnglePoint(self, element, point, cx, cy):
        (x, y) = self.elementOrigin(element)
        dX = cx - x
        dY = cy - y
        A = math.atan2(dY, dX) * 180 / math.pi
        if (A > 360):
            A -= 360
        if (A < 0):
            A += 360
        A = -A * 16
        if point == 0:
            element.setAngle(A, element.a2)
        elif point == 1:
            element.setAngle(element.a1, A-element.a1)
            
        self.update()
    
    #@Override
    def paintEvent(self, event):
        painter = QPainter(self)
        p = ViewPainter(painter, self.scale, self.dX, self.dY, self.fontScale)

        rect = event.rect()
        cx = self.mapX(rect.left())
        cy = self.mapY(rect.top())
        width = self.mapW(rect.width())
        height = self.mapH(rect.height())
        
        cxWhole = self.mapAbsX(0)
        cyWhole = self.mapAbsY(0);
        widthWhole = self.mapAbsW(self.schemaWidth());
        heightWhole = self.mapAbsH(self.schemaHeight());
        
        p.painter.fillRect(cxWhole, cyWhole, widthWhole, heightWhole, QBrush(self.background))

        self.__paintGrid(p, 0, 0, widthWhole, heightWhole)

        self.__paintFrame(p)
        self.__paintEvents(p)

        self.anchorPoints.clear()
        
        for id, element in self.elements.items():
            element.render(p);
                            
            if element.selected:
                (x1, y1, x2, y2) = element.bounding()
                x1 = self.mapAbsX(x1); x2 = self.mapAbsX(x2)
                y1 = self.mapAbsY(y1); y2 = self.mapAbsY(y2)
                cx1 = x1 if (x1 < x2) else x2
                cx2 = x1 if (x1 > x2) else x2
                cy1 = y1 if (y1 < y2) else y2
                cy2 = y1 if (y1 > y2) else y2

                width = abs(cx2-cx1)+1
                height = abs(cy2-cy1)+1
                
                # Draw selection rectangle
                if width > 0 and height > 0:
                    pen = p.painter.pen()
                    p.painter.setPen(Qt.DotLine);
                    p.painter.drawRect(cx1, cy1, width, height);
                    p.painter.setPen(pen)
                
                # Draw anchor points for modifiable element
                if element.modifiable():
                    sizeType = element.sizeType()
                    if sizeType & ElementSizeType.LINE_2P:
                        self.__appendAnchorPoint(p, element, AnchorType.ANCHOR_POINT_POINT, 2)
                        
                    elif sizeType & ElementSizeType.RECT_4P:
                        self.__appendAnchorPoint(p, element, AnchorType.ANCHOR_POINT_POINT, 4)
                        
                    if sizeType & ElementSizeType.ANGLE_2A:
                        self.__appendAnchorPoint(p, element, AnchorType.ANCHOR_POINT_CENTER_ANGLE, 2)
                        
    #@Override
    def mousePressEvent(self, event):
        cx = self.mapX(event.pos().x())
        cy = self.mapY(event.pos().y())

        self.currentAnchorPoint = None
        for anchor in self.anchorPoints: # is the target an anchor point ?
            if (anchor[3]-5 <= cx):
                if (anchor[3]+5 >= cx):
                    if (anchor[4]-5 <= cy):
                        if (anchor[4]+5 >= cy):
                            self.MAx1 = self.togridX(cx)
                            self.MAy1 = self.togridY(cy)
                            self.currentAnchorPoint = anchor
                            break
        else:
            if (self.mousePressAction is not None):
                self.mousePressAction(event)
    
    #@Override
    def mouseMoveEvent(self, event):
        if self.currentAnchorPoint is not None: # Moving anchor point
            cx = self.togridX(self.mapX(event.pos().x()))
            cy = self.togridY(self.mapY(event.pos().y()))
            dX = cx - self.MAx1
            dY = cy - self.MAy1
            self.MAx1 = cx
            self.MAy1 = cy
            anchorType = self.currentAnchorPoint[1]
            if anchorType == AnchorType.ANCHOR_POINT_POINT:
                self.__moveElementPosPoint(self.currentAnchorPoint[0], self.currentAnchorPoint[2], dX, dY)
            elif anchorType == AnchorType.ANCHOR_POINT_CENTER_ANGLE:
                self.__moveElementAnglePoint(self.currentAnchorPoint[0], self.currentAnchorPoint[2],
                                                self.mapX(event.pos().x()), self.mapY(event.pos().y()))
                    
        else:
            if (self.mouseMoveAction is not None):
                self.mouseMoveAction(event)
            
    #@Override
    def mouseReleaseEvent(self, event):
        self.currentAnchorPoint = None
        if (self.mouseReleaseAction is not None):
            self.mouseReleaseAction(event)
            

    #****************************************************************
    # Actions management
    #****************************************************************
    
    def switchMode(self, mode):
        if mode == ActionMode.MODE_SELECT:
            self.setMousePressAction(self.MousePressSelect)
            self.setMouseMoveAction(None)
            self.setMouseReleaseAction(None)
            
        elif mode == ActionMode.MODE_INSERT:
            self.setMousePressAction(self.MousePressInsert)
            self.setMouseMoveAction(None)
            self.setMouseReleaseAction(None)
        
    def getUndoStack(self):
        return self.undoStack
        
    """
    @brief Call before begining element editing.
    """
    def __beginElementEdit(self, targets):
        self.oldNodeStreams.clear()
        for element in targets:
            self.oldNodeStreams.append( self.exportNodeStream(element) )
        self.modified = False
    """
    @brief Call after ended element editing, which creates a restore point for undo.
            Note that this only makes sense when self.modified is True
    """
    def __endElementEdit(self, targets):
        if self.modified:
            self.newNodeStreams.clear()
            for element in targets:
                self.newNodeStreams.append( self.exportNodeStream(element) )

            command = EditElementCommand(self, self.currentElements, self.oldNodeStreams, self.newNodeStreams)
            self.undoStack.push(command)
            
            self.modified = False
        
    #****************************************************************
    # Mouse actions for element selection
    #****************************************************************
    
    def MousePressSelect(self, event):
        self.MAx1 = self.mapX(event.pos().x())
        self.MAy1 = self.mapY(event.pos().y())
        
        curSelectedElements = []
        self.currentElements.clear()
        
        self.getElememntsByPos(self.MAx1, self.MAy1, curSelectedElements)
        self.getSelectedElements(self.currentElements)
        
        pressElement = None

        if len(curSelectedElements) == 1:
            pressElement = curSelectedElements[0]
        elif len(curSelectedElements) > 1:
            if len(self.currentElements) == 0: # ask for selection assurance
                self.assureSelectionMenu.clear()
                action = self.assureSelectionMenu.addAction("Clear choice")
                action.setEnabled(False)
                self.assureSelectionMenu.addSeparator()

                for idx, element in enumerate(curSelectedElements):
                    title = self.elementPosString(element) + ': ' + element.classname()
                    action = self.assureSelectionMenu.addAction(title)
                    action.setData(idx)

                self.assureSelectId = -1
                self.assureSelectionMenu.exec(event.globalPos())
                if self.assureSelectId != -1:
                    element = curSelectedElements[self.assureSelectId]
                    
                    cx1, cy1 = self.elementOrigin(element)
                    QCursor.setPos(self.mapToGlobal(QPoint(self.mapAbsX(cx1), self.mapAbsY(cy1)))) # goto in view
                    
                    pressElement = element
            else:
                pressElement = self.currentElements[0]
        
        #
        # Status switching of editor
        #
        doSelection = False
        
        if pressElement is not None:
            if len(self.currentElements) == 0:
                self.selectElement(pressElement.id, update=True) # do selection for selection assurance
            else:
                doSelection = True
                for element in curSelectedElements:
                    if element in self.currentElements:
                        # selected an(some) element(s), in consequence cancel the selection mode
                        # and switch to moving mode.
                        doSelection = False
                        break
                
                # --- Enter to element moving mode ---
                if not doSelection:
                    self.__beginElementEdit(self.currentElements)
                    self.MAx1 = self.togridX(self.mapX(event.pos().x()))
                    self.MAy1 = self.togridY(self.mapY(event.pos().y()))
                    self.setMouseMoveAction(self.MouseMoveElementMoving)
                    self.setMouseReleaseAction(self.MouseReleaseElementMoving)
        else:
            doSelection = True
        
        # --- Enter to free selection mode ---
        if doSelection: # open a rectangle
            self.MAx2 = 0
            self.MAy2 = 0
            self.setMouseMoveAction(self.MouseMoveSelect)
            self.setMouseReleaseAction(self.MouseReleaseSelect)
            
            self.clearSelectedElements(update=True)

    def MouseMoveSelect(self, event):
        self.MAx2 = self.mapX(event.pos().x()) - self.MAx1
        self.MAy2 = self.mapY(event.pos().y()) - self.MAy1
        
        self.clearPaintEvent()
        self.postPaintEvent (1, self.MAx1, self.MAy1, self.MAx2, self.MAy2, update=True)
        
    def MouseReleaseSelect(self, event):
        # selects all elements within the rectangle
        self.selectElements(self.MAx1, self.MAy1, self.MAx1 + self.MAx2, self.MAy1 + self.MAy2)
        self.clearPaintEvent(update=True)
        
        self.switchMode(ActionMode.MODE_SELECT)
        
    def slotAssureSelectionMenuTriggered(self, action):
        self.assureSelectId = int(action.data())
        
    
    #****************************************************************
    # Mouse actions for element moving
    #****************************************************************
    
    def MouseMoveElementMoving(self, event):
        cx = self.togridX(self.mapX(event.pos().x()))
        cy = self.togridY(self.mapY(event.pos().y()))
        dX = cx - self.MAx1
        dY = cy - self.MAy1
        self.MAx1 = cx
        self.MAy1 = cy
        
        for element in self.currentElements:
            sizeType = element.sizeType()
            if sizeType & ElementSizeType.LINE_2P:
                element.setPos( element.x1 + dX, element.y1 + dY, element.x2 + dX, element.y2 + dY )
            elif sizeType & ElementSizeType.RECT_1P:
                element.setPos( element.x1 + dX, element.y1 + dY )
            elif sizeType & ElementSizeType.RECT_4P:
                element.setPos( element.x1 + dX, element.y1 + dY, element.x2 + dX, element.y2 + dY )

        self.modified = True
        self.update()

    def MouseReleaseElementMoving(self, event):
        self.__endElementEdit(self.currentElements)
        self.switchMode(ActionMode.MODE_SELECT)
