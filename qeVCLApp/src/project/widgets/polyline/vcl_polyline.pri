# $File: //ASP/tec/gui/qevcl/trunk/qeVCLApp/src/project/widgets/polyline/vcl_polyline.pri $
# $Revision: #1 $
# $DateTime: 2023/03/10 12:25:22 $
# Last checked in by: $Author: starritt $
#

INCLUDEPATH += $$PWD

HEADERS += $$PWD/vcl_polyline.h
SOURCES += $$PWD/vcl_polyline.cpp

HEADERS += $$PWD/vcl_polyline_manager.h
SOURCES += $$PWD/vcl_polyline_manager.cpp

HEADERS += $$PWD/vcl_polyline_setup_dialog.h
SOURCES += $$PWD/vcl_polyline_setup_dialog.cpp
FORMS   += $$PWD/vcl_polyline_setup_dialog.ui

OTHER_FILES += $$PWD/vcl_polyline.png

RESOURCES   += $$PWD/vcl_polyline.qrc

# end
