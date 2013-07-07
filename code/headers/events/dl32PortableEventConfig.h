/* 
 * File:   dl32PortableEventConfig.h
 * Author: manu343726
 *
 * Created on 8 de julio de 2013, 0:32
 */

#ifndef DL32PORTABLEEVENTCONFIG_H
#define	DL32PORTABLEEVENTCONFIG_H

#ifdef _WIN32
#include <windows.h>

using dl32NativeEventData = MSG;

#define NATIVE_EVENT_DATA_BYPASS_DECLARATION HWND hwnd , UINT msg , UINT wparam , UINT lparam
#define NATIVE_EVENT_DATA_BYPASS hwnd , msg , wparam , lparam
#define MESSAGE_TO_NATIVE_EVENT_DATA_BYPASS(x) (x).hWnd , (x).msg , (x).wParam , (x).lParam

#else
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <X11/Xatom.h>

using dl32NativeEventData = XEvent;

#define NATIVE_EVENT_DATA_BYPASS_DECLARATION XEvent event_data
#define NATIVE_EVENT_DATA_BYPASS event_data

#endif
#endif	/* DL32PORTABLEEVENTCONFIG_H */

