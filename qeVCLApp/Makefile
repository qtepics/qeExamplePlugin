# File: qeVCLApp/Makefile
# DateTime: Mon May 26 17:17:29 2025
# Last checked in by: starritt
#

TOP = ..
include $(TOP)/configure/CONFIG
DIRS := $(DIRS) $(filter-out $(DIRS), $(wildcard *src*))
DIRS := $(DIRS) $(filter-out $(DIRS), $(wildcard *Db*))
DIRS := $(DIRS) $(filter-out $(DIRS), $(wildcard *ui*))
include $(TOP)/configure/RULES_DIRS

# end
