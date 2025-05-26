# File: qeVCLApp/src/project/widgets/polyline/vcl_polyline.pri
# DateTime: Mon May 26 17:17:29 2025
# Last checked in by: starritt
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
