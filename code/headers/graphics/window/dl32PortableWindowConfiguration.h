/* 
 * File:   dl32PortableWindowConfiguration.h
 * Author: manu343726
 *
 * Created on 28 de junio de 2013, 21:00
 */

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

#ifndef DL32PORTABLEWINDOWCONFIGURATION_H
#define	DL32PORTABLEWINDOWCONFIGURATION_H

#include "dl32Config.h"

 #ifdef _WIN32

#include <Windows.h>

using dl32WindowNativeHandle = HWND; 
using dl32NativeEventCode    = UINT;
using dl32NativeEventData    = MSG;

const int WINDOW_DEFAULT_POS_X  = CW_USEDEFAULT;
const int WINDOW_DEFAULT_POS_Y  = CW_USEDEFAULT;
const int WINDOW_DEFAULT_WIDTH  = 800;
const int WINDOW_DEFAULT_HEIGHT = 600;
#else

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <X11/Xatom.h>

using dl32WindowNativeHandle = Window; 
using dl32NativeEventCode    = int;
using dl32NativeEventData    = XEvent;

const int WINDOW_DEFAULT_POS_X  = 0;//Provisional
const int WINDOW_DEFAULT_POS_Y  = 0;//Provisional
const unsigned int WINDOW_DEFAULT_WIDTH  = 800;
const unsigned int WINDOW_DEFAULT_HEIGHT = 600;
const unsigned int WINDOW_BORDER_WIDTH   = 10; //Provisional
#endif   





#endif	/* DL32PORTABLEWINDOWCONFIGURATION_H */

