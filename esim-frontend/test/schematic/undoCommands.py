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

from PyQt5.QtCore import Qt, QPoint
from PyQt5.QtWidgets import QAction, QMenu, QUndoCommand, QUndoStack
from PyQt5.QtGui import QCursor
from .element import ElementSizeType
from util.persistence import saveIns, getIns, getObj
from enum import IntEnum
import copy

class ActionMode(IntEnum):
    MODE_SELECT = 1
    MODE_INSERT = 2

class EditElementCommand(QUndoCommand):
    def __init__(self, doc, elements, oldNodeStreams, newNodeStreams, parent=None):
        super().__init__(parent)
        self.doc = doc
        self.elementIds = []
        for element in elements:
            self.elementIds.append(element.id)
            
        self.oldNodeStreams = copy.deepcopy(oldNodeStreams)
        self.newNodeStreams = copy.deepcopy(newNodeStreams)
        
    # @Override
    def undo(self):
        node = copy.deepcopy(self.oldNodeStreams)
        self.doc.importNodeStreams(node, self.elementIds)
        self.doc.update()
    
    # @Override
    def redo(self):
        node = copy.deepcopy(self.newNodeStreams)
        self.doc.importNodeStreams(node, self.elementIds)
        self.doc.update()

class SchematicActions():
    def __init__(self, doc):
        self.doc = doc
        self.MAx1 = self.MAy1 = 0
        self.MAx2 = self.MAy2 = 0
        self.currentElements = []
        
        self.assureSelectionMenu = QMenu(doc)
        self.assureSelectionMenu.triggered.connect(self.slotAssureSelectionMenuTriggered)
        self.assureSelectId = -1
        
        self.undoStack = QUndoStack(doc)
        self.oldNodeStreams = []
        self.newNodeStreams = []
        self.modified = False

        self.mode = ActionMode.MODE_SELECT
        self.doc.setMouseReleaseAction(None)
        self.doc.setMouseMoveAction(None)
        self.switchMode(self.mode)

    def switchMode(self, mode):
        if mode == ActionMode.MODE_SELECT:
            self.doc.setMousePressAction(self.MousePressSelect)
            self.doc.setMouseMoveAction(None)
            self.doc.setMouseReleaseAction(None)
            
        elif mode == ActionMode.MODE_INSERT:
            self.doc.setMousePressAction(self.MousePressInsert)
            self.doc.setMouseMoveAction(None)
            self.doc.setMouseReleaseAction(None)
        
    def getUndoStack(self):
        return self.undoStack

    """
    @brief Call before begining element editing.
    """
    def __beginElementEdit(self, targets):
        self.oldNodeStreams.clear()
        for element in targets:
            self.oldNodeStreams.append( self.doc.exportNodeStream(element) )
        self.modified = False
    """
    @brief Call after ended element editing, which creates a restore point for undo.
            Note that this only makes sense when self.modified is True
    """
    def __endElementEdit(self, targets):
        if self.modified:
            self.newNodeStreams.clear()
            for element in targets:
                self.newNodeStreams.append( self.doc.exportNodeStream(element) )

            command = EditElementCommand(self.doc, self.currentElements, self.oldNodeStreams, self.newNodeStreams)
            self.undoStack.push(command)
            
            self.modified = False
        
    #****************************************************************
    # Mouse actions for element selection
    #****************************************************************
    
    def MousePressSelect(self, event):
        self.MAx1 = self.doc.mapX(event.pos().x())
        self.MAy1 = self.doc.mapY(event.pos().y())
        
        curSelectedElements = []
        self.currentElements.clear()
        
        self.doc.getElememntsByPos(self.MAx1, self.MAy1, curSelectedElements)
        self.doc.getSelectedElements(self.currentElements)
        
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
                    title = self.doc.elementPosString(element) + ': ' + element.classname()
                    action = self.assureSelectionMenu.addAction(title)
                    action.setData(idx)

                self.assureSelectId = -1
                self.assureSelectionMenu.exec(event.globalPos())
                if self.assureSelectId != -1:
                    element = curSelectedElements[self.assureSelectId]
                    
                    cx1, cy1 = self.doc.elementOrigin(element)
                    QCursor.setPos(self.doc.mapToGlobal(QPoint(self.doc.mapAbsX(cx1), self.doc.mapAbsY(cy1)))) # goto in view
                    
                    pressElement = element
            else:
                pressElement = self.currentElements[0]
        
        #
        # Status switching of editor
        #
        doSelection = False
        
        if pressElement is not None:
            if len(self.currentElements) == 0:
                self.doc.selectElement(pressElement.id, update=True) # do selection for selection assurance
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
                    self.MAx1 = self.doc.togridX(self.doc.mapX(event.pos().x()))
                    self.MAy1 = self.doc.togridY(self.doc.mapY(event.pos().y()))
                    self.doc.setMouseMoveAction(self.MouseMoveElementMoving)
                    self.doc.setMouseReleaseAction(self.MouseReleaseElementMoving)
        else:
            doSelection = True
        
        # --- Enter to free selection mode ---
        if doSelection: # open a rectangle
            self.MAx2 = 0
            self.MAy2 = 0
            self.doc.setMouseMoveAction(self.MouseMoveSelect)
            self.doc.setMouseReleaseAction(self.MouseReleaseSelect)
            
            self.doc.clearSelectedElements(update=True)

    def MouseMoveSelect(self, event):
        self.MAx2 = self.doc.mapX(event.pos().x()) - self.MAx1
        self.MAy2 = self.doc.mapY(event.pos().y()) - self.MAy1
        
        self.doc.clearPaintEvent()
        self.doc.postPaintEvent (1, self.MAx1, self.MAy1, self.MAx2, self.MAy2, update=True)
        
    def MouseReleaseSelect(self, event):
        # selects all elements within the rectangle
        self.doc.selectElements(self.MAx1, self.MAy1, self.MAx1 + self.MAx2, self.MAy1 + self.MAy2)
        self.doc.clearPaintEvent(update=True)
        
        self.switchMode(ActionMode.MODE_SELECT)
        
    def slotAssureSelectionMenuTriggered(self, action):
        self.assureSelectId = int(action.data())
        
    
    #****************************************************************
    # Mouse actions for element moving
    #****************************************************************
    
    def MouseMoveElementMoving(self, event):
        cx = self.doc.togridX(self.doc.mapX(event.pos().x()))
        cy = self.doc.togridY(self.doc.mapY(event.pos().y()))
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
        self.doc.update()
        
    def MouseReleaseElementMoving(self, event):
        self.__endElementEdit(self.currentElements)
        self.switchMode(ActionMode.MODE_SELECT)
        