# File: configure/Makefile
# DateTime: Mon May 26 17:17:28 2025
# Last checked in by: starritt
#

TOP=..

include $(TOP)/configure/CONFIG

TARGETS = $(CONFIG_TARGETS)
CONFIGS += $(subst ../,,$(wildcard $(CONFIG_INSTALLS)))

include $(TOP)/configure/RULES

# end
