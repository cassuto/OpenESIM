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

from PyQt5.QtWidgets import QApplication, QAction, QWidget, QHBoxLayout, QMainWindow, QTreeWidget, QTreeWidgetItem, QMessageBox, QDesktopWidget, QFileDialog
from PyQt5.QtWidgets import QTableWidget, QTableWidgetItem, QHeaderView
from PyQt5.QtWidgets import QTabWidget
from PyQt5.QtGui import QIcon, QColor
from PyQt5.QtCore import Qt, QSize
from workspace import DocumentType
from workspaceSchematic import WorkspaceSchematic
from schematic.schematicWidget import SchematicWidget
import sip
import objgraph

class MainWindow(QMainWindow):
    def __init__(self):
        super().__init__()
        
        self.setGeometry(300, 300, 300, 220)
        self.setWindowTitle('OpenESIM (Electrical Engineering Simulator)')
        
        #
        # Actions of workspace
        #
        self.actionNew = QAction(QIcon(':/assets/filenew.png'), '&New document.', self)
        self.actionNew.setShortcut('Ctrl+N')
        self.actionNew.setStatusTip('New document.')
        self.actionNew.triggered.connect(self.slotNew)
        
        self.actionOpen = QAction(QIcon(':/assets/fileopen.png'), '&Open a document.', self)
        self.actionOpen.setShortcut('Ctrl+O')
        self.actionOpen.setStatusTip('Open a document.')
        self.actionOpen.triggered.connect(self.slotOpen)
        
        self.actionSave = QAction(QIcon(':/assets/filesave.png'), '&Save the current file.', self)
        self.actionSave.setShortcut('Ctrl+S')
        self.actionSave.setStatusTip('Save the current file.')
        self.actionSave.triggered.connect(self.slotSave)
        
        self.actionSaveall = QAction(QIcon(':/assets/filesaveall.png'), '&Save All the files.', self)
        self.actionSaveall.setShortcut('Ctrl+Shift+S')
        self.actionSaveall.setStatusTip('Save All the files.')
        self.actionSaveall.triggered.connect(self.slotSaveall)
        
        self.actionClose = QAction(QIcon(':/assets/fileclose.png'), '&Close the current file.', self)
        self.actionClose.setShortcut('Ctrl+K')
        self.actionClose.setStatusTip('Close the current file.')
        self.actionClose.triggered.connect(self.slotClose)
        
        self.actionCloseAll = QAction(QIcon(':/assets/filecloseall.png'), '&Close All the files.', self)
        self.actionCloseAll.setShortcut('Ctrl+ShiftK')
        self.actionCloseAll.setStatusTip('Close All the files.')
        self.actionCloseAll.triggered.connect(self.slotCloseAll)
        
        self.actionCloseAllButThis = QAction('&Close All but this file.', self)
        self.actionCloseAllButThis.setShortcut('Ctrl+ShiftK')
        self.actionCloseAllButThis.setStatusTip('Close All the files.')
        self.actionCloseAllButThis.triggered.connect(self.slotCloseAllButThis)
        
        self.actionPrint = QAction(QIcon(':/assets/print.png'), '&Print', self)
        self.actionPrint.setStatusTip('Print')
        self.actionPrint.triggered.connect(self.slotPrint)
        
        self.actionPrintSetup = QAction(QIcon(':/assets/printsetup.png'), '&Print Setup', self)
        self.actionPrintSetup.setStatusTip('Print Setup')
        self.actionPrintSetup.triggered.connect(self.slotPrintSetup)
        
        self.actionQuit = QAction('&Exit', self)
        self.actionQuit.setShortcut('Ctrl+Q')
        self.actionQuit.setStatusTip('Exit application')
        self.actionQuit.triggered.connect(self.slotQuit)

        self.actionGotoPrevTab = QAction(QIcon(':/assets/gotoprev.png'), '&Goto prev Table', self)
        self.actionGotoPrevTab.setStatusTip('Goto prev Table...')
        self.actionGotoPrevTab.triggered.connect(self.slotGotoPrevTab)
        self.actionGotoNextTab = QAction(QIcon(':/assets/gotonext.png'), '&Goto next Table', self)
        self.actionGotoNextTab.setStatusTip('Goto next Table...')
        self.actionGotoNextTab.triggered.connect(self.slotGotoNextTab)
 
        self.statusBar()
 
        menubar = self.menuBar()
        #
        # Menu - File
        #
        self.menuFile = menubar.addMenu('&File')
        self.menuFile.addAction(self.actionNew)
        self.menuFile.addSeparator()
        self.menuFile.addAction(self.actionOpen)
        self.menuFile.addAction(self.actionSave)
        self.menuFile.addAction(self.actionSaveall)
        self.menuFile.addSeparator()
        self.menuFile.addAction(self.actionClose)
        self.menuFile.addAction(self.actionCloseAll)
        self.menuFile.addSeparator()
        self.menuFile.addAction(self.actionPrint)
        self.menuFile.addAction(self.actionPrintSetup)
        self.menuFile.addSeparator()
        self.menuFile.addAction(self.actionQuit)
        
        #
        # Menu - Edit
        #
        self.menuEdit = menubar.addMenu('&Edit')
        
        #
        # Menu - Windows
        #
        self.menuWindows = menubar.addMenu('&Windows')
        self.menuWindows.addAction(self.actionClose)
        self.menuWindows.addAction(self.actionCloseAllButThis)
        self.menuWindows.addAction(self.actionCloseAll)
        self.menuWindows.addSeparator()
        self.menuWindows.addAction(self.actionGotoPrevTab)
        self.menuWindows.addAction(self.actionGotoNextTab)
        
        #
        # File ToolBar
        #
        self.toolbarFile = self.addToolBar('File')
        self.toolbarFile.addAction(self.actionNew)
        self.toolbarFile.addAction(self.actionOpen)
        self.toolbarFile.addAction(self.actionSave)
        self.toolbarFile.addAction(self.actionSaveall)
        self.toolbarFile.addSeparator()
        self.toolbarFile.addAction(self.actionClose)
        self.toolbarFile.addAction(self.actionCloseAll)
        self.toolbarFile.addSeparator()
        self.toolbarFile.addAction(self.actionPrint)
        self.toolbarFile.addAction(self.actionPrintSetup)
        self.toolbarFile.setIconSize( QSize(20, 29) )
        
        #
        # Edit ToolBar
        #
        self.toolbarEdit = self.addToolBar('Edit')
        
        self.setGeometry(300, 300, 1600, 800)
        self.center()
        
        #
        # TreeView for database entries
        #
        self.layoutTreeWidget = QTreeWidget()
        self.layoutTreeWidget.setColumnCount(2)
        self.layoutTreeWidget.setHeaderLabels(['Database','Filename'])
        self.layoutTreeWidget.header().setSectionResizeMode(QHeaderView.ResizeToContents);
        
        self.layoutTreeGroups = QTreeWidgetItem(self.layoutTreeWidget)
        self.layoutTreeGroups.setText(0, 'Groups')
        self.layoutTreeGroups.setExpanded(True)
        self.layoutTreeWidget.addTopLevelItem(self.layoutTreeGroups)
        
        #
        # SDI Table
        #
        self.tabWidget = QTabWidget()
        self.tabWidget.setTabsClosable(True)
        self.tabWidget.currentChanged.connect(self.slotCurrentDocumentChanged)
        self.tabWidget.tabCloseRequested.connect(self.closeDocument)

        self.centralWidget = QWidget(self)
        self.centralLayout = QHBoxLayout(self.centralWidget)
        self.centralLayout.setContentsMargins(0, 0, 0, 0)
        self.centralLayout.setSpacing(0)
        self.centralLayout.addWidget(self.layoutTreeWidget)
        self.centralLayout.addWidget(self.tabWidget)
        self.centralLayout.setStretchFactor(self.layoutTreeWidget, 1);
        self.centralLayout.setStretchFactor(self.tabWidget, 5);
        self.setCentralWidget(self.centralWidget)
        
        #
        # Prepare Workspaces
        #
        self.workspaces = {}
        self.workspaces[int(DocumentType.DOC_SCHEMATIC)] = WorkspaceSchematic(self, False)
        self.workspaces[int(DocumentType.DOC_SCHEMATIC_SYMBOL)] = WorkspaceSchematic(self, True)
        self.documents = []
        self.currentDocument = None
        self.currentWorkspace = None
        self.__activateWorkspace(None)
        
    def updateActions(self):
        hasDocuments = (self.tabWidget.count() > 0)
        oneMoreThanDocuments = self.tabWidget.count() > 1
        self.actionSave.setEnabled(hasDocuments)
        self.actionSaveall.setEnabled(hasDocuments)
        self.actionPrint.setEnabled(hasDocuments)
        
        self.actionClose.setEnabled(hasDocuments)
        self.actionCloseAll.setEnabled(oneMoreThanDocuments)
        self.actionCloseAllButThis.setEnabled(oneMoreThanDocuments)
        self.actionGotoPrevTab.setEnabled(oneMoreThanDocuments)           
        self.actionGotoNextTab.setEnabled(oneMoreThanDocuments)
        
    def center(self):
        qr = self.frameGeometry()
        cp = QDesktopWidget().availableGeometry().center()
        qr.moveCenter(cp)
        self.move(qr.topLeft())
    
    def workspace(self, type):
        return self.workspaces[int(type)]
    
    def newDocument(self, filename):
        staticWS = self.workspace( DocumentType.getFromFilename(filename) )
        
        document = staticWS.createDocument(self, filename)
        self.documents.append(document)
        
        self.tabWidget.addTab(document.view, '%s - [%d]' % (filename, len(self.documents)-1) )
        self.tabWidget.setCurrentWidget(document.view)
        
        return document
        
    def closeDocument(self, tabIndex):
        document = self.documents[tabIndex]
        if document.modified():
            btn = QMessageBox.warning(self,
                                    'Close document', "The document '%s' has been modified, would you want to save it?" % document.filename,
                                    QMessageBox.Yes | QMessageBox.No, QMessageBox.Yes)
            if btn == QMessageBox.Yes:
                document.save()

        document.close()
        self.tabWidget.removeTab(self.tabWidget.indexOf(document.view))
        view = document.view
        self.documents.remove(document)
        sip.delete(view) #!FIXME ugly delete C++ pointer here, but memory leaked without sake?

    def closeAllDocuments(self):
        while self.tabWidget.count() > 0:
            self.closeDocument(0)
        
    def openDocument(self, filename):
        document = self.newDocument(filename)
        result = document.open()

    def saveDocument(self, tabIndex):
        document = self.newDocument(filename)
        result = document.save()

    def gotoPrevTab(self):
        last = self.tabWidget.count()
        current = self.tabWidget.currentIndex()
        if last:
            last -= 1
            current = last if current == 0 else current - 1
            self.tabWidget.setCurrentIndex(current)
            
    def gotoNextTab(self):
        last = self.tabWidget.count()
        current = self.tabWidget.currentIndex()
        if last:
            last -= 1
            current = 0 if current == last else current + 1
            self.tabWidget.setCurrentIndex(current)

    """
    @brief Activate workspace.
    @param currentWorkspace Point to the target workspace, None if nothing.
    """
    def __activateWorkspace(self, currentWorkspace, currentDocument=None):
        if currentWorkspace is not None:
            for type, staticWS in self.workspaces.items():
                if currentWorkspace is staticWS:
                    staticWS.switch(True, currentDocument)
                else:
                    staticWS.switch(False, None)
        else:
            for type, staticWS in self.workspaces.items():
                staticWS.switch(False, None)
        
        self.currentDocument = currentDocument
        self.currentWorkspace = currentWorkspace
        
        self.updateActions()
    
    #
    # Slot Processing
    #
    def slotNew(self, event):
        for i in range(1):
            self.newDocument("./sch1.sch")
    def slotOpen(self, event):
        filename, types = QFileDialog.getOpenFileName(self,'Open file')
        if len(filename):
            self.openDocument(filename)
    def slotSave(self, event):
        self.saveDocument(self.tabWidget.currentIndex())
    def slotSaveall(self, event):
        for i in range(self.tabWidget.count()):
            self.saveDocument(i)
    def slotClose(self, event):
        self.closeDocument(self.tabWidget.currentIndex())
        #objgraph.show_backrefs(objgraph.by_type('DocumentSchematic')[0], max_depth = 10, filename = 'obj.dot')
    def slotCloseAll(self, event):
        self.closeAllDocuments()
    def slotCloseAllButThis(self, event):
        thisId = self.tabWidget.currentIndex()
        i = 0
        while self.tabWidget.count() > 1:
            if i < thisId:
                self.closeDocument(0)
            elif i > thisId:
                self.closeDocument(1)
            i += 1

    def slotPrint(self, event):
        pass
    def slotPrintSetup(self, event):
        pass
         
    def slotQuit(self, event):
        self.close()

    def slotGotoPrevTab(self, event):
        self.gotoPrevTab()
    def slotGotoNextTab(self, event):
        self.gotoNextTab()
    
    def treeItemDoubleClick(self, item, index):
        pass
    
    def slotCurrentDocumentChanged(self, index): # Handle switching among documents
        if self.currentDocument is not None:
            self.currentDocument.activate(False) # restore the status of previous document
        if index != -1:
            currentDocument = self.documents[index]
            currentDocument.activate(True) # Activate the document and workspace
            self.__activateWorkspace(currentDocument.workspace, currentDocument)
        else:
            self.__activateWorkspace(None)

    def closeEvent(self, event):
        self.closeAllDocuments()
