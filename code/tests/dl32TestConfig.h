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

#define UNIQUE_TEST_COUNTER_BYPASS(x) x
#define UNIQUE_TEST_NUMBER UNIQUE_TEST_COUNTER_BYPASS( __COUNTER__ )
        
#define DL32TEST_NOTEST -1

#define DL32TEST_GENERAL_STRESS         0
#define DL32TEST_GENERAL_MATRIX         1
#define DL32TEST_GENERAL_ECUATIONSYSTEM 2

#define DL32TEST_SPECIFIC_MANDELBROTFRACTAL_SIMPLETEST 10
#define DL32TEST_SPECIFIC_PARTICLEENGINE_SIMPLETEST    11
#define DL32TEST_SPECIFIC_LAURA_PONG_PSEUDOPLAYABLE    12
#define DL32TEST_SPECIFIC_POLYLINE_SIMPLETEST          13
#define DL32TEST_SPECIFIC_ISOMETRICENGINE_ENGINETEST   14

#define DL32TEST_REFACTORING_EVENTSSYSTEM      20
#define DL32TEST_REFACTORING_TYPINGTEST        21
#define DL32TEST_REFACTORING_IS_DESIGN_TEST    22
#define DL32TEST_REFACTORING_CONSOLECOLORSTEST 23
#define DL32TEST_REFACTORING_NEWMATH           24
#define DL32TEST_REFACTORING_TMP_QUICKSORT     25
#define DL32TEST_REFACTORING_TMP_MINIMAX       26
#define DL32TEST_REFACTORING_WINDOWTEST        27
#define DL32TEST_REFACTORING_EVENTTEST         28

#define DL32TEST_ASCII_RASTER_TEST 30



#define DL32TESTS_CURRENTTEST DL32TEST_REFACTORING_EVENTTEST 
        
#define DL32TESTS_ISCURRENTTEST( x ) ((x) == DL32TESTS_CURRENTTEST)


#define DL32DEBUG_SPLINE_PROMPTINTERVALDATA NO
#define DL32DEBUG_GENERAL_PROMPTUNHANDLEDEXCEPTIONS YES