/*******************************************************************************
* cpp_lib32 project. C++ port of the dx_lib32 project.                         *
*                                                                              *
* Copyright (C) 2012 - 2013, Manuel Sánchez Pérez                              *                     
*                                                                              *
* This file is part of cpp_lib32 project.                                      *
*                                                                              *
* cpp_lib32 project is free software: you can redistribute it and/or modify    *
* it under the terms of the GNU Lesser General Public License as published by  *
* the Free Software Foundation, version 2 of the License.                      *
*                                                                              *
* cpp_lib32 is distributed in the hope that it will be useful,                 *
* but WITHOUT ANY WARRANTY; without even the implied warranty of               * 
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the                *
* GNU Lesser General Public License for more details.                          *
*                                                                              *
 You should have received a copy of the GNU Lesser General Public License      *
 along with cpp_lib32 project. If not, see <http://www.gnu.org/licenses/>.     *
*******************************************************************************/

//CONFIGURACIÓN DE TESTS:
/////////////////////////

#include "dl32Config.h"

#define UNIQUE_TEST __FILE__ ## __LINE__
        
#define DL32TEST_NOTEST -1

#define DL32TEST_GENERAL_STRESS 0
#define DL32TEST_GENERAL_MATRIX 1
#define DL32TEST_GENERAL_ECUATIONSYSTEM 2

#define DL32TEST_MANDELBROTFRACTAL_SIMPLETEST 3

#define DL32TEST_PARTICLEENGINE_SIMPLETEST 4

#define DL32TEST_LAURA_PONG_PSEUDOPLAYABLE 5

#define DL32TEST_POLYLINE_SIMPLETEST 6

#define DL32TEST_ISOMETRICENGINE_ENGINETEST 7

#define DL32TEST_REFACTORING_EVENTSSYSTEM 8
#define DL32TEST_REFACTORING_TYPINGTEST 9
#define DL32TEST_REFACTORING_IS_DESIGN_TEST 10
#define DL32TEST_REFACTORING_CONSOLECOLORSTEST 11



#define DL32TESTS_CURRENTTEST DL32TEST_REFACTORING_CONSOLECOLORSTEST 
        
#define DL32TESTS_ISCURRENTTEST( x ) ((x) == DL32TESTS_CURRENTTEST)


#define DL32DEBUG_SPLINE_PROMPTINTERVALDATA NO
#define DL32DEBUG_GENERAL_PROMPTUNHANDLEDEXCEPTIONS YES