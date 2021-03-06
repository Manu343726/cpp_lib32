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
CND_DLIB_EXT=so
CND_CONF=Debug
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/code/source/dl32Color.o \
	${OBJECTDIR}/code/source/dl32Math.o \
	${OBJECTDIR}/code/source/graphics/window/dl32Window.o \
	${OBJECTDIR}/code/source/graphics/window/xws/XWSScreenManager.o \
	${OBJECTDIR}/code/source/utils/dl32Config.o \
	${OBJECTDIR}/code/source/utils/dl32Demangling.o \
	${OBJECTDIR}/code/source/utils/dl32Memory.o \
	${OBJECTDIR}/code/tests/ASCII_raster/ASCII_raster_test.o \
	${OBJECTDIR}/code/tests/refactoring/Redesigned_event_system_test.o \
	${OBJECTDIR}/code/tests/refactoring/console_color_test.o \
	${OBJECTDIR}/code/tests/refactoring/dl32Event_test.o \
	${OBJECTDIR}/code/tests/refactoring/dl32Window_test.o \
	${OBJECTDIR}/code/tests/refactoring/is_design_test.o \
	${OBJECTDIR}/code/tests/refactoring/minimax.o \
	${OBJECTDIR}/code/tests/refactoring/new_math_test.o \
	${OBJECTDIR}/code/tests/refactoring/quicksort.o \
	${OBJECTDIR}/code/tests/refactoring/typing_test.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=-ftemplate-depth=10000
CXXFLAGS=-ftemplate-depth=10000

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=-lX11

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/cpp_lib32

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/cpp_lib32: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/cpp_lib32 ${OBJECTFILES} ${LDLIBSOPTIONS}

${OBJECTDIR}/code/source/dl32Color.o: code/source/dl32Color.cpp 
	${MKDIR} -p ${OBJECTDIR}/code/source
	${RM} $@.d
	$(COMPILE.cc) -g -Icode/headers -Icode/source -Icode/tests -Icode/headers/utils -Icode/headers/utils/metaprogramming_library -Icode/source/utils -Icode/tests/ASCII_raster -Icode/tests/ASCII_raster/ascii -Icode/tests/ASCII_raster/graphics -Icode/tests/refactoring -Icode/headers/math -Icode/headers/graphics -Icode/headers/graphics/window -Icode/headers/events -std=c++11 -ftemplate-depth=10000 -MMD -MP -MF $@.d -o ${OBJECTDIR}/code/source/dl32Color.o code/source/dl32Color.cpp

${OBJECTDIR}/code/source/dl32Math.o: code/source/dl32Math.cpp 
	${MKDIR} -p ${OBJECTDIR}/code/source
	${RM} $@.d
	$(COMPILE.cc) -g -Icode/headers -Icode/source -Icode/tests -Icode/headers/utils -Icode/headers/utils/metaprogramming_library -Icode/source/utils -Icode/tests/ASCII_raster -Icode/tests/ASCII_raster/ascii -Icode/tests/ASCII_raster/graphics -Icode/tests/refactoring -Icode/headers/math -Icode/headers/graphics -Icode/headers/graphics/window -Icode/headers/events -std=c++11 -ftemplate-depth=10000 -MMD -MP -MF $@.d -o ${OBJECTDIR}/code/source/dl32Math.o code/source/dl32Math.cpp

${OBJECTDIR}/code/source/graphics/window/dl32Window.o: code/source/graphics/window/dl32Window.cpp 
	${MKDIR} -p ${OBJECTDIR}/code/source/graphics/window
	${RM} $@.d
	$(COMPILE.cc) -g -Icode/headers -Icode/source -Icode/tests -Icode/headers/utils -Icode/headers/utils/metaprogramming_library -Icode/source/utils -Icode/tests/ASCII_raster -Icode/tests/ASCII_raster/ascii -Icode/tests/ASCII_raster/graphics -Icode/tests/refactoring -Icode/headers/math -Icode/headers/graphics -Icode/headers/graphics/window -Icode/headers/events -std=c++11 -ftemplate-depth=10000 -MMD -MP -MF $@.d -o ${OBJECTDIR}/code/source/graphics/window/dl32Window.o code/source/graphics/window/dl32Window.cpp

${OBJECTDIR}/code/source/graphics/window/xws/XWSScreenManager.o: code/source/graphics/window/xws/XWSScreenManager.cpp 
	${MKDIR} -p ${OBJECTDIR}/code/source/graphics/window/xws
	${RM} $@.d
	$(COMPILE.cc) -g -Icode/headers -Icode/source -Icode/tests -Icode/headers/utils -Icode/headers/utils/metaprogramming_library -Icode/source/utils -Icode/tests/ASCII_raster -Icode/tests/ASCII_raster/ascii -Icode/tests/ASCII_raster/graphics -Icode/tests/refactoring -Icode/headers/math -Icode/headers/graphics -Icode/headers/graphics/window -Icode/headers/events -std=c++11 -ftemplate-depth=10000 -MMD -MP -MF $@.d -o ${OBJECTDIR}/code/source/graphics/window/xws/XWSScreenManager.o code/source/graphics/window/xws/XWSScreenManager.cpp

${OBJECTDIR}/code/source/utils/dl32Config.o: code/source/utils/dl32Config.cpp 
	${MKDIR} -p ${OBJECTDIR}/code/source/utils
	${RM} $@.d
	$(COMPILE.cc) -g -Icode/headers -Icode/source -Icode/tests -Icode/headers/utils -Icode/headers/utils/metaprogramming_library -Icode/source/utils -Icode/tests/ASCII_raster -Icode/tests/ASCII_raster/ascii -Icode/tests/ASCII_raster/graphics -Icode/tests/refactoring -Icode/headers/math -Icode/headers/graphics -Icode/headers/graphics/window -Icode/headers/events -std=c++11 -ftemplate-depth=10000 -MMD -MP -MF $@.d -o ${OBJECTDIR}/code/source/utils/dl32Config.o code/source/utils/dl32Config.cpp

${OBJECTDIR}/code/source/utils/dl32Demangling.o: code/source/utils/dl32Demangling.cpp 
	${MKDIR} -p ${OBJECTDIR}/code/source/utils
	${RM} $@.d
	$(COMPILE.cc) -g -Icode/headers -Icode/source -Icode/tests -Icode/headers/utils -Icode/headers/utils/metaprogramming_library -Icode/source/utils -Icode/tests/ASCII_raster -Icode/tests/ASCII_raster/ascii -Icode/tests/ASCII_raster/graphics -Icode/tests/refactoring -Icode/headers/math -Icode/headers/graphics -Icode/headers/graphics/window -Icode/headers/events -std=c++11 -ftemplate-depth=10000 -MMD -MP -MF $@.d -o ${OBJECTDIR}/code/source/utils/dl32Demangling.o code/source/utils/dl32Demangling.cpp

${OBJECTDIR}/code/source/utils/dl32Memory.o: code/source/utils/dl32Memory.cpp 
	${MKDIR} -p ${OBJECTDIR}/code/source/utils
	${RM} $@.d
	$(COMPILE.cc) -g -Icode/headers -Icode/source -Icode/tests -Icode/headers/utils -Icode/headers/utils/metaprogramming_library -Icode/source/utils -Icode/tests/ASCII_raster -Icode/tests/ASCII_raster/ascii -Icode/tests/ASCII_raster/graphics -Icode/tests/refactoring -Icode/headers/math -Icode/headers/graphics -Icode/headers/graphics/window -Icode/headers/events -std=c++11 -ftemplate-depth=10000 -MMD -MP -MF $@.d -o ${OBJECTDIR}/code/source/utils/dl32Memory.o code/source/utils/dl32Memory.cpp

${OBJECTDIR}/code/tests/ASCII_raster/ASCII_raster_test.o: code/tests/ASCII_raster/ASCII_raster_test.cpp 
	${MKDIR} -p ${OBJECTDIR}/code/tests/ASCII_raster
	${RM} $@.d
	$(COMPILE.cc) -g -Icode/headers -Icode/source -Icode/tests -Icode/headers/utils -Icode/headers/utils/metaprogramming_library -Icode/source/utils -Icode/tests/ASCII_raster -Icode/tests/ASCII_raster/ascii -Icode/tests/ASCII_raster/graphics -Icode/tests/refactoring -Icode/headers/math -Icode/headers/graphics -Icode/headers/graphics/window -Icode/headers/events -std=c++11 -ftemplate-depth=10000 -MMD -MP -MF $@.d -o ${OBJECTDIR}/code/tests/ASCII_raster/ASCII_raster_test.o code/tests/ASCII_raster/ASCII_raster_test.cpp

${OBJECTDIR}/code/tests/refactoring/Redesigned_event_system_test.o: code/tests/refactoring/Redesigned_event_system_test.cpp 
	${MKDIR} -p ${OBJECTDIR}/code/tests/refactoring
	${RM} $@.d
	$(COMPILE.cc) -g -Icode/headers -Icode/source -Icode/tests -Icode/headers/utils -Icode/headers/utils/metaprogramming_library -Icode/source/utils -Icode/tests/ASCII_raster -Icode/tests/ASCII_raster/ascii -Icode/tests/ASCII_raster/graphics -Icode/tests/refactoring -Icode/headers/math -Icode/headers/graphics -Icode/headers/graphics/window -Icode/headers/events -std=c++11 -ftemplate-depth=10000 -MMD -MP -MF $@.d -o ${OBJECTDIR}/code/tests/refactoring/Redesigned_event_system_test.o code/tests/refactoring/Redesigned_event_system_test.cpp

${OBJECTDIR}/code/tests/refactoring/console_color_test.o: code/tests/refactoring/console_color_test.cpp 
	${MKDIR} -p ${OBJECTDIR}/code/tests/refactoring
	${RM} $@.d
	$(COMPILE.cc) -g -Icode/headers -Icode/source -Icode/tests -Icode/headers/utils -Icode/headers/utils/metaprogramming_library -Icode/source/utils -Icode/tests/ASCII_raster -Icode/tests/ASCII_raster/ascii -Icode/tests/ASCII_raster/graphics -Icode/tests/refactoring -Icode/headers/math -Icode/headers/graphics -Icode/headers/graphics/window -Icode/headers/events -std=c++11 -ftemplate-depth=10000 -MMD -MP -MF $@.d -o ${OBJECTDIR}/code/tests/refactoring/console_color_test.o code/tests/refactoring/console_color_test.cpp

${OBJECTDIR}/code/tests/refactoring/dl32Event_test.o: code/tests/refactoring/dl32Event_test.cpp 
	${MKDIR} -p ${OBJECTDIR}/code/tests/refactoring
	${RM} $@.d
	$(COMPILE.cc) -g -Icode/headers -Icode/source -Icode/tests -Icode/headers/utils -Icode/headers/utils/metaprogramming_library -Icode/source/utils -Icode/tests/ASCII_raster -Icode/tests/ASCII_raster/ascii -Icode/tests/ASCII_raster/graphics -Icode/tests/refactoring -Icode/headers/math -Icode/headers/graphics -Icode/headers/graphics/window -Icode/headers/events -std=c++11 -ftemplate-depth=10000 -MMD -MP -MF $@.d -o ${OBJECTDIR}/code/tests/refactoring/dl32Event_test.o code/tests/refactoring/dl32Event_test.cpp

${OBJECTDIR}/code/tests/refactoring/dl32Window_test.o: code/tests/refactoring/dl32Window_test.cpp 
	${MKDIR} -p ${OBJECTDIR}/code/tests/refactoring
	${RM} $@.d
	$(COMPILE.cc) -g -Icode/headers -Icode/source -Icode/tests -Icode/headers/utils -Icode/headers/utils/metaprogramming_library -Icode/source/utils -Icode/tests/ASCII_raster -Icode/tests/ASCII_raster/ascii -Icode/tests/ASCII_raster/graphics -Icode/tests/refactoring -Icode/headers/math -Icode/headers/graphics -Icode/headers/graphics/window -Icode/headers/events -std=c++11 -ftemplate-depth=10000 -MMD -MP -MF $@.d -o ${OBJECTDIR}/code/tests/refactoring/dl32Window_test.o code/tests/refactoring/dl32Window_test.cpp

${OBJECTDIR}/code/tests/refactoring/is_design_test.o: code/tests/refactoring/is_design_test.cpp 
	${MKDIR} -p ${OBJECTDIR}/code/tests/refactoring
	${RM} $@.d
	$(COMPILE.cc) -g -Icode/headers -Icode/source -Icode/tests -Icode/headers/utils -Icode/headers/utils/metaprogramming_library -Icode/source/utils -Icode/tests/ASCII_raster -Icode/tests/ASCII_raster/ascii -Icode/tests/ASCII_raster/graphics -Icode/tests/refactoring -Icode/headers/math -Icode/headers/graphics -Icode/headers/graphics/window -Icode/headers/events -std=c++11 -ftemplate-depth=10000 -MMD -MP -MF $@.d -o ${OBJECTDIR}/code/tests/refactoring/is_design_test.o code/tests/refactoring/is_design_test.cpp

${OBJECTDIR}/code/tests/refactoring/minimax.o: code/tests/refactoring/minimax.cpp 
	${MKDIR} -p ${OBJECTDIR}/code/tests/refactoring
	${RM} $@.d
	$(COMPILE.cc) -g -Icode/headers -Icode/source -Icode/tests -Icode/headers/utils -Icode/headers/utils/metaprogramming_library -Icode/source/utils -Icode/tests/ASCII_raster -Icode/tests/ASCII_raster/ascii -Icode/tests/ASCII_raster/graphics -Icode/tests/refactoring -Icode/headers/math -Icode/headers/graphics -Icode/headers/graphics/window -Icode/headers/events -std=c++11 -ftemplate-depth=10000 -MMD -MP -MF $@.d -o ${OBJECTDIR}/code/tests/refactoring/minimax.o code/tests/refactoring/minimax.cpp

${OBJECTDIR}/code/tests/refactoring/new_math_test.o: code/tests/refactoring/new_math_test.cpp 
	${MKDIR} -p ${OBJECTDIR}/code/tests/refactoring
	${RM} $@.d
	$(COMPILE.cc) -g -Icode/headers -Icode/source -Icode/tests -Icode/headers/utils -Icode/headers/utils/metaprogramming_library -Icode/source/utils -Icode/tests/ASCII_raster -Icode/tests/ASCII_raster/ascii -Icode/tests/ASCII_raster/graphics -Icode/tests/refactoring -Icode/headers/math -Icode/headers/graphics -Icode/headers/graphics/window -Icode/headers/events -std=c++11 -ftemplate-depth=10000 -MMD -MP -MF $@.d -o ${OBJECTDIR}/code/tests/refactoring/new_math_test.o code/tests/refactoring/new_math_test.cpp

${OBJECTDIR}/code/tests/refactoring/quicksort.o: code/tests/refactoring/quicksort.cpp 
	${MKDIR} -p ${OBJECTDIR}/code/tests/refactoring
	${RM} $@.d
	$(COMPILE.cc) -g -Icode/headers -Icode/source -Icode/tests -Icode/headers/utils -Icode/headers/utils/metaprogramming_library -Icode/source/utils -Icode/tests/ASCII_raster -Icode/tests/ASCII_raster/ascii -Icode/tests/ASCII_raster/graphics -Icode/tests/refactoring -Icode/headers/math -Icode/headers/graphics -Icode/headers/graphics/window -Icode/headers/events -std=c++11 -ftemplate-depth=10000 -MMD -MP -MF $@.d -o ${OBJECTDIR}/code/tests/refactoring/quicksort.o code/tests/refactoring/quicksort.cpp

${OBJECTDIR}/code/tests/refactoring/typing_test.o: code/tests/refactoring/typing_test.cpp 
	${MKDIR} -p ${OBJECTDIR}/code/tests/refactoring
	${RM} $@.d
	$(COMPILE.cc) -g -Icode/headers -Icode/source -Icode/tests -Icode/headers/utils -Icode/headers/utils/metaprogramming_library -Icode/source/utils -Icode/tests/ASCII_raster -Icode/tests/ASCII_raster/ascii -Icode/tests/ASCII_raster/graphics -Icode/tests/refactoring -Icode/headers/math -Icode/headers/graphics -Icode/headers/graphics/window -Icode/headers/events -std=c++11 -ftemplate-depth=10000 -MMD -MP -MF $@.d -o ${OBJECTDIR}/code/tests/refactoring/typing_test.o code/tests/refactoring/typing_test.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/cpp_lib32

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
