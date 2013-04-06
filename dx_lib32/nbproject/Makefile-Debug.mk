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
CND_PLATFORM=MinGW-Windows
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
	${OBJECTDIR}/_ext/1028437842/dl32Color.o \
	${OBJECTDIR}/_ext/1028437842/dl32Console.o \
	${OBJECTDIR}/_ext/1028437842/dl32Graphics.o \
	${OBJECTDIR}/_ext/1028437842/dl32Math.o \
	${OBJECTDIR}/_ext/1028437842/dl32String.o \
	${OBJECTDIR}/_ext/1028437842/dl32Window.o \
	${OBJECTDIR}/_ext/1133222896/dl32Memory.o \
	${OBJECTDIR}/_ext/1133222896/dl32Timing.o \
	${OBJECTDIR}/_ext/1133222896/dl32Utils3D.o \
	${OBJECTDIR}/_ext/461600824/EcuationSolverTest.o \
	${OBJECTDIR}/_ext/461600824/GeneralStressTest.o \
	${OBJECTDIR}/_ext/461600824/MatrixTest.o \
	${OBJECTDIR}/_ext/92388514/IsometricEngine.o \
	${OBJECTDIR}/_ext/92388514/IsometricEngineTest.o \
	${OBJECTDIR}/_ext/1950470279/MandelbrotEngine.o \
	${OBJECTDIR}/_ext/1950470279/MandelbrotFractalTest.o \
	${OBJECTDIR}/_ext/695214021/Particle_engine.o \
	${OBJECTDIR}/_ext/695214021/ParticleEngineTest.o \
	${OBJECTDIR}/_ext/809142145/PolylineTest.o \
	${OBJECTDIR}/_ext/227780153/Pong.o


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
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libdx_lib32.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libdx_lib32.a: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libdx_lib32.a
	${AR} -rv ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libdx_lib32.a ${OBJECTFILES} 
	$(RANLIB) ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libdx_lib32.a

${OBJECTDIR}/_ext/1028437842/dl32Color.o: ../code/source/dl32Color.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1028437842
	${RM} $@.d
	$(COMPILE.cc) -g -I../code/headers -I../code/headers/utils -I../code/source -I../code/source/utils -I../code/tests -I../code/tests/general\ tests -I../code/tests/isometric\ tilemap -I../code/tests/mandlebrot -I../code/tests/particle\ engine -I../code/tests/polyline -I../code/tests/pong -I${DXSDK_DIR}/ -I/E/PROGRAMASXP/PROGRAMACION/DirectXSDKJunio2010/Include -std=c++11 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1028437842/dl32Color.o ../code/source/dl32Color.cpp

${OBJECTDIR}/_ext/1028437842/dl32Console.o: ../code/source/dl32Console.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1028437842
	${RM} $@.d
	$(COMPILE.cc) -g -I../code/headers -I../code/headers/utils -I../code/source -I../code/source/utils -I../code/tests -I../code/tests/general\ tests -I../code/tests/isometric\ tilemap -I../code/tests/mandlebrot -I../code/tests/particle\ engine -I../code/tests/polyline -I../code/tests/pong -I${DXSDK_DIR}/ -I/E/PROGRAMASXP/PROGRAMACION/DirectXSDKJunio2010/Include -std=c++11 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1028437842/dl32Console.o ../code/source/dl32Console.cpp

${OBJECTDIR}/_ext/1028437842/dl32Graphics.o: ../code/source/dl32Graphics.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1028437842
	${RM} $@.d
	$(COMPILE.cc) -g -I../code/headers -I../code/headers/utils -I../code/source -I../code/source/utils -I../code/tests -I../code/tests/general\ tests -I../code/tests/isometric\ tilemap -I../code/tests/mandlebrot -I../code/tests/particle\ engine -I../code/tests/polyline -I../code/tests/pong -I${DXSDK_DIR}/ -I/E/PROGRAMASXP/PROGRAMACION/DirectXSDKJunio2010/Include -std=c++11 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1028437842/dl32Graphics.o ../code/source/dl32Graphics.cpp

${OBJECTDIR}/_ext/1028437842/dl32Math.o: ../code/source/dl32Math.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1028437842
	${RM} $@.d
	$(COMPILE.cc) -g -I../code/headers -I../code/headers/utils -I../code/source -I../code/source/utils -I../code/tests -I../code/tests/general\ tests -I../code/tests/isometric\ tilemap -I../code/tests/mandlebrot -I../code/tests/particle\ engine -I../code/tests/polyline -I../code/tests/pong -I${DXSDK_DIR}/ -I/E/PROGRAMASXP/PROGRAMACION/DirectXSDKJunio2010/Include -std=c++11 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1028437842/dl32Math.o ../code/source/dl32Math.cpp

${OBJECTDIR}/_ext/1028437842/dl32String.o: ../code/source/dl32String.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1028437842
	${RM} $@.d
	$(COMPILE.cc) -g -I../code/headers -I../code/headers/utils -I../code/source -I../code/source/utils -I../code/tests -I../code/tests/general\ tests -I../code/tests/isometric\ tilemap -I../code/tests/mandlebrot -I../code/tests/particle\ engine -I../code/tests/polyline -I../code/tests/pong -I${DXSDK_DIR}/ -I/E/PROGRAMASXP/PROGRAMACION/DirectXSDKJunio2010/Include -std=c++11 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1028437842/dl32String.o ../code/source/dl32String.cpp

${OBJECTDIR}/_ext/1028437842/dl32Window.o: ../code/source/dl32Window.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1028437842
	${RM} $@.d
	$(COMPILE.cc) -g -I../code/headers -I../code/headers/utils -I../code/source -I../code/source/utils -I../code/tests -I../code/tests/general\ tests -I../code/tests/isometric\ tilemap -I../code/tests/mandlebrot -I../code/tests/particle\ engine -I../code/tests/polyline -I../code/tests/pong -I${DXSDK_DIR}/ -I/E/PROGRAMASXP/PROGRAMACION/DirectXSDKJunio2010/Include -std=c++11 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1028437842/dl32Window.o ../code/source/dl32Window.cpp

${OBJECTDIR}/_ext/1133222896/dl32Memory.o: ../code/source/utils/dl32Memory.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1133222896
	${RM} $@.d
	$(COMPILE.cc) -g -I../code/headers -I../code/headers/utils -I../code/source -I../code/source/utils -I../code/tests -I../code/tests/general\ tests -I../code/tests/isometric\ tilemap -I../code/tests/mandlebrot -I../code/tests/particle\ engine -I../code/tests/polyline -I../code/tests/pong -I${DXSDK_DIR}/ -I/E/PROGRAMASXP/PROGRAMACION/DirectXSDKJunio2010/Include -std=c++11 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1133222896/dl32Memory.o ../code/source/utils/dl32Memory.cpp

${OBJECTDIR}/_ext/1133222896/dl32Timing.o: ../code/source/utils/dl32Timing.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1133222896
	${RM} $@.d
	$(COMPILE.cc) -g -I../code/headers -I../code/headers/utils -I../code/source -I../code/source/utils -I../code/tests -I../code/tests/general\ tests -I../code/tests/isometric\ tilemap -I../code/tests/mandlebrot -I../code/tests/particle\ engine -I../code/tests/polyline -I../code/tests/pong -I${DXSDK_DIR}/ -I/E/PROGRAMASXP/PROGRAMACION/DirectXSDKJunio2010/Include -std=c++11 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1133222896/dl32Timing.o ../code/source/utils/dl32Timing.cpp

${OBJECTDIR}/_ext/1133222896/dl32Utils3D.o: ../code/source/utils/dl32Utils3D.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1133222896
	${RM} $@.d
	$(COMPILE.cc) -g -I../code/headers -I../code/headers/utils -I../code/source -I../code/source/utils -I../code/tests -I../code/tests/general\ tests -I../code/tests/isometric\ tilemap -I../code/tests/mandlebrot -I../code/tests/particle\ engine -I../code/tests/polyline -I../code/tests/pong -I${DXSDK_DIR}/ -I/E/PROGRAMASXP/PROGRAMACION/DirectXSDKJunio2010/Include -std=c++11 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1133222896/dl32Utils3D.o ../code/source/utils/dl32Utils3D.cpp

${OBJECTDIR}/_ext/461600824/EcuationSolverTest.o: ../code/tests/general\ tests/EcuationSolverTest.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/461600824
	${RM} $@.d
	$(COMPILE.cc) -g -I../code/headers -I../code/headers/utils -I../code/source -I../code/source/utils -I../code/tests -I../code/tests/general\ tests -I../code/tests/isometric\ tilemap -I../code/tests/mandlebrot -I../code/tests/particle\ engine -I../code/tests/polyline -I../code/tests/pong -I${DXSDK_DIR}/ -I/E/PROGRAMASXP/PROGRAMACION/DirectXSDKJunio2010/Include -std=c++11 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/461600824/EcuationSolverTest.o ../code/tests/general\ tests/EcuationSolverTest.cpp

${OBJECTDIR}/_ext/461600824/GeneralStressTest.o: ../code/tests/general\ tests/GeneralStressTest.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/461600824
	${RM} $@.d
	$(COMPILE.cc) -g -I../code/headers -I../code/headers/utils -I../code/source -I../code/source/utils -I../code/tests -I../code/tests/general\ tests -I../code/tests/isometric\ tilemap -I../code/tests/mandlebrot -I../code/tests/particle\ engine -I../code/tests/polyline -I../code/tests/pong -I${DXSDK_DIR}/ -I/E/PROGRAMASXP/PROGRAMACION/DirectXSDKJunio2010/Include -std=c++11 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/461600824/GeneralStressTest.o ../code/tests/general\ tests/GeneralStressTest.cpp

${OBJECTDIR}/_ext/461600824/MatrixTest.o: ../code/tests/general\ tests/MatrixTest.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/461600824
	${RM} $@.d
	$(COMPILE.cc) -g -I../code/headers -I../code/headers/utils -I../code/source -I../code/source/utils -I../code/tests -I../code/tests/general\ tests -I../code/tests/isometric\ tilemap -I../code/tests/mandlebrot -I../code/tests/particle\ engine -I../code/tests/polyline -I../code/tests/pong -I${DXSDK_DIR}/ -I/E/PROGRAMASXP/PROGRAMACION/DirectXSDKJunio2010/Include -std=c++11 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/461600824/MatrixTest.o ../code/tests/general\ tests/MatrixTest.cpp

${OBJECTDIR}/_ext/92388514/IsometricEngine.o: ../code/tests/isometric\ tilemap/IsometricEngine.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/92388514
	${RM} $@.d
	$(COMPILE.cc) -g -I../code/headers -I../code/headers/utils -I../code/source -I../code/source/utils -I../code/tests -I../code/tests/general\ tests -I../code/tests/isometric\ tilemap -I../code/tests/mandlebrot -I../code/tests/particle\ engine -I../code/tests/polyline -I../code/tests/pong -I${DXSDK_DIR}/ -I/E/PROGRAMASXP/PROGRAMACION/DirectXSDKJunio2010/Include -std=c++11 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/92388514/IsometricEngine.o ../code/tests/isometric\ tilemap/IsometricEngine.cpp

${OBJECTDIR}/_ext/92388514/IsometricEngineTest.o: ../code/tests/isometric\ tilemap/IsometricEngineTest.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/92388514
	${RM} $@.d
	$(COMPILE.cc) -g -I../code/headers -I../code/headers/utils -I../code/source -I../code/source/utils -I../code/tests -I../code/tests/general\ tests -I../code/tests/isometric\ tilemap -I../code/tests/mandlebrot -I../code/tests/particle\ engine -I../code/tests/polyline -I../code/tests/pong -I${DXSDK_DIR}/ -I/E/PROGRAMASXP/PROGRAMACION/DirectXSDKJunio2010/Include -std=c++11 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/92388514/IsometricEngineTest.o ../code/tests/isometric\ tilemap/IsometricEngineTest.cpp

${OBJECTDIR}/_ext/1950470279/MandelbrotEngine.o: ../code/tests/mandlebrot/MandelbrotEngine.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1950470279
	${RM} $@.d
	$(COMPILE.cc) -g -I../code/headers -I../code/headers/utils -I../code/source -I../code/source/utils -I../code/tests -I../code/tests/general\ tests -I../code/tests/isometric\ tilemap -I../code/tests/mandlebrot -I../code/tests/particle\ engine -I../code/tests/polyline -I../code/tests/pong -I${DXSDK_DIR}/ -I/E/PROGRAMASXP/PROGRAMACION/DirectXSDKJunio2010/Include -std=c++11 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1950470279/MandelbrotEngine.o ../code/tests/mandlebrot/MandelbrotEngine.cpp

${OBJECTDIR}/_ext/1950470279/MandelbrotFractalTest.o: ../code/tests/mandlebrot/MandelbrotFractalTest.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1950470279
	${RM} $@.d
	$(COMPILE.cc) -g -I../code/headers -I../code/headers/utils -I../code/source -I../code/source/utils -I../code/tests -I../code/tests/general\ tests -I../code/tests/isometric\ tilemap -I../code/tests/mandlebrot -I../code/tests/particle\ engine -I../code/tests/polyline -I../code/tests/pong -I${DXSDK_DIR}/ -I/E/PROGRAMASXP/PROGRAMACION/DirectXSDKJunio2010/Include -std=c++11 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1950470279/MandelbrotFractalTest.o ../code/tests/mandlebrot/MandelbrotFractalTest.cpp

${OBJECTDIR}/_ext/695214021/Particle_engine.o: ../code/tests/particle\ engine/Particle\ engine.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/695214021
	${RM} $@.d
	$(COMPILE.cc) -g -I../code/headers -I../code/headers/utils -I../code/source -I../code/source/utils -I../code/tests -I../code/tests/general\ tests -I../code/tests/isometric\ tilemap -I../code/tests/mandlebrot -I../code/tests/particle\ engine -I../code/tests/polyline -I../code/tests/pong -I${DXSDK_DIR}/ -I/E/PROGRAMASXP/PROGRAMACION/DirectXSDKJunio2010/Include -std=c++11 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/695214021/Particle_engine.o ../code/tests/particle\ engine/Particle\ engine.cpp

${OBJECTDIR}/_ext/695214021/ParticleEngineTest.o: ../code/tests/particle\ engine/ParticleEngineTest.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/695214021
	${RM} $@.d
	$(COMPILE.cc) -g -I../code/headers -I../code/headers/utils -I../code/source -I../code/source/utils -I../code/tests -I../code/tests/general\ tests -I../code/tests/isometric\ tilemap -I../code/tests/mandlebrot -I../code/tests/particle\ engine -I../code/tests/polyline -I../code/tests/pong -I${DXSDK_DIR}/ -I/E/PROGRAMASXP/PROGRAMACION/DirectXSDKJunio2010/Include -std=c++11 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/695214021/ParticleEngineTest.o ../code/tests/particle\ engine/ParticleEngineTest.cpp

${OBJECTDIR}/_ext/809142145/PolylineTest.o: ../code/tests/polyline/PolylineTest.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/809142145
	${RM} $@.d
	$(COMPILE.cc) -g -I../code/headers -I../code/headers/utils -I../code/source -I../code/source/utils -I../code/tests -I../code/tests/general\ tests -I../code/tests/isometric\ tilemap -I../code/tests/mandlebrot -I../code/tests/particle\ engine -I../code/tests/polyline -I../code/tests/pong -I${DXSDK_DIR}/ -I/E/PROGRAMASXP/PROGRAMACION/DirectXSDKJunio2010/Include -std=c++11 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/809142145/PolylineTest.o ../code/tests/polyline/PolylineTest.cpp

${OBJECTDIR}/_ext/227780153/Pong.o: ../code/tests/pong/Pong.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/227780153
	${RM} $@.d
	$(COMPILE.cc) -g -I../code/headers -I../code/headers/utils -I../code/source -I../code/source/utils -I../code/tests -I../code/tests/general\ tests -I../code/tests/isometric\ tilemap -I../code/tests/mandlebrot -I../code/tests/particle\ engine -I../code/tests/polyline -I../code/tests/pong -I${DXSDK_DIR}/ -I/E/PROGRAMASXP/PROGRAMACION/DirectXSDKJunio2010/Include -std=c++11 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/227780153/Pong.o ../code/tests/pong/Pong.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libdx_lib32.a

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
