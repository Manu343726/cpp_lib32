/* 
 * File:   XWSSCreenManager.h
 * Author: manu343726
 *
 * Created on 28 de junio de 2013, 23:55
 */

#ifndef XWSSCREENMANAGER_H
#define	XWSSCREENMANAGER_H

#include "dl32PortableWindowConfiguration.h"
#include "dl32Singleton.h"

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <X11/Xatom.h>

DL32EXCEPTION_SUBCLASS_NODOC( dl32XWSDisplayConnectionFailed )

///////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief This singleton manages the screen instance of the X windows system.
///
/// @author	Manu343726
///////////////////////////////////////////////////////////////////////////////////////////////////////
class XWSScreenManager : public dl32Singleton<XWSScreenManager>
{
    MAKE_SINGLETON( XWSScreenManager );
    
private:
    Display* _display;
    unsigned int _screen_number;
    dl32WindowNativeHandle _root_window;
    
public:
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief Gets the number of the default screen used by the windows system display.
    ///
    /// @author	Manu343726
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    unsigned int screen_number() const { return _screen_number; }   
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief Gets a pointer to the display used by the windows system display.
    ///
    /// @author	Manu343726
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    Display* display() const { return _display; }   
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief Returns a handle to the root window of the screen.
    ///
    /// @author	Manu343726
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    dl32WindowNativeHandle root() const { return _root_window; }
};

#endif	/* XWSSCREENMANAGER_H */

