# $File: //ASP/tec/gui/qevcl/trunk/qeVCLApp/src/project/widgets/shutter/vcl_shutter.pri $
# $Revision: #2 $
# $DateTime: 2020/09/14 20:04:51 $
# Last checked in by: $Author: starritt $
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
