# $File: //ASP/tec/gui/qeExamplePlugin/trunk/qeExamplePluginSup/Makefile $
# $Revision: #4 $
# $DateTime: 2017/05/03 18:48:46 $
# Last checked in by: $Author: starritt $
#
# This make file is the bridge beween the EPICS build system and the Qt project qmake/make paradigm.
#

TOP=..

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
    # We use a hard link - it has to be the same file otherwise loading and
    # plugin libraries end up being separate copies; also hardlinks don't
    # require special privileges. Hey ... it's Windoze, what do you expect??
    # At least the parameter order is sensible.
    #
    define LINKFILE
        MKLINK /H  $(LIBFILE) ..\$(LIBFILE)
    endef

    define NOOP
       echo ""
    endef

else
    # Linux and the like.
    # We can use symbolic links on Linux
    #
    define LINKFILE
       rm -f $(1) && ln -s ../$(LIBFILE)  $(LIBFILE)
    endef

    define NOOP
       :
    endef

endif

# Directories of interest.
#
SOURCE_DIR = project
TARGET_DIR = $(TOP)/lib/$(EPICS_HOST_ARCH)

# Files
#
MAKEFILE = Makefile.$(EPICS_HOST_ARCH)
PROJECT  = examplePlugin.pro

ifeq ($(OS),Windows_NT)
   LIBFILE = examplePlugin.dll
else
   LIBFILE = libexamplePlugin.so
endif

# Full target path
#
TARGET=$(TARGET_DIR)/$(LIBFILE)


.PHONY: all install clean uninstall  always

all: $(TARGET)

install: $(TARGET)


# The Qt widget loader (as used by qegui, designer, etc.) expect plugin libraries
# to be located in a directory called designer, i.e. like:  ...../designer/libQEPlugin.so
# and QT_PLUGIN_PATH exdludes the /designer part of the path.
#
# However, users building against this, esp using EPICS make system,
# are expecting a file path like:  ...../lib/linux-x86_64/libQEPlugin.so.
#
# We can't install in to both locations, but the the creation of a
# designer directory together with a link in allows the following:
#  ...../lib/linux-x86_64/designer/libQEPlugin.so
# which keeps both Qt and EPICS happy.
#
# Note: we always run this step
#
$(TARGET) : $(SOURCE_DIR)/$(MAKEFILE)  always
	@echo "=== Building $(LIBFILE) plugin"                      && \
	cd  $(SOURCE_DIR)                                           && \
	$(MAKE) -f $(MAKEFILE) -j 3 -w                              && \
	echo "=== Installing plugin headers"                        && \
	$(MAKE) -f $(MAKEFILE) install                              && \
	echo "=== Setting up designer directory/folder"             && \
	cd .. && cd $(TARGET_DIR)                                   && \
	$(MKDIR) designer  &&  cd designer                          && \
	$(RM) $(LIBFILE) && $(call LINKFILE)                        && \
	echo "=== Complete"


$(SOURCE_DIR)/$(MAKEFILE) : $(SOURCE_DIR)/$(PROJECT)
	@echo "=== Running qmake - generating $(MAKEFILE)"          && \
	cd  $(SOURCE_DIR)                                           && \
	qmake -o $(MAKEFILE) $(PROJECT) -r


# Do a qt clean, then delete all qmake generated Makefiles.
#
clean:
	cd $(SOURCE_DIR) && $(MAKE) -f $(MAKEFILE) clean || $(NOOP)
	cd $(SOURCE_DIR) && $(RM) $(MAKEFILE)


uninstall:
	rm -f $(TARGET)

always:

# end
