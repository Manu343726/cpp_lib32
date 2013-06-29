
#include "dl32Window.h"

#ifdef _WIN32

#else

#include "xws/XWSSCreenManager.h"

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <X11/Xatom.h>

/* Tools */

XWindowAttributes get_window_attributes( dl32WindowNativeHandle window )
{
    XWindowAttributes attributes;
    
    XGetWindowAttributes( XWSScreenManager::instance().display() , window , &attributes );
    
    return attributes;
}

void force_operations()//Puta arquitectura cliente-servidor...
{
    XFlush( XWSScreenManager::instance().display() );
}

/* dl32Window implementation (UNIX/X11 version) */

dl32Window::dl32Window( const std::string& title, int x , int y , unsigned int width , unsigned int height )
{
    auto display = XWSScreenManager::instance().display();
    auto screen  = XWSScreenManager::instance().screen_number();
    
    _window_handle = XCreateSimpleWindow( display , XRootWindow( display , screen ) , x , y , width , height , WINDOW_BORDER_WIDTH , XWhitePixel( display , screen ) , XBlackPixel( display , screen ) );

    this->title( title );
    show();
}

void dl32Window::title(const std::string& title)
{
    XStoreName( XWSScreenManager::instance().display() , _window_handle , title.c_str() ); force_operations();
}

std::string dl32Window::title() const
{
    char* c_style_window_title_string;
    
    XFetchName( XWSScreenManager::instance().display() , _window_handle , &c_style_window_title_string);
    
    if( c_style_window_title_string )
        return std::string( c_style_window_title_string );
    else
        throw dl32WindowTitleFetchingFailed();
}

dl32Point2Di dl32Window::position() const
{
    dl32Point2Di coordinates;
    dl32WindowNativeHandle child_window;
    
    XTranslateCoordinates( XWSScreenManager::instance().display() , 
                           _window_handle , 
                           XWSScreenManager::instance().root() ,
                           0 , 0 ,
                           &coordinates.x ,
                           &coordinates.y ,
                           &child_window
                         );
    
    return coordinates;
}

void dl32Window::position(int x , int y)
{
    XMoveWindow( XWSScreenManager::instance().display() , _window_handle , x , y ); force_operations();
}

void dl32Window::show()
{
    XMapWindow( XWSScreenManager::instance().display() , _window_handle ); force_operations();
}

void dl32Window::hide()
{
    XUnmapWindow( XWSScreenManager::instance().display() , _window_handle ); force_operations();
}

bool dl32Window::visible() const 
{
    return get_window_attributes( _window_handle ).map_state == IsViewable;
}

#endif
