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
import os.path

class DocumentType(IntEnum):
    DOC_SCHEMATIC = 1
    DOC_SCHEMATIC_SYMBOL = 2

    def getFromFilename(filename):
        suffix = os.path.splitext(filename)[1] 
        if suffix == ".sch":
            return DocumentType.DOC_SCHEMATIC
        elif suffix == ".ssym":
            return DocumentType.DOC_SCHEMATIC_SYMBOL
    
class Document():
    def __init__(self, workspace, view, filename):
        # Parent of document workspace
        self.workspace = workspace
        # Workspace state
        self.workspaceState = workspace.state()
        # Point to view widget
        self.view = view
        # Document filename
        self.filename = filename
        # Current document
        self.currentDocument = None
        # Dynamic widgets
        self.widgets = []
        
    def _addWidget(self, widget):
        self.widgets.append(widget)
        return widget

    def activate(self, active):
        if active:
            self.workspace.setState(self.workspaceState) # restore/store
        self.workspaceState = self.workspace.state()
        
        for widget in self.widgets: # Set visible of all the widgets
            widget.setEnabled(active)
            widget.setVisible(active)
            
    def open(self):
        pass
        
    def save(self):
        pass
        
    def close(self):
        pass
        
    def modified(self):
        return False
        
class Workspace():
    def __init__(self, mainWindow):
        self.widgets = []
        
    def _addWidget(self, widget):
        self.widgets.append(widget)
        return widget
    
    """
    @brief Interface to create a instance of document.
    """
    def createDocument(self, mainWindow, filename):
        return None

    """
    @brief Interface to get states of workspace elements.
    """
    def state(self):
        return []
    """
    @brief Interface to set states. Restore from workspace switching.
    """
    def setState(self, state):
        pass
        
    """
    @brief called when mainWindow is to switch workspace
    @param visible Indicate whether this workspace is visible
    """
    def switch(self, visible, currentDocument):
        self.currentDocument = currentDocument
        
        for widget in self.widgets: # Set visible of all the widgets
            widget.setEnabled(visible)
            widget.setVisible(visible)
