#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
GREP=grep
NM=nm
CCADMIN=CCadmin
RANLIB=ranlib
CC=gcc
CCC=g++
CXX=g++
FC=gfortran
AS=as

# Macros
CND_PLATFORM=GNU-Linux-x86
CND_CONF=Release
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/_ext/877439419/axExecute.o \
	${OBJECTDIR}/_ext/801113954/axRect2.o \
	${OBJECTDIR}/_ext/801113954/axVec4.o \
	${OBJECTDIR}/_ext/877439419/axCommandLineApp.o \
	${OBJECTDIR}/_ext/615804755/axLangTable.o \
	${OBJECTDIR}/_ext/985522123/axIniParser.o \
	${OBJECTDIR}/_ext/1592309743/axDBConn.o \
	${OBJECTDIR}/_ext/864554464/axCondVar.o \
	${OBJECTDIR}/_ext/1641885700/axMemMap.o \
	${OBJECTDIR}/_ext/1641885700/axDirCopyStatus.o \
	${OBJECTDIR}/_ext/864554464/axThreadPool.o \
	${OBJECTDIR}/_ext/877439419/axSharedLibrary.o \
	${OBJECTDIR}/_ext/864554464/axThread.o \
	${OBJECTDIR}/_ext/877439419/axLog.o \
	${OBJECTDIR}/_ext/864554464/axMutex.o \
	${OBJECTDIR}/_ext/801113954/axQuaternion.o \
	${OBJECTDIR}/_ext/877439419/ax_poll.o \
	${OBJECTDIR}/_ext/801113954/axVec_Str.o \
	${OBJECTDIR}/_ext/1641885700/axFileSystem.o \
	${OBJECTDIR}/_ext/1641885700/axFile.o \
	${OBJECTDIR}/_ext/801113954/axVec3.o \
	${OBJECTDIR}/_ext/1592309743/axDBStmt.o \
	${OBJECTDIR}/_ext/877439419/axApp.o \
	${OBJECTDIR}/_ext/1134219961/axSocket.o \
	${OBJECTDIR}/_ext/801113954/axRay3.o \
	${OBJECTDIR}/_ext/877439419/axAndroid.o \
	${OBJECTDIR}/_ext/294757390/precompiled_header.o \
	${OBJECTDIR}/_ext/801113954/axFrustum3.o \
	${OBJECTDIR}/_ext/1134219961/axSockAddr.o \
	${OBJECTDIR}/_ext/877439419/axSystem.o \
	${OBJECTDIR}/_ext/801113954/axEulerRotation3.o \
	${OBJECTDIR}/_ext/801113954/axMatrix4.o \
	${OBJECTDIR}/_ext/1641885700/axDir.o \
	${OBJECTDIR}/_ext/985522123/axCSVParser.o \
	${OBJECTDIR}/_ext/1641885700/axFileStream.o \
	${OBJECTDIR}/_ext/801113954/axIntersect3.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=${axCCFLAGS}
CXXFLAGS=${axCCFLAGS}

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ../../../../../lib/${axPLATFORM_PATH}/${CND_CONF}/libax.a

../../../../../lib/${axPLATFORM_PATH}/${CND_CONF}/libax.a: ${OBJECTFILES}
	${MKDIR} -p ../../../../../lib/${axPLATFORM_PATH}/${CND_CONF}
	${RM} ../../../../../lib/${axPLATFORM_PATH}/${CND_CONF}/libax.a
	${AR} -rv ../../../../../lib/${axPLATFORM_PATH}/${CND_CONF}/libax.a ${OBJECTFILES} 
	$(RANLIB) ../../../../../lib/${axPLATFORM_PATH}/${CND_CONF}/libax.a

${OBJECTDIR}/_ext/877439419/axExecute.o: ../../../src/core/system/axExecute.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/877439419
	${RM} $@.d
	$(COMPILE.cc) -O2 -Wall -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/877439419/axExecute.o ../../../src/core/system/axExecute.cpp

${OBJECTDIR}/_ext/801113954/axRect2.o: ../../../src/core/math/axRect2.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/801113954
	${RM} $@.d
	$(COMPILE.cc) -O2 -Wall -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/801113954/axRect2.o ../../../src/core/math/axRect2.cpp

${OBJECTDIR}/_ext/801113954/axVec4.o: ../../../src/core/math/axVec4.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/801113954
	${RM} $@.d
	$(COMPILE.cc) -O2 -Wall -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/801113954/axVec4.o ../../../src/core/math/axVec4.cpp

${OBJECTDIR}/_ext/877439419/axCommandLineApp.o: ../../../src/core/system/axCommandLineApp.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/877439419
	${RM} $@.d
	$(COMPILE.cc) -O2 -Wall -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/877439419/axCommandLineApp.o ../../../src/core/system/axCommandLineApp.cpp

${OBJECTDIR}/_ext/615804755/axLangTable.o: ../../../src/core/lang_table/axLangTable.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/615804755
	${RM} $@.d
	$(COMPILE.cc) -O2 -Wall -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/615804755/axLangTable.o ../../../src/core/lang_table/axLangTable.cpp

${OBJECTDIR}/_ext/985522123/axIniParser.o: ../../../src/core/parser/axIniParser.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/985522123
	${RM} $@.d
	$(COMPILE.cc) -O2 -Wall -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/985522123/axIniParser.o ../../../src/core/parser/axIniParser.cpp

${OBJECTDIR}/_ext/1592309743/axDBConn.o: ../../../src/core/database/axDBConn.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1592309743
	${RM} $@.d
	$(COMPILE.cc) -O2 -Wall -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1592309743/axDBConn.o ../../../src/core/database/axDBConn.cpp

${OBJECTDIR}/_ext/864554464/axCondVar.o: ../../../src/core/thread/axCondVar.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/864554464
	${RM} $@.d
	$(COMPILE.cc) -O2 -Wall -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/864554464/axCondVar.o ../../../src/core/thread/axCondVar.cpp

${OBJECTDIR}/_ext/1641885700/axMemMap.o: ../../../src/core/file_system/axMemMap.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1641885700
	${RM} $@.d
	$(COMPILE.cc) -O2 -Wall -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1641885700/axMemMap.o ../../../src/core/file_system/axMemMap.cpp

${OBJECTDIR}/_ext/1641885700/axDirCopyStatus.o: ../../../src/core/file_system/axDirCopyStatus.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1641885700
	${RM} $@.d
	$(COMPILE.cc) -O2 -Wall -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1641885700/axDirCopyStatus.o ../../../src/core/file_system/axDirCopyStatus.cpp

${OBJECTDIR}/_ext/864554464/axThreadPool.o: ../../../src/core/thread/axThreadPool.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/864554464
	${RM} $@.d
	$(COMPILE.cc) -O2 -Wall -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/864554464/axThreadPool.o ../../../src/core/thread/axThreadPool.cpp

${OBJECTDIR}/_ext/877439419/axSharedLibrary.o: ../../../src/core/system/axSharedLibrary.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/877439419
	${RM} $@.d
	$(COMPILE.cc) -O2 -Wall -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/877439419/axSharedLibrary.o ../../../src/core/system/axSharedLibrary.cpp

${OBJECTDIR}/_ext/864554464/axThread.o: ../../../src/core/thread/axThread.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/864554464
	${RM} $@.d
	$(COMPILE.cc) -O2 -Wall -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/864554464/axThread.o ../../../src/core/thread/axThread.cpp

${OBJECTDIR}/_ext/877439419/axLog.o: ../../../src/core/system/axLog.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/877439419
	${RM} $@.d
	$(COMPILE.cc) -O2 -Wall -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/877439419/axLog.o ../../../src/core/system/axLog.cpp

${OBJECTDIR}/_ext/864554464/axMutex.o: ../../../src/core/thread/axMutex.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/864554464
	${RM} $@.d
	$(COMPILE.cc) -O2 -Wall -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/864554464/axMutex.o ../../../src/core/thread/axMutex.cpp

${OBJECTDIR}/_ext/801113954/axQuaternion.o: ../../../src/core/math/axQuaternion.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/801113954
	${RM} $@.d
	$(COMPILE.cc) -O2 -Wall -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/801113954/axQuaternion.o ../../../src/core/math/axQuaternion.cpp

${OBJECTDIR}/_ext/877439419/ax_poll.o: ../../../src/core/system/ax_poll.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/877439419
	${RM} $@.d
	$(COMPILE.cc) -O2 -Wall -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/877439419/ax_poll.o ../../../src/core/system/ax_poll.cpp

${OBJECTDIR}/_ext/801113954/axVec_Str.o: ../../../src/core/math/axVec_Str.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/801113954
	${RM} $@.d
	$(COMPILE.cc) -O2 -Wall -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/801113954/axVec_Str.o ../../../src/core/math/axVec_Str.cpp

${OBJECTDIR}/_ext/1641885700/axFileSystem.o: ../../../src/core/file_system/axFileSystem.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1641885700
	${RM} $@.d
	$(COMPILE.cc) -O2 -Wall -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1641885700/axFileSystem.o ../../../src/core/file_system/axFileSystem.cpp

${OBJECTDIR}/_ext/1641885700/axFile.o: ../../../src/core/file_system/axFile.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1641885700
	${RM} $@.d
	$(COMPILE.cc) -O2 -Wall -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1641885700/axFile.o ../../../src/core/file_system/axFile.cpp

${OBJECTDIR}/_ext/801113954/axVec3.o: ../../../src/core/math/axVec3.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/801113954
	${RM} $@.d
	$(COMPILE.cc) -O2 -Wall -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/801113954/axVec3.o ../../../src/core/math/axVec3.cpp

${OBJECTDIR}/_ext/1592309743/axDBStmt.o: ../../../src/core/database/axDBStmt.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1592309743
	${RM} $@.d
	$(COMPILE.cc) -O2 -Wall -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1592309743/axDBStmt.o ../../../src/core/database/axDBStmt.cpp

${OBJECTDIR}/_ext/877439419/axApp.o: ../../../src/core/system/axApp.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/877439419
	${RM} $@.d
	$(COMPILE.cc) -O2 -Wall -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/877439419/axApp.o ../../../src/core/system/axApp.cpp

${OBJECTDIR}/_ext/1134219961/axSocket.o: ../../../src/core/net/axSocket.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1134219961
	${RM} $@.d
	$(COMPILE.cc) -O2 -Wall -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1134219961/axSocket.o ../../../src/core/net/axSocket.cpp

${OBJECTDIR}/_ext/801113954/axRay3.o: ../../../src/core/math/axRay3.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/801113954
	${RM} $@.d
	$(COMPILE.cc) -O2 -Wall -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/801113954/axRay3.o ../../../src/core/math/axRay3.cpp

${OBJECTDIR}/_ext/877439419/axAndroid.o: ../../../src/core/system/axAndroid.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/877439419
	${RM} $@.d
	$(COMPILE.cc) -O2 -Wall -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/877439419/axAndroid.o ../../../src/core/system/axAndroid.cpp

${OBJECTDIR}/_ext/294757390/precompiled_header.o: ../../../src/core/precompiled_header/precompiled_header.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/294757390
	${RM} $@.d
	$(COMPILE.cc) -O2 -Wall -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/294757390/precompiled_header.o ../../../src/core/precompiled_header/precompiled_header.cpp

${OBJECTDIR}/_ext/801113954/axFrustum3.o: ../../../src/core/math/axFrustum3.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/801113954
	${RM} $@.d
	$(COMPILE.cc) -O2 -Wall -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/801113954/axFrustum3.o ../../../src/core/math/axFrustum3.cpp

${OBJECTDIR}/_ext/1134219961/axSockAddr.o: ../../../src/core/net/axSockAddr.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1134219961
	${RM} $@.d
	$(COMPILE.cc) -O2 -Wall -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1134219961/axSockAddr.o ../../../src/core/net/axSockAddr.cpp

${OBJECTDIR}/_ext/877439419/axSystem.o: ../../../src/core/system/axSystem.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/877439419
	${RM} $@.d
	$(COMPILE.cc) -O2 -Wall -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/877439419/axSystem.o ../../../src/core/system/axSystem.cpp

${OBJECTDIR}/_ext/801113954/axEulerRotation3.o: ../../../src/core/math/axEulerRotation3.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/801113954
	${RM} $@.d
	$(COMPILE.cc) -O2 -Wall -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/801113954/axEulerRotation3.o ../../../src/core/math/axEulerRotation3.cpp

${OBJECTDIR}/_ext/801113954/axMatrix4.o: ../../../src/core/math/axMatrix4.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/801113954
	${RM} $@.d
	$(COMPILE.cc) -O2 -Wall -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/801113954/axMatrix4.o ../../../src/core/math/axMatrix4.cpp

${OBJECTDIR}/_ext/1641885700/axDir.o: ../../../src/core/file_system/axDir.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1641885700
	${RM} $@.d
	$(COMPILE.cc) -O2 -Wall -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1641885700/axDir.o ../../../src/core/file_system/axDir.cpp

${OBJECTDIR}/_ext/985522123/axCSVParser.o: ../../../src/core/parser/axCSVParser.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/985522123
	${RM} $@.d
	$(COMPILE.cc) -O2 -Wall -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/985522123/axCSVParser.o ../../../src/core/parser/axCSVParser.cpp

${OBJECTDIR}/_ext/1641885700/axFileStream.o: ../../../src/core/file_system/axFileStream.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1641885700
	${RM} $@.d
	$(COMPILE.cc) -O2 -Wall -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1641885700/axFileStream.o ../../../src/core/file_system/axFileStream.cpp

${OBJECTDIR}/_ext/801113954/axIntersect3.o: ../../../src/core/math/axIntersect3.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/801113954
	${RM} $@.d
	$(COMPILE.cc) -O2 -Wall -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/801113954/axIntersect3.o ../../../src/core/math/axIntersect3.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ../../../../../lib/${axPLATFORM_PATH}/${CND_CONF}/libax.a

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
