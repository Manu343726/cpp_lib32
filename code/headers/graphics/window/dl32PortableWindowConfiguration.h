/* 
 * File:   dl32PortableWindowConfiguration.h
 * Author: manu343726
 *
 * Created on 28 de junio de 2013, 21:00
 */

#ifndef DL32PORTABLEWINDOWCONFIGURATION_H
#define	DL32PORTABLEWINDOWCONFIGURATION_H

#include "dl32Config.h"

namespace _dl32
{
 #ifdef _WIN32

#include <Windows.h>

using dl32WindowNativeHandle = HWND; 

const int WINDOW_DEFAULT_POS_X  = CW_USEDEFAULT;
const int WINDOW_DEFAULT_POS_Y  = CW_USEDEFAULT;
const int WINDOW_DEFAULT_WIDTH  = 800;
const int WINDOW_DEFAULT_HEIGHT = 600;
#else

#include <X11/Xlib.h>

using dl32WindowNativeHandle = Window*; 

const int WINDOW_DEFAULT_POS_X  = 0;//Provisional
const int WINDOW_DEFAULT_POS_Y  = 0;//Provisional
const int WINDOW_DEFAULT_WIDTH  = 800;
const int WINDOW_DEFAULT_HEIGHT = 600;
#endif   
}





#endif	/* DL32PORTABLEWINDOWCONFIGURATION_H */

