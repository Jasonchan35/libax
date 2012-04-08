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
	${OBJECTDIR}/_ext/341740327/axDBConn_SQLite3.o \
	${OBJECTDIR}/_ext/341740327/axDBStmt_SQLite3.o


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
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${libax_path}/lib/${axPLATFORM_PATH}/${CND_CONF}/libaxSQLite3.a

${libax_path}/lib/${axPLATFORM_PATH}/${CND_CONF}/libaxSQLite3.a: ${OBJECTFILES}
	${MKDIR} -p ${libax_path}/lib/${axPLATFORM_PATH}/${CND_CONF}
	${RM} ${libax_path}/lib/${axPLATFORM_PATH}/${CND_CONF}/libaxSQLite3.a
	${AR} -rv ${libax_path}/lib/${axPLATFORM_PATH}/${CND_CONF}/libaxSQLite3.a ${OBJECTFILES} 
	$(RANLIB) ${libax_path}/lib/${axPLATFORM_PATH}/${CND_CONF}/libaxSQLite3.a

${OBJECTDIR}/_ext/341740327/axDBConn_SQLite3.o: ../../../../src/database/axSQLite3/axDBConn_SQLite3.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/341740327
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/341740327/axDBConn_SQLite3.o ../../../../src/database/axSQLite3/axDBConn_SQLite3.cpp

${OBJECTDIR}/_ext/341740327/axDBStmt_SQLite3.o: ../../../../src/database/axSQLite3/axDBStmt_SQLite3.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/341740327
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/341740327/axDBStmt_SQLite3.o ../../../../src/database/axSQLite3/axDBStmt_SQLite3.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${libax_path}/lib/${axPLATFORM_PATH}/${CND_CONF}/libaxSQLite3.a

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
