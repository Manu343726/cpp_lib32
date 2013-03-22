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
CND_PLATFORM=Cygwin-Windows
CND_DLIB_EXT=dll
CND_CONF=Debug
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include dl32Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/code/source/utils/dl32Utils3D.o \
	${OBJECTDIR}/code/source/dl32Math.o \
	${OBJECTDIR}/code/source/dl32String.o \
	${OBJECTDIR}/code/source/utils/dl32Timing.o \
	${OBJECTDIR}/code/source/dl32Graphics.o \
	${OBJECTDIR}/code/source/utils/dl32Memory.o \
	${OBJECTDIR}/code/source/dl32Console.o \
	${OBJECTDIR}/code/source/dl32Window.o \
	${OBJECTDIR}/code/source/dl32Color.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=
CXXFLAGS=

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/dx_lib32.exe

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/dx_lib32.exe: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/dx_lib32 ${OBJECTFILES} ${LDLIBSOPTIONS} 

${OBJECTDIR}/code/source/utils/dl32Utils3D.o: code/source/utils/dl32Utils3D.cpp 
	${MKDIR} -p ${OBJECTDIR}/code/source/utils
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/code/source/utils/dl32Utils3D.o code/source/utils/dl32Utils3D.cpp

${OBJECTDIR}/code/source/dl32Math.o: code/source/dl32Math.cpp 
	${MKDIR} -p ${OBJECTDIR}/code/source
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/code/source/dl32Math.o code/source/dl32Math.cpp

${OBJECTDIR}/code/source/dl32String.o: code/source/dl32String.cpp 
	${MKDIR} -p ${OBJECTDIR}/code/source
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/code/source/dl32String.o code/source/dl32String.cpp

${OBJECTDIR}/code/source/utils/dl32Timing.o: code/source/utils/dl32Timing.cpp 
	${MKDIR} -p ${OBJECTDIR}/code/source/utils
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/code/source/utils/dl32Timing.o code/source/utils/dl32Timing.cpp

${OBJECTDIR}/code/source/dl32Graphics.o: code/source/dl32Graphics.cpp 
	${MKDIR} -p ${OBJECTDIR}/code/source
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/code/source/dl32Graphics.o code/source/dl32Graphics.cpp

${OBJECTDIR}/code/source/utils/dl32Memory.o: code/source/utils/dl32Memory.cpp 
	${MKDIR} -p ${OBJECTDIR}/code/source/utils
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/code/source/utils/dl32Memory.o code/source/utils/dl32Memory.cpp

${OBJECTDIR}/code/source/dl32Console.o: code/source/dl32Console.cpp 
	${MKDIR} -p ${OBJECTDIR}/code/source
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/code/source/dl32Console.o code/source/dl32Console.cpp

${OBJECTDIR}/code/source/dl32Window.o: code/source/dl32Window.cpp 
	${MKDIR} -p ${OBJECTDIR}/code/source
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/code/source/dl32Window.o code/source/dl32Window.cpp

${OBJECTDIR}/code/source/dl32Color.o: code/source/dl32Color.cpp 
	${MKDIR} -p ${OBJECTDIR}/code/source
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/code/source/dl32Color.o code/source/dl32Color.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/dx_lib32.exe

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
