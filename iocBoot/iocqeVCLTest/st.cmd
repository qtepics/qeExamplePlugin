#!../../bin/linux-x86_64/qeVCLTest

# $File: //ASP/tec/gui/qevcl/trunk/iocBoot/iocqeVCLTest/st.cmd $
# $Revision: #2 $
# $DateTime: 2021/03/23 17:16:58 $
# Last checked in by: $Author: starritt $
#

#- You may have to change qeVCLTest to something else
#- everywhere it appears in this file

< envPaths

cd "${TOP}"

## Register all support components
dbLoadDatabase "dbd/qeVCLTest.dbd"
qeVCLTest_registerRecordDeviceDriver pdbbase

## Load record instances
#
dbLoadTemplate ("db/dbl_slit_test.substitutions")
dbLoadRecords ("db/filter_screen.db", "DEVICE=MCTPDS01FS01, NAME=Tom's Filter")

cd "${TOP}/iocBoot/${IOC}"
iocInit

# end
