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

def save(node, val):
    node.append(val)

def saveObj(node, obj):
    nodeMid = []
    obj.serialize(nodeMid)
    node.append(nodeMid)

def saveIns(node, obj):
    nodeMid = []
    obj.serialize(nodeMid)
    node.append( [obj.classname(), [nodeMid]] )

def get(node):
    return node.pop(0)

def getObj(node, obj):
    obj.deserialize(node.pop(0))

def getIns(node):
    return node.pop(0)
