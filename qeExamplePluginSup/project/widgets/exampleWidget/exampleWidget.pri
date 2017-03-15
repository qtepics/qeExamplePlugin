# exampleWidget.pri
#
# This file is part of the EPICS QT Framework, and included into
# examplePlugin.pro as part of a sample custom designer plugin
# using the QE Framework.
#
OTHER_FILES += \
    widgets/exampleWidget/exampleWidget.png

RESOURCES += \
    widgets/exampleWidget/exampleWidget.qrc

HEADERS += \
    widgets/exampleWidget/exampleWidget.h \
    widgets/exampleWidget/exampleWidgetManager.h

SOURCES += \
    widgets/exampleWidget/exampleWidget.cpp \
    widgets/exampleWidget/exampleWidgetManager.cpp

INCLUDEPATH += \
    widgets/exampleWidget \
    plugin

LIBS += -L$$_QE_FRAMEWORK/designer -lQEPlugin

# end
