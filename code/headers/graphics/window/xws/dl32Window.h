/* 
 * File:   dl32Window_xws.h
 * Author: manu343726
 *
 * Created on 27 de junio de 2013, 16:25
 */

#ifndef DL32WINDOW_XWS_H
#define	DL32WINDOW_XWS_H

#include "dl32Config.h"
#include "dl32Singleton.h"
#include "dl32Exceptions.h"

#include "dl32PortableWindowConfiguration.h"

class dl32Window
{
private:
    _dl32::dl32WindowNativeHandle _window_handle;
    
public:
    static const unsigned int DEFAULT_WIDTH  = _dl32::WINDOW_DEFAULT_WIDTH; ///< Gets the window default width.
    static const unsigned int DEFAULT_HEIGHT = _dl32::WINDOW_DEFAULT_HEIGHT; ///< Gets the window default height.
    static const int DEFAULT_POS_X  = _dl32::WINDOW_DEFAULT_POS_X; ///< Gets the window default x coorinate.
    static const int DEFAULT_POS_Y  = _dl32::WINDOW_DEFAULT_POS_Y; ///< Gets the window default y coordinate.
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief Creates a window with the specified title and area (Position and size).
    ///
    /// @param title A str::string containing the window title.
    /// @param x Initial x-coorinate of the window. Its default value is DEFAULT_POS_X.
    /// @param y Initial x-coorinate of the window. Its default value is DEFAULT_POS_Y.
    /// @param width Initial width of the window. Its default value is DEFAULT_WIDTH.
    /// @param height Initial height of the window. Its default value is DEFAULT_HEIGHT.
    ///
    /// @author	Manu343726
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    dl32Window(const std::string& title , int x = DEFAULT_POS_X , int y = DEFAULT_POS_Y , unsigned int width = DEFAULT_WIDTH, unsigned int height = DEFAULT_HEIGHT);
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief Creates a window with the specified title and area (Position and size).
    ///
    /// @param title A str::string containing the window title.
    /// @param position A integer 2D point with containing the initial position of the window.
    /// @param width Initial width of the window. Its default value is DEFAULT_WIDTH.
    /// @param height Initial height of the window. Its default value is DEFAULT_HEIGHT.
    ///
    /// @author	Manu343726
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    dl32Window(const std::string& title , const dl32Point2Di& position , unsigned int width = DEFAULT_WIDTH , unsigned int height = DEFAULT_HEIGHT) : XWindow( title , position.x , position.y , width , height) {}
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief Gets the title of the window.
    ///
    /// @return A std::string with the window title.
    ///
    /// @author	Manu343726
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    std::string title() const;
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief Sets the title of the window.
    ///
    /// @param title A std::string containing the new title of the window.
    ///
    /// @author	Manu343726
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    void title(const std::string& title);
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief Gets the position of the window.
    ///
    /// @return An integer 2d point with the window coordinates.
    ///
    /// @author	Manu343726
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    dl32Point2Di position() const;
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief Sets the position of the window.
    ///
    /// @param x x-coordinate of the new window position.
    /// @param y y-coordinate of the new window position.
    ///
    /// @author	Manu343726
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    void position(int x , int y);
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief Sets the position of the window.
    ///
    /// @param _position An integer 2D point with the new window position.
    ///
    /// @author	Manu343726
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    void position(const dl32Point2Di& _position) { position( _position.x , _position.y ); }
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief Set the window as visible.
    ///
    /// @author	Manu343726
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    void show();
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief Set the window as invisible.
    ///
    /// @author	Manu343726
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    void hide();
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief Checks if the window is visible.
    ///
    /// @return Returns true if the window is visible. Returns false if is invisible.
    ///
    /// @author	Manu343726
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    bool visible() const;
};

#endif	/* DL32WINDOW_XWS_H */

