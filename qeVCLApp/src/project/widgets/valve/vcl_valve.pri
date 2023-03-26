# $File: //ASP/tec/gui/qevcl/trunk/qeVCLApp/src/project/widgets/valve/vcl_valve.pri $
# $Revision: #2 $
# $DateTime: 2020/09/14 20:04:51 $
# Last checked in by: $Author: starritt $
#

INCLUDEPATH += $$PWD

HEADERS += $$PWD/vcl_valve.h
SOURCES += $$PWD/vcl_valve.cpp
FORMS   += $$PWD/vcl_valve.ui

HEADERS += $$PWD/vcl_valve_manager.h
SOURCES += $$PWD/vcl_valve_manager.cpp

OTHER_FILES += $$PWD/vcl_valve_closed.png
OTHER_FILES += $$PWD/vcl_valve_invalid.png
OTHER_FILES += $$PWD/vcl_valve_moving.png
OTHER_FILES += $$PWD/vcl_valve_open.png
OTHER_FILES += $$PWD/vcl_valve_unknown.png
OTHER_FILES += $$PWD/vcl_valve_disconnected.png

RESOURCES   += $$PWD/vcl_valve.qrc

# end
