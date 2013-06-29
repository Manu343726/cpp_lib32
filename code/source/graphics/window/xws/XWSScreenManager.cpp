
#include "graphics/window/xws/XWSSCreenManager.h"

XWSScreenManager::XWSScreenManager()
{
   if( !(_display = XOpenDisplay( NULL ) ) )
        throw dl32XWSDisplayConnectionFailed();
   
   _screen_number = (unsigned int) XDefaultScreen( _display );
   _root_window   = XRootWindow( _display , _screen_number );
}