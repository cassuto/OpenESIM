INCLUDEPATH += schematic

SOURCES += \
    schematic/schematic.cc \
    schematic/schematicview.cc \
    schematic/factory.cc \
    schematic/renderdevice.cc \
    schematic/styletemplate.cc \
    schematic/element.cc \
    schematic/elementarc.cc \
    schematic/elementchord.cc \
    schematic/elementellipse.cc \
    schematic/elementcircle.cc \
    schematic/elementline.cc \
    schematic/elementrect.cc \
    schematic/elementtext.cc \
    schematic/elementpin.cc \
    schematic/elementorigin.cc \
    schematic/undocommands.cc \
    schematic/preview.cc
    
HEADERS += \
    schematic/schematic.h \
    schematic/schematicview.h \
    schematic/renderdevice.h

FORMS += \
    schematic/dialogs/schematic-graphicstyledialog.ui

SOURCES += \
    schematic/dialogs/schematic-graphicstyledialog.cc
    
HEADERS += \
    schematic/preview.h \
    schematic/dialogs/schematic-graphicstyledialog.h
