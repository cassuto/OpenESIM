##
# @brief Base class of all the schematic elements
#

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

from util.persistence import save, get
from enum import IntEnum

"""
@brief Indicate the type of element size in form of bitmask.
       The flags of ElementSizeType could be set simultaneously, but there should be
        at least a flag corresponding to setPos() set.
"""
class ElementSizeType(IntEnum):
    LINE_2P = 1     # setPos(x1, y1, x2, y2)
    RECT_1P = 1<<2  # setPos(x1, y1)
    RECT_4P = 1<<3  # setPos(x1, y1, x2, y2, x3, y3, x4, y4)
    ANGLE_2A  = 1<<4  # optionally, setAngle(a1, a2)
    
"""
@brief Direction of element.
"""
class ElementDirection(IntEnum):
    DIR_RIGHT = 1
    DIR_TOP = 2
    DIR_LEFT = 3
    DIR_BOTTOM = 4
    
class Element():
    def __init__(self, doc, id):
        # Persistence properties
        # Note that all the child classes should call super().serialize() and super().deserialize()
        # to keep consistent of these variables with document stream.
        
        # unique ID
        self.id = id
        # reference depth (the number of elements referenced this element)
        self.subDepth = 0
        # IDs of sub elements
        self.subElementId = []
        # Instance of sub elements, resolved and turn valid after createSubElements() is called.
        self.subElements = []
        
        # dynamic properties (no need to be saved)
        self.selected = False

    # the following methods must be reimplemented by child #
    
    """
    @brief Persistence interface - class name used for persistence
    """
    def classname(self):
        return "unknown"

    """
    @brief Persistence interface - serialize the element.
    @return status code.
    """
    def serialize(self, node):
        save(node, self.id)
        save(node, self.subDepth)
        save(node, self.subElementId)

    """
    @brief Persistence interface - deserialize the element.
    @return status code.
    """
    def deserialize(self, node):
        self.id = get(node)
        self.subDepth = get(node)
        self.subElementId = get(node)
    
    """
    @brief Get graphic bound rectangle of this element.
    @return (x1,y1,x2,y2)
    """
    def bounding(self):
        if len(self.subElements) == 0:
            return (0, 0, 0, 0)
        else:
            if not hasattr(self, "__minX"): # cache the bounding of sub elements
                (self.__minX, self.__minY, self.__maxX, self.__maxY) = self.subElements[0].bounding()
                for element in self.subElements:
                    (x1, y1, x2, y2) = element.bounding()
                    cx1 = x1 if (x1 < x2) else x2
                    cx2 = x1 if (x1 > x2) else x2
                    cy1 = y1 if (y1 < y2) else y2
                    cy2 = y1 if (y1 > y2) else y2
                    
                    if cx1 < self.__minX: self.__minX = cx1
                    if cy1 < self.__minY: self.__minY = cy1
                    if cx2 > self.__maxX: self.__maxX = cx2
                    if cy2 > self.__maxY: self.__maxY = cy2
            return (self.__minX, self.__minY, self.__maxX, self.__maxY)
    
    """
    @brief Called by child class, when the cached bounding became invalid. 
    """
    def _invalidBounding(self):
        if hasattr(self, "__minX"):
            delattr(self, "__minX")
    
    """
    @brief Get size type. The Element must implement setPos() interface as well,
           see @ElementSizeType for details.
    """
    def sizeType(self):
        return ElementSizeType.RECT_4P
    
    """
    @brief Render interface
    """
    def render(self, viewPainter):
        pass

    # optional methods for child to be reimplemented #
        
    """
    @brief Get if the element is modifiable
    """
    def modifiable(self):
        return self.subDepth == 0
        
    """
    @brief Attach a sub element. This will set subDepth property of source element concurrently.
    """
    def attachElement(self, element):
        element.subDepth += 1
        self.subElementId.append(element.id)
        self.subElements.append(element)

    """
    @brief Called after all the preparations (including deserializing) has been done.
            Framework guarantees the following calling sequence:
                __init__() -> deserialize() -> resolveSubElements()
            No function that queries or modifies attributes of element is called during this period.
            Note that this func might be called many times during the element is alive.
    @return True if there are sub elements, otherwise False.
    """
    def resolveSubElements(self, doc):
        if len(self.subElementId) > 0:
            if len(self.subElements) == 0:
                self.subElements.clear()
                for id in self.subElementId:
                    instance = doc.getElement(id) # resolve sub elements
                    self.subElements.append(instance)
            return True
        return False
