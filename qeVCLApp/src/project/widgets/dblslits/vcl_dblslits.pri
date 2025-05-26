# File: qeVCLApp/src/project/widgets/dblslits/vcl_dblslits.pri
# DateTime: Mon May 26 17:17:29 2025
# Last checked in by: starritt
#

INCLUDEPATH += $$PWD

HEADERS += $$PWD/vcl_dblslits.h
SOURCES += $$PWD/vcl_dblslits.cpp
FORMS   += $$PWD/vcl_dblslits.ui

HEADERS += $$PWD/vcl_dblslits_manager.h
SOURCES += $$PWD/vcl_dblslits_manager.cpp

OTHER_FILES += $$PWD/vcl_dblslits.png
OTHER_FILES += $$PWD/vcl_ds_down.png
OTHER_FILES += $$PWD/vcl_ds_left.png
OTHER_FILES += $$PWD/vcl_ds_neg.png
OTHER_FILES += $$PWD/vcl_ds_pos.png
OTHER_FILES += $$PWD/vcl_ds_right.png
OTHER_FILES += $$PWD/vcl_ds_up.png

RESOURCES   += $$PWD/vcl_dblslits.qrc

# end
