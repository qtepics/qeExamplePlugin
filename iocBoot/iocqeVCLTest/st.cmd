#!../../bin/linux-x86_64/qeVCLTest

# File: iocBoot/iocqeVCLTest/st.cmd
# DateTime: Mon May 26 17:17:29 2025
# Last checked in by: starritt
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
