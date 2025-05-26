# File: qeVCLTestApp/src/Makefile
# DateTime: Mon May 26 17:17:30 2025
# Last checked in by: starritt
#

TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================

#=============================
# Build the IOC application

PROD_IOC = qeVCLTest
# qeVCLTest.dbd will be created and installed
DBD += qeVCLTest.dbd

# qeVCLTest.dbd will be made up from these files:
qeVCLTest_DBD += base.dbd

# Include dbd files from all support applications:
#qeVCLTest_DBD += xxx.dbd

# Add all the support libraries needed by this IOC
#qeVCLTest_LIBS += xxx

# qeVCLTest_registerRecordDeviceDriver.cpp derives from qeVCLTest.dbd
qeVCLTest_SRCS += qeVCLTest_registerRecordDeviceDriver.cpp

# Build the main IOC entry point on workstation OSs.
qeVCLTest_SRCS_DEFAULT += qeVCLTestMain.cpp
qeVCLTest_SRCS_vxWorks += -nil-

# Add support from base/src/vxWorks if needed
#qeVCLTest_OBJS_vxWorks += $(EPICS_BASE_BIN)/vxComLibrary

# Finally link to the EPICS Base libraries
qeVCLTest_LIBS += $(EPICS_BASE_IOC_LIBS)

#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

# end
