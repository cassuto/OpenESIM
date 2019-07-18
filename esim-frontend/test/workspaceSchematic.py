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

from PyQt5.QtWidgets import QAction, QActionGroup, QToolBar, QScrollArea
from PyQt5.QtGui import QIcon
from PyQt5.QtCore import Qt, QSize
from schematic.schematicWidget import SchematicWidget, ActionMode
from schematic.element import ElementDirection
from workspace import Workspace, Document

# --- Document ---
class DocumentSchematic(Document):
    def __init__(self, workspace, mainWindow, filename, symbolMode):
        #
        # Schematic Editor
        #
        schematicView = QScrollArea()
        self.schematicWidget = SchematicWidget(symbolMode, schematicView)
        schematicView.setWidget(self.schematicWidget)

        super().__init__(workspace, schematicView, filename)
        
        #
        # Actions - edit
        #
        undoStack = self.schematicWidget.getUndoStack()
        self.actionUndo = self._addWidget(undoStack.createUndoAction(mainWindow, "&Undo"))
        self.actionUndo.setIcon(QIcon(':/assets/undo.png'))
        self.actionUndo.setStatusTip('Undo.')
        
        self.actionRedo = self._addWidget(undoStack.createRedoAction(mainWindow, "&Redo"))
        self.actionRedo.setIcon(QIcon(':/assets/redo.png'))
        self.actionRedo.setStatusTip('Redo.')
        
        #
        # Menu - edit
        #
        mainWindow.menuEdit.addAction(self.actionUndo)
        mainWindow.menuEdit.addAction(self.actionRedo)
        
        #
        # Toolbar - edit
        #
        mainWindow.toolbarEdit.addAction(self.actionUndo)
        mainWindow.toolbarEdit.addAction(self.actionRedo)
        
        if False:
            for i in range(1):
                line = self.schematicWidget.createElement("line")
                line.setPos(120, 120, 200, 120)
                
                text = self.schematicWidget.createElement("text")
                text.setPos(128, 128)
                text.setText('Hello world 0')
                text.textStyle.set(QColor(0, 0, 0), 'Times New Roman', 8, False, False, False)
                text.setRotation(0)
                
                text = self.schematicWidget.createElement("text")
                text.setPos(128, 128)
                text.setText('Hello world 90')
                text.textStyle.set(QColor(0, 0, 0), 'Times New Roman', 8, False, False, False)
                text.setRotation(90)
                
                text = self.schematicWidget.createElement("text")
                text.setPos(128, 128)
                text.setText('Hello world 180')
                text.textStyle.set(QColor(0, 0, 0), 'Times New Roman', 8, False, False, False)
                text.setRotation(180)
                
                text = self.schematicWidget.createElement("text")
                text.setPos(128, 128)
                text.setText('Hello world 270')
                text.textStyle.set(QColor(0, 0, 0), 'Times New Roman', 8, False, False, False)
                text.setRotation(270)
                
                rect = self.schematicWidget.createElement("rect")
                rect.setPos(760, 300, 500, 500)
                
                ellipse = self.schematicWidget.createElement("ellipse")
                ellipse.setPos(400, 400, 600, 600)
                
                arc = self.schematicWidget.createElement("arc")
                arc.setAngle(30, -270*16)
                arc.setPos(200, 200, 300, 300)
                
                chord = self.schematicWidget.createElement("chord")
                chord.setAngle(0, -270*16)
                chord.setPos(300, 300, 400, 400)
                
                line = self.schematicWidget.createElement("line")
                line.setPos(120, 120, 100, 400)
                
                pin = self.schematicWidget.createElement("pin")
                pin.setPos(100, 400)
                pin.setDirection(ElementDirection.DIR_LEFT)
                pin = self.schematicWidget.createElement("pin")
                pin.setDirection(ElementDirection.DIR_RIGHT)
                pin.setPos(100, 400)
                
                pin = self.schematicWidget.createElement("pin")
                pin.setPos(100, 400)
                pin.setDirection(ElementDirection.DIR_TOP)
                
                pin = self.schematicWidget.createElement("pin")
                pin.setPos(100, 400)
                pin.setDirection(ElementDirection.DIR_BOTTOM)
                
            
            self.schematicWidget.update()
            
            fp = open("schematic1.sch", "w")
            self.schematicWidget.save(fp)
            
        if False:
            fp = open("schematic1.sch", "r")
            self.schematicWidget.open(fp)

    # @Override
    def open(self):
        #try:
        fp = open(self.filename, 'r')
        self.schematicWidget.open(fp)
        self.schematicWidget.update()
        fp.close()

        '''except:
            return False
        else:
            return True'''
            
    # @Override
    def save(self):
        #try:
        fp = open(self.filename, 'w')
        self.schematicWidget.save(fp)
        fp.close()
            
        '''except:
            return False
        else:
            return True'''
        
    # @Override
    def close(self):
        self.schematicWidget.close()

# --- Workspace ---
class WorkspaceSchematic(Workspace):
    def __init__(self, mainWindow, symbolMode):
        super().__init__(mainWindow)
        self.symbolMode = symbolMode
        #
        # Actions - mode
        #
        self.actionGroupModes = QActionGroup(mainWindow)
        
        self.actionSelection = self._addWidget(QAction(QIcon(':/assets/modeselection.png'), '&Enter selection mode.', mainWindow))
        self.actionSelection.setStatusTip('Enter selection mode.')
        self.actionSelection.setCheckable(True)
        self.actionSelection.triggered.connect(self.slotSelection)
        self.actionGroupModes.addAction(self.actionSelection)
        
        if self.symbolMode:
            self.actionInsertPin = self._addWidget(QAction(QIcon(':/assets/insertpin.png'), '&Insert a pin to schematic.', mainWindow))
            self.actionInsertPin.setStatusTip('Insert a pin to schematic.')
            self.actionInsertPin.setCheckable(True)
            self.actionInsertPin.triggered.connect(self.slotInsertPin)
            self.actionGroupModes.addAction(self.actionInsertPin)
        else:
            self.actionInsertDevice = self._addWidget(QAction(QIcon(':/assets/insertdevice.png'), '&Insert a device to schematic.', mainWindow))
            self.actionInsertDevice.setStatusTip('Insert a device to schematic.')
            self.actionInsertDevice.setCheckable(True)
            self.actionInsertDevice.triggered.connect(self.slotInsertDevice)
            self.actionGroupModes.addAction(self.actionInsertDevice)
            
        self.actionInsertLine = self._addWidget(QAction(QIcon(':/assets/insertline.png'), '&Insert a line to schematic.', mainWindow))
        self.actionInsertLine.setStatusTip('Insert a line to schematic.')
        self.actionInsertLine.setCheckable(True)
        self.actionInsertLine.triggered.connect(self.slotInsertLine)
        self.actionGroupModes.addAction(self.actionInsertLine)
        
        self.actionInsertRect = self._addWidget(QAction(QIcon(':/assets/insertrect.png'), '&Insert a rectangle to schematic.', mainWindow))
        self.actionInsertRect.setStatusTip('Insert a rectangle to schematic.')
        self.actionInsertRect.setCheckable(True)
        self.actionInsertRect.triggered.connect(self.slotInsertRect)
        self.actionGroupModes.addAction(self.actionInsertRect)
        
        
        self.actionInsertEllipse = self._addWidget(QAction(QIcon(':/assets/insertellipse.png'), '&Insert a ellipse to schematic.', mainWindow))
        self.actionInsertEllipse.setStatusTip('Insert a ellipse to schematic.')
        self.actionInsertEllipse.setCheckable(True)
        self.actionInsertEllipse.triggered.connect(self.slotInsertEllipse)
        self.actionGroupModes.addAction(self.actionInsertEllipse)
        
        self.actionInsertArc = self._addWidget(QAction(QIcon(':/assets/insertarc.png'), '&Insert an arc to schematic.', mainWindow))
        self.actionInsertArc.setStatusTip('Insert an arc to schematic.')
        self.actionInsertArc.setCheckable(True)
        self.actionInsertArc.triggered.connect(self.slotInsertArc)
        self.actionGroupModes.addAction(self.actionInsertArc)
        
        self.actionInsertChord = self._addWidget(QAction(QIcon(':/assets/insertchord.png'), '&Insert a chord to schematic.', mainWindow))
        self.actionInsertChord.setStatusTip('Insert a chord to schematic.')
        self.actionInsertChord.setCheckable(True)
        self.actionInsertChord.triggered.connect(self.slotInsertChord)
        self.actionGroupModes.addAction(self.actionInsertChord)
        
        self.actionInsertText = self._addWidget(QAction(QIcon(':/assets/inserttext.png'), '&Insert a text to schematic.', mainWindow))
        self.actionInsertText.setStatusTip('Insert a text to schematic.')
        self.actionInsertText.setCheckable(True)
        self.actionInsertText.triggered.connect(self.slotInsertText)
        self.actionGroupModes.addAction(self.actionInsertText)
        
        self.actionInsertOrigin = self._addWidget(QAction(QIcon(':/assets/insertorigin.png'), '&Insert an origin to schematic.', mainWindow))
        self.actionInsertOrigin.setStatusTip('Insert an origin to schematic.')
        self.actionInsertOrigin.setCheckable(True)
        self.actionInsertOrigin.triggered.connect(self.slotInsertOrigin)
        self.actionGroupModes.addAction(self.actionInsertOrigin)

        self.actionSelection.setChecked(True)
        
        #
        # Toolbar - mode 
        #
        self.toolbarMode = self._addWidget(QToolBar("Mode", mainWindow))
        self.toolbarMode.addAction(self.actionSelection)
        self.toolbarMode.addSeparator()
        if self.symbolMode:
            self.toolbarMode.addAction(self.actionInsertPin)
        else:
            self.toolbarMode.addAction(self.actionInsertDevice)
        self.toolbarMode.addSeparator()
        self.toolbarMode.addAction(self.actionInsertLine)
        self.toolbarMode.addAction(self.actionInsertRect)
        self.toolbarMode.addAction(self.actionInsertEllipse)
        self.toolbarMode.addAction(self.actionInsertArc)
        self.toolbarMode.addAction(self.actionInsertChord)
        self.toolbarMode.addAction(self.actionInsertText)
        self.toolbarMode.addSeparator()
        self.toolbarMode.addAction(self.actionInsertOrigin)
        self.toolbarMode.setIconSize(QSize(20, 20))
        mainWindow.addToolBar(Qt.LeftToolBarArea, self.toolbarMode)

    # @Override
    def switch(self, visible, currentDocument):
        super().switch(visible, currentDocument)

    # @Override
    def state(self):
        s = [self.actionSelection.isChecked(),
                self.actionInsertLine.isChecked(),
                self.actionInsertRect.isChecked(),
                self.actionInsertEllipse.isChecked(),
                self.actionInsertArc.isChecked(),
                self.actionInsertChord.isChecked(),
                self.actionInsertText.isChecked(),
                self.actionInsertOrigin.isChecked()]

        if self.symbolMode:
            s.append(self.actionInsertPin.isChecked())
        else:
            s.append(self.actionInsertDevice.isChecked())
        return s
        
    # @Override
    def setState(self, state):
        self.actionSelection.setChecked(state.pop(0))
        self.actionInsertLine.setChecked(state.pop(0))
        self.actionInsertRect.setChecked(state.pop(0))
        self.actionInsertEllipse.setChecked(state.pop(0))
        self.actionInsertArc.setChecked(state.pop(0))
        self.actionInsertChord.setChecked(state.pop(0))
        self.actionInsertText.setChecked(state.pop(0))
        self.actionInsertOrigin.setChecked(state.pop(0))
        
        if self.symbolMode:
            self.actionInsertPin.setChecked(state.pop(0))
        else:
            self.actionInsertDevice.setChecked(state.pop(0))
        
    # @Override
    def createDocument(self, mainWindow, filename):
        return DocumentSchematic(self, mainWindow, filename, self.symbolMode)
          
    #
    # Slot Processing
    #
    def slotSelection(self, event):
        pass
    def slotInsertPin(self, event):
        pass
    def slotInsertDevice(self, event):
        pass
    def slotInsertLine(self, event):
        pass
    def slotInsertRect(self, event):
        pass
    def slotInsertEllipse(self, event):
        pass
    def slotInsertArc(self, event):
        pass
    def slotInsertChord(self, event):
        pass
    def slotInsertText(self, event):
        pass
    def slotInsertOrigin(self, event):
        pass
