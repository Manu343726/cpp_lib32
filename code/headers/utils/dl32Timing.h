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

#ifndef DL32TIMING_H
#define DL32TIMING_H

#include "dl32Config.h"

#include <chrono>
#include <string>
#include <vector>

template<typename UNIT>
class dl32TimmingFrame
{
private:
    std::chrono::steady_clock::time_point _begin , _end;
    
    dl32TimmingFrame( const std::chrono::steady_clock::time_point& begin , const std::chrono::steady_clock::time_point& end ) : _begin( begin ) , _end( end ) {}
};

#if DL32DEBUG_TIMING
#define DL32TIMING_BEGIN dl32Timing::push(__FUNCTION__);
#define DL32TIMING_END dl32Timing::pop();
#else
#define DL32TIMING_BEGIN
#define DL32TIMING_END
#endif
#endif /* INCLUDE GUARDS */