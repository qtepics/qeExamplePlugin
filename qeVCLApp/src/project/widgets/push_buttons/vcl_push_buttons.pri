# $File: //ASP/tec/gui/qevcl/trunk/qeVCLApp/src/project/widgets/push_buttons/vcl_push_buttons.pri $
# $Revision: #1 $
# $DateTime: 2021/05/01 20:37:04 $
# Last checked in by: $Author: starritt $
#

INCLUDEPATH += $$PWD

HEADERS += $$PWD/vcl_push_buttons.h
SOURCES += $$PWD/vcl_push_buttons.cpp

HEADERS += $$PWD/vcl_push_buttons_manager.h
SOURCES += $$PWD/vcl_push_buttons_manager.cpp

OTHER_FILES += $$PWD/vcl_apply_push_button.png
OTHER_FILES += $$PWD/vcl_navigate_push_button.png
OTHER_FILES += $$PWD/vcl_reset_push_button.png
OTHER_FILES += $$PWD/vcl_start_push_button.png
OTHER_FILES += $$PWD/vcl_stop_push_button.png
OTHER_FILES += $$PWD/vcl_validate_push_button.png

RESOURCES   += $$PWD/vcl_push_buttons.qrc

# end
