# File: qeVCLApp/src/project/vcl.pro
# DateTime: Fri Nov 14 17:13:08 2025
# Last checked in by: starritt
#
#===========================================================
# Since Qt 4.7.4, enable-auto-import is required to avoid a crash on windows when the qwt dll is loaded
# Depending on compiler the '-W1' may or may not be required.
# If an error relating to nable-auto-import occurs (missing leading -e) then use -W1 option
#win32:QMAKE_LFLAGS += -enable-auto-import
win32QMAKE_LFLAGS += -Wl,-enable-auto-import

#==========================================================
#Define this if you are using MinGW compiler
#DEFINES += _MINGW=TRUE
#===========================================================
# Project configuration

# Points to the top directory in which plugin will be created.
# This follows the regular EPICS Makefile paradigm.
#
TOP=../../..

# Announce version.
#
message ("QT_VERSION = "$$QT_MAJOR_VERSION"."$$QT_MINOR_VERSION"."$$QT_PATCH_VERSION )

# Qt 4 configuration - not supported
equals( QT_MAJOR_VERSION, 4 ) {
    error ( "Qt Version 4 not supported")
}

# Qt 5 configuration
equals( QT_MAJOR_VERSION, 5 ) {
    CONFIG += plugin
    QT += core gui xml network
    QT += uitools designer
}

# Qt 6 configuration
equals( QT_MAJOR_VERSION, 6 ) {
    CONFIG += plugin
    QT += core gui xml network
    QT += uitools designer
}

# Must be consistant with Makefile
TARGET = vcl_plugin
TEMPLATE = lib
DEFINES += VISUAL_COMPONENT_LIBRARY

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
    error( "QE_FRAMEWORK must be defined. Ensure EPICS Qt is installed and QE_FRAMEWORK environment variable is defined." )
}

INSTALL_DIR = $$TOP

# The plugin ends up here.
#
DESTDIR = $$INSTALL_DIR/lib/$$(EPICS_HOST_ARCH)/designer

# Place all intermediate generated files in architecture specific locations
#
MOC_DIR        = O.$$(EPICS_HOST_ARCH)/moc
OBJECTS_DIR    = O.$$(EPICS_HOST_ARCH)/obj
UI_DIR         = O.$$(EPICS_HOST_ARCH)/ui
RCC_DIR        = O.$$(EPICS_HOST_ARCH)/rcc
MAKEFILE       = Makefile.$$(EPICS_HOST_ARCH)

# We don't get these include paths for free - need to be explicit.
#
INCLUDEPATH += O.$$(EPICS_HOST_ARCH)/ui
INCLUDEPATH += O.$$(EPICS_HOST_ARCH)/moc

#===========================================================
# Project files
#
# Plugin files
include (plugin/plugin.pri)

# Widget sub projects
# Included .pri (project include) files for each widget
#
include (widgets/dblslits/vcl_dblslits.pri)
include (widgets/vertslits/vcl_vertslits.pri)
include (widgets/horizslits/vcl_horizslits.pri)
include (widgets/shutter/vcl_shutter.pri)
include (widgets/valve/vcl_valve.pri)
include (widgets/filter_screen/vcl_filter_screen.pri)
include (widgets/stack_lights/vcl_stack_lights.pri)
include (widgets/push_buttons/vcl_push_buttons.pri)
include (widgets/icons/vcl_icons.pri)
include (widgets/eps_comp/vcl_eps_comp.pri)

# INCLUDE NEXT WIDGET HERE...
# include (widgets/polyline/vcl_polyline.pri)


# Note: This is currently intended to be a pure plugin library only.
# #===========================================================
# # Install include files
# # THIS MUST BE AFTER SUB-PROJECT INCLUDES
# # If plugin is to be used as a pure plugin, this step is not required.
# #
# VCL_INCLUDEPATH = $$INSTALL_DIR/include
# VCL_INCLUDE.files = $$HEADERS
# VCL_INCLUDE.path = $$VCL_INCLUDEPATH
# INSTALLS += VCL_INCLUDE
#
# message( "Note: By default qtcreator does not have a 'make install' build step. When using qtcreator, modify project " )
# message( "....: to add an install build step which is required to install header files to $$INSTALL_DIR/include" )


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
