# $File: //ASP/tec/gui/qevcl/trunk/qeVCLApp/src/Makefile $
# $Revision: #3 $
# $DateTime: 2025/01/20 14:28:28 $
# Last checked in by: $Author: starritt $
#
# This make file is the bridge beween the EPICS build system and the Qt project qmake/make paradigm.
#

TOP=../..

# Pick up configure/RELEASE defitions.
#
include $(TOP)/configure/CONFIG
#
# Note: we do not include the rule $(TOP)/configure/RULES

# Create environment variables from Make variable as defined in $(TOP)/configure/RELEASE
# Note use of := which avoids "*** Recursive variable `EPICS_BASE' references itself (eventually)" error
#
export EPICS_BASE := $(EPICS_BASE)
export QE_FRAMEWORK := $(QE_FRAMEWORK)

# Commands
# EPICS make system defines many OS independent commands, but not all
#
ifeq ($(OS),Windows_NT)
    define NOOP
       echo ""
    endef

else
    define NOOP
       :
    endef

endif

# Directories of interest.
#
SOURCE_DIR = project

# Must be consistant with project file
#
TARGET_DIR = $(TOP)/lib/$(EPICS_HOST_ARCH)

# Files
# Must be consistant with project file
#
MAKEFILE = Makefile.$(EPICS_HOST_ARCH)
PROJECT  = vcl.pro

ifeq ($(OS),Windows_NT)
   LIBFILE = vcl_plugin.dll
else
   LIBFILE = vcl_plugin.so
endif

# Full target path
#
TARGET=$(TARGET_DIR)/designer/$(LIBFILE)


.PHONY: all install clean uninstall

all: $(TARGET)

install: $(TARGET)


# The Qt widget loader (as used by qegui, kubili, etc.) expect plugin libraries
# to be located in a directory called designer, i.e. like:  ...../designer/libvcl_plugin.so
# and QT_PLUGIN_PATH excludes the /designer part of the path.
#
# Note: we always run this step
#
$(TARGET) : $(SOURCE_DIR)/$(MAKEFILE)  $(SOURCE_DIR)/*/*
	@echo "=== Building $(LIBFILE) plugin"                      && \
	cd  $(SOURCE_DIR)                                           && \
	make -f $(MAKEFILE) -j 3 -w                                 && \
	echo "=== $(LIBFILE) library build complete"


$(SOURCE_DIR)/$(MAKEFILE) : $(SOURCE_DIR)/$(PROJECT) $(SOURCE_DIR)/*/*.pri
	@echo "=== Running qmake - generating $(MAKEFILE)"          && \
	cd  $(SOURCE_DIR)                                           && \
	qmake -o $(MAKEFILE) $(PROJECT) -r


# Do a qt clean, then delete all qmake generated Makefiles.
# Note the : "command" is a null command in bash.
#
clean:
	cd $(SOURCE_DIR) && $(MAKE) -f $(MAKEFILE) clean || $(NOOP)
	cd $(SOURCE_DIR) && $(RM) $(MAKEFILE)


uninstall:
	rm -f $(TARGET)

# end
