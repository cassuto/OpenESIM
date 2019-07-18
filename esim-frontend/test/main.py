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

import sys
from mainWindow import MainWindow
from PyQt5 import QtWidgets

if __name__=='__main__':
    app = QtWidgets.QApplication(sys.argv)
 
    #
    # Loading main UI
    #
    mainWindow = MainWindow()
    mainWindow.show()

    sys.exit(app.exec_())

