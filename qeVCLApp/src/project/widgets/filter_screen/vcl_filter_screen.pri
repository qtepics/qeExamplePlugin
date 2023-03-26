# $File: //ASP/tec/gui/qevcl/trunk/qeVCLApp/src/project/widgets/filter_screen/vcl_filter_screen.pri $
# $Revision: #4 $
# $DateTime: 2023/01/13 11:16:02 $
# Last checked in by: $Author: starritt $
#

INCLUDEPATH += $$PWD

HEADERS += $$PWD/vcl_filter_screen.h
SOURCES += $$PWD/vcl_filter_screen.cpp
FORMS   += $$PWD/vcl_filter_screen.ui

HEADERS += $$PWD/vcl_filter_screen_manager.h
SOURCES += $$PWD/vcl_filter_screen_manager.cpp

OTHER_FILES += $$PWD/vcl_filter_screen_r00_disconnected.png
OTHER_FILES += $$PWD/vcl_filter_screen_r00_inserted.png
OTHER_FILES += $$PWD/vcl_filter_screen_r00_invalid.png
OTHER_FILES += $$PWD/vcl_filter_screen_r00_moving.png
OTHER_FILES += $$PWD/vcl_filter_screen_r00_unknown.png
OTHER_FILES += $$PWD/vcl_filter_screen_r00_withdrawn.png
OTHER_FILES += $$PWD/vcl_filter_screen_r90_disconnected.png
OTHER_FILES += $$PWD/vcl_filter_screen_r90_inserted.png
OTHER_FILES += $$PWD/vcl_filter_screen_r90_invalid.png
OTHER_FILES += $$PWD/vcl_filter_screen_r90_moving.png
OTHER_FILES += $$PWD/vcl_filter_screen_r90_unknown.png
OTHER_FILES += $$PWD/vcl_filter_screen_r90_withdrawn.png

RESOURCES   += $$PWD/vcl_filter_screen.qrc

# end
