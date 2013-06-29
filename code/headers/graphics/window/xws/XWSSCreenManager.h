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
    Screen* display() const { return _display; }     
};

#endif	/* XWSSCREENMANAGER_H */

