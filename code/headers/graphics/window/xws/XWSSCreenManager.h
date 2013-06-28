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

class XWSScreenManager : public dl32Singleton<XWSScreenManager>
{
    MAKE_SINGLETON( XWSScreenManager );
    
private:
    Screen* _screen;
    
public:
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief Gets a pointer to the screen used by the windows system.
    ///
    /// @author	Manu343726
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    Screen* screen() const { return _screen; }     
};

#endif	/* XWSSCREENMANAGER_H */

