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

#include "dl32TestConfig.h"

#if DL32TESTS_CURRENTTEST == DL32TEST_REFACTORING_NEWMATH

#include "dl32NewMath.h"

#include <iostream>
using namespace std;

struct A{};

int main()
{
   dl32Point2Df p1(1,1) , p2(2,2) , p3(3,3);
   dl32Vector2Df v1(1,1) , v2(2,2) , v3(3,3);
   dl32Vector2D<bool> vbool1, vbool2, vbool3;
   //dl32Vector2D<A> vcabum1 , vcabum2 , vcabum3;
   
   p3 = p2 + p3;
   v1 = v2 + v3*3;
   
   return 0;
}

#endif /* DL32TESTS_CURRENTTEST */