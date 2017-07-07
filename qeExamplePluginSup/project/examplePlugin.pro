# $File: //ASP/tec/gui/qeExamplePlugin/trunk/qeExamplePluginSup/project/examplePlugin.pro $
# $Revision: #3 $
# $DateTime: 2017/07/07 12:01:14 $
# Last checked in by: $Author: starritt $
#
#===========================================================
# Since Qt 4.7.4, enable-auto-import is required to avoid a crash on windows when the qwt dll is loaded
# Depending on compiler the '-W1' may or may not be required. If an error relating to nable-auto-import occurs (missing leading -e) then use -W1 option
#win32:QMAKE_LFLAGS += -enable-auto-import
win32QMAKE_LFLAGS += -Wl,-enable-auto-import
#==========================================================
#Define this if you are using MinGW compiler
#DEFINES += _MINGW=TRUE
#===========================================================
# Project configuration

# Points to the target directoy in which bin/EPICS_HOST_ARCH/qegui
# will be created. This follows the regular EPICS Makefile paradigm.
#
TOP=../..

message ("QT_MAJOR_VERSION = " $$QT_MAJOR_VERSION )


# Qt 4 configuration
equals( QT_MAJOR_VERSION, 4 ) {
    CONFIG += plugin designer
    QT += core gui multimedia
}

# Qt 5 configuration
equals( QT_MAJOR_VERSION, 5 ) {
    CONFIG += plugin
    QT += core gui designer multimedia
}

TARGET = examplePlugin
TEMPLATE = lib
DEFINES += EXAMPLE_PLUGIN_LIBRARY

# Determine EPICS_BASE
_EPICS_BASE = $$(EPICS_BASE)

# Check EPICS dependancies
isEmpty( _EPICS_BASE ) {
    error( "EPICS_BASE must be defined. Ensure EPICS is installed and EPICS_BASE environment variable is defined." )
}

_EPICS_HOST_ARCH = $$(EPICS_HOST_ARCH)
isEmpty( _EPICS_HOST_ARCH ) {
    error( "EPICS_HOST_ARCH must be defined. Ensure EPICS is installed and EPICS_HOST_ARCH environment variable is defined." )
}

# Determine QE framework library
#
_QE_FRAMEWORK = $$(QE_FRAMEWORK)
isEmpty( _QE_FRAMEWORK ) {
    error( "QE_FRAMEWORK must be defined. Ensure EPICS is installed and EPICS_HOST_ARCH environment variable is defined." )
}

# Install the generated plugin library and include files in QE_TARGET_DIR if defined.
_QE_TARGET_DIR = $$(QE_TARGET_DIR)
isEmpty( _QE_TARGET_DIR ) {
    INSTALL_DIR = $$TOP
    message( "QE_TARGET_DIR is not defined. The QE GUI application will be installed into the <top> directory." )
} else {
    INSTALL_DIR = $$(QE_TARGET_DIR)
    message( "QE_TARGET_DIR is defined. The QE GUI application will be installed directly into" $$INSTALL_DIR )
}

# The plugin ends up here.
#
DESTDIR = $$INSTALL_DIR/lib/$$(EPICS_HOST_ARCH)


# Place all intermediate generated files in architecture specific locations
#
MOC_DIR        = O.$$(EPICS_HOST_ARCH)/moc
OBJECTS_DIR    = O.$$(EPICS_HOST_ARCH)/obj
UI_DIR         = O.$$(EPICS_HOST_ARCH)/ui
RCC_DIR        = O.$$(EPICS_HOST_ARCH)/rcc
MAKEFILE       = Makefile.$$(EPICS_HOST_ARCH)

# We don't get this include path for free - need to be explicit.
#
INCLUDEPATH += O.$$(EPICS_HOST_ARCH)/ui

#===========================================================
# Project files
#
# Plugin files
include (plugin/plugin.pri)

# Widget sub projects
# Included .pri (project include) files for each widget
#
include (widgets/exampleWidget/exampleWidget.pri)
# INCLUE NEXT WIDGET HERE...


#===========================================================
# Install include files
# THIS MUST BE AFTER SUB-PROJECT INCLUDES
#
EXAMPLE_INCLUDEPATH = $$INSTALL_DIR/include
EXAMPLE_INCLUDE.files = $$HEADERS
EXAMPLE_INCLUDE.path = $$EXAMPLE_INCLUDEPATH
INSTALLS += EXAMPLE_INCLUDE

message( "Note: By default qtcreator does not have a 'make install' build step. When using qtcreator, modify project " )
message( "....: to add an install build step which is required to install header files to $$INSTALL_DIR/include" )


#===========================================================
# Dependancies
#
# Include header files from the QE framework
#
INCLUDEPATH += $$(QE_FRAMEWORK)/include

LIBS += -L$$(QE_FRAMEWORK)/lib/$$(EPICS_HOST_ARCH) -lQEFramework
LIBS += -L$$(EPICS_BASE)/lib/$$(EPICS_HOST_ARCH) -lca -lCom

# Set run time path for shared libraries
#
unix: QMAKE_LFLAGS += -Wl,-rpath,$$(QE_FRAMEWORK)/lib/$$(EPICS_HOST_ARCH)
unix: QMAKE_LFLAGS += -Wl,-rpath,$$(EPICS_BASE)/lib/$$(EPICS_HOST_ARCH)

# end

