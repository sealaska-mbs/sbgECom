#!/bin/sh
# This script is used to build the sbgCom library on unix systems.

# Define the GCC options according to the selected endianness
gccOptions="-I ../../common/ -c -Wall "

# Create the intermediate directory		
mkdir obj

# Create all objects for the SBG Systems C Common Lib
gcc $gccOptions ../../common/crc/sbgCrc.c -o obj/sbgCrc.o
gcc $gccOptions ../../common/interfaces/sbgInterfaceFile.c -o obj/sbgInterfaceFile.o
gcc $gccOptions ../../common/interfaces/sbgInterfaceSerialUnix.c -o obj/sbgInterfaceSerialUnix.o
gcc $gccOptions ../../common/interfaces/sbgInterfaceUdp.c -o obj/sbgInterfaceUdp.o
gcc $gccOptions ../../common/platform/sbgPlatform.c -o obj/sbgPlatform.o

# Create all objects for binary logs directory
gcc $gccOptions ../../src/binaryLogs/sbgEComBinaryLogDebug.c -o obj/sbgEComBinaryLogDebug.o
gcc $gccOptions ../../src/binaryLogs/sbgEComBinaryLogDvl.c -o obj/sbgEComBinaryLogDvl.o
gcc $gccOptions ../../src/binaryLogs/sbgEComBinaryLogEkf.c -o obj/sbgEComBinaryLogEkf.o
gcc $gccOptions ../../src/binaryLogs/sbgEComBinaryLogEvent.c -o obj/sbgEComBinaryLogEvent.o
gcc $gccOptions ../../src/binaryLogs/sbgEComBinaryLogGps.c -o obj/sbgEComBinaryLogGps.o
gcc $gccOptions ../../src/binaryLogs/sbgEComBinaryLogImu.c -o obj/sbgEComBinaryLogImu.o
gcc $gccOptions ../../src/binaryLogs/sbgEComBinaryLogMag.c -o obj/sbgEComBinaryLogMag.o
gcc $gccOptions ../../src/binaryLogs/sbgEComBinaryLogOdometer.c -o obj/sbgEComBinaryLogOdometer.o
gcc $gccOptions ../../src/binaryLogs/sbgEComBinaryLogPressure.c -o obj/sbgEComBinaryLogPressure.o
gcc $gccOptions ../../src/binaryLogs/sbgEComBinaryLogs.c -o obj/sbgEComBinaryLogs.o
gcc $gccOptions ../../src/binaryLogs/sbgEComBinaryLogShipMotion.c -o obj/sbgEComBinaryLogShipMotion.o
gcc $gccOptions ../../src/binaryLogs/sbgEComBinaryLogStatus.c -o obj/sbgEComBinaryLogStatus.o
gcc $gccOptions ../../src/binaryLogs/sbgEComBinaryLogUsbl.c -o obj/sbgEComBinaryLogUsbl.o
gcc $gccOptions ../../src/binaryLogs/sbgEComBinaryLogUtc.c -o obj/sbgEComBinaryLogUtc.o

# Create all objects for commands directory
gcc $gccOptions ../../src/commands/sbgEComCmdAdvanced.c -o obj/sbgEComCmdAdvanced.o
gcc $gccOptions ../../src/commands/sbgEComCmdCommon.c -o obj/sbgEComCmdCommon.o
gcc $gccOptions ../../src/commands/sbgEComCmdEvent.c -o obj/sbgEComCmdEvent.o
gcc $gccOptions ../../src/commands/sbgEComCmdFeatures.c -o obj/sbgEComCmdFeatures.o
gcc $gccOptions ../../src/commands/sbgEComCmdGnss.c -o obj/sbgEComCmdGnss.o
gcc $gccOptions ../../src/commands/sbgEComCmdInfo.c -o obj/sbgEComCmdInfo.o
gcc $gccOptions ../../src/commands/sbgEComCmdInterface.c -o obj/sbgEComCmdInterface.o
gcc $gccOptions ../../src/commands/sbgEComCmdLicense.c -o obj/sbgEComCmdLicense.o
gcc $gccOptions ../../src/commands/sbgEComCmdMag.c -o obj/sbgEComCmdMag.o
gcc $gccOptions ../../src/commands/sbgEComCmdOdo.c -o obj/sbgEComCmdOdo.o
gcc $gccOptions ../../src/commands/sbgEComCmdOutput.c -o obj/sbgEComCmdOutput.o
gcc $gccOptions ../../src/commands/sbgEComCmdSensor.c -o obj/sbgEComCmdSensor.o
gcc $gccOptions ../../src/commands/sbgEComCmdSettings.c -o obj/sbgEComCmdSettings.o
gcc $gccOptions ../../src/commands/transfer/sbgEComTransfer.c -o obj/sbgEComTransfer.o

# Create all objects for protocol directory
gcc $gccOptions ../../src/protocol/sbgEComProtocol.c -o obj/sbgEComProtocol.o

# Create all objets for the root directory
gcc $gccOptions ../../src/sbgECom.c -o obj/sbgECom.o

# Create the library
ar cr ../../libSbgECom.a obj/sbgCrc.o obj/sbgInterfaceFile.o obj/sbgInterfaceSerialUnix.o obj/sbgInterfaceUdp.o obj/sbgPlatform.o obj/sbgEComBinaryLogDebug.o obj/sbgEComBinaryLogDvl.o obj/sbgEComBinaryLogEkf.o obj/sbgEComBinaryLogEvent.o obj/sbgEComBinaryLogGps.o obj/sbgEComBinaryLogImu.o obj/sbgEComBinaryLogMag.o obj/sbgEComBinaryLogOdometer.o obj/sbgEComBinaryLogPressure.o obj/sbgEComBinaryLogs.o obj/sbgEComBinaryLogShipMotion.o obj/sbgEComBinaryLogStatus.o obj/sbgEComBinaryLogUsbl.o obj/sbgEComBinaryLogUtc.o obj/sbgEComCmdAdvanced.o obj/sbgEComCmdCommon.o obj/sbgEComCmdEvent.o obj/sbgEComCmdFeatures.o obj/sbgEComCmdGnss.o obj/sbgEComCmdInfo.o obj/sbgEComCmdInterface.o obj/sbgEComCmdLicense.o obj/sbgEComCmdMag.o obj/sbgEComCmdOdo.o obj/sbgEComCmdOutput.o obj/sbgEComCmdSensor.o obj/sbgEComCmdSettings.o obj/sbgEComTransfer.o obj/sbgEComProtocol.o obj/sbgECom.o