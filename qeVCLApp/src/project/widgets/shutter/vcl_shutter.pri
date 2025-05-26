# File: qeVCLApp/src/project/widgets/shutter/vcl_shutter.pri
# DateTime: Mon May 26 17:17:29 2025
# Last checked in by: starritt
#

INCLUDEPATH += $$PWD

HEADERS += $$PWD/vcl_shutter.h
SOURCES += $$PWD/vcl_shutter.cpp
FORMS   += $$PWD/vcl_shutter.ui

HEADERS += $$PWD/vcl_shutter_manager.h
SOURCES += $$PWD/vcl_shutter_manager.cpp

OTHER_FILES += $$PWD/vcl_shutter_closed.png
OTHER_FILES += $$PWD/vcl_shutter_invalid.png
OTHER_FILES += $$PWD/vcl_shutter_moving.png
OTHER_FILES += $$PWD/vcl_shutter_open.png
OTHER_FILES += $$PWD/vcl_shutter_unknown.png
OTHER_FILES += $$PWD/vcl_shutter_disconnected.png

RESOURCES   += $$PWD/vcl_shutter.qrc

# end
