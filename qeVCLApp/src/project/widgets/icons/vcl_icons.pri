# File: qeVCLApp/src/project/widgets/icons/vcl_icons.pri
# DateTime: Mon May 26 17:17:29 2025
# Last checked in by: starritt
#
# This file is part of the EPICS Qt Visual Component Library
#

INCLUDEPATH += $$PWD

HEADERS += $$PWD/vcl_icons.h
SOURCES += $$PWD/vcl_icons.cpp

HEADERS += $$PWD/vcl_icons_manager.h
SOURCES += $$PWD/vcl_icons_manager.cpp

OTHER_FILES += $$PWD/vcl_camera_icon.png
OTHER_FILES += $$PWD/vcl_ion_pump_icon.png

RESOURCES += $$PWD/vcl_icons.qrc

# end
