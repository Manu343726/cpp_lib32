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

#ifndef WINDOW_H
#define WINDOW_H

#include "dl32Config.h"
#include "dl32Events.h"
#include "dl32Math.h"
#include "dl32Exceptions.h"
#include "dl32Singleton.h"

#include <Windows.h>

#include <string>
#include <memory>

using namespace std;

/// @brief	Defines an alias representing handle of a dl32Window.
typedef HWND dl32WindowHandle;


class dl32WindowsManager;;

DL32EXCEPTION_SUBCLASS_NODOC(dl32WindowClassRegistrationFailedException);

DL32EXCEPTION_SUBCLASS_NODOC(dl32WindowCreationFailedException);

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief	cpp_lib32 window class.
/// @details This class is just a Win32 HWND wrapper, thats simplifies windows operations. 
///
/// @author	Manu 343726
/// @date	09/04/2013
////////////////////////////////////////////////////////////////////////////////////////////////////
class dl32Window
{
    friend class dl32WindowsManager;
private:
	static const WNDCLASS WINDOWCLASS; ///< cpp_lib32 window class (See http://msdn.microsoft.com/es-es/library/windows/desktop/ms633576(v=vs.85).aspx) 
	static bool _windowClassRegistered; ///< Window class registration flag.

	dl32WindowHandle _handle; ///< Window handle.

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief	Setups the window.
	///
	/// @author	Manu 343726
	/// @date	09/04/2013
	/// 		
	/// @param	title 	Title for the new window.
	/// @param	width 	Window width. default value is dl32Window::DEFAULT_WIDTH.
	/// @param	height	Window height. default value is dl32Window::DEFAULT_HEIGHT.
	/// @param	left  	Window left. default value is dl32Window::DEFAULT_LEFT.
	/// @param	top   	Window top. default value is dl32Window::DEFAULT_TOP.
	/// @param	isNewWindow	(Optional) If this dl32Window instance is a wrapper of an existing window,
	/// 					this function not performs the window creation operations.
	////////////////////////////////////////////////////////////////////////////////////////////////////
	void _setup(const string& title , uint width , uint height , uint left , uint top , bool isNewWindow = true);
        
        ////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief	Creates a new window and registers it in the window manager.
	///
	/// @author	Manu 343726
	/// @date	09/04/2013
	///
	/// @param	title 	Title for the new window.
	/// @param	width 	(Optional) Window width. default value is dl32Window::DEFAULT_WIDTH.
	/// @param	height	(Optional) Window height. default value is dl32Window::DEFAULT_HEIGHT.
	/// @param	left  	(Optional) Window left. default value is dl32Window::DEFAULT_LEFT.
	/// @param	top   	(Optional) Window top. default value is dl32Window::DEFAULT_TOP.
	////////////////////////////////////////////////////////////////////////////////////////////////////
	dl32Window(const string& title , uint width = DEFAULT_WIDTH , uint height = DEFAULT_HEIGHT , uint left = DEFAULT_LEFT , uint top = DEFAULT_TOP) {_setup( title , width , height , left , top); }

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief	Creates a wrapper for an existing window (@see dl32Window details).
	///
	/// @author	Manu 343726
	/// @date	09/04/2013
	///
	/// @param	handle	Handle of the existing window.
	////////////////////////////////////////////////////////////////////////////////////////////////////
	dl32Window(dl32WindowHandle handle) : _handle( handle ) {}
public:
	static const dl32WindowHandle INVALID_WINDOW_HANDLE;	///< Invalid window handle value.

	static const uint DEFAULT_WIDTH  = 800; ///< dl32Window default width (default value for ctors parameter 'width')
	static const uint DEFAULT_HEIGHT = 600; ///< dl32Window default width (default value for ctors parameter 'height')
	static const uint DEFAULT_LEFT   = CW_USEDEFAULT; ///< dl32Window default left  (default value for ctors parameter 'left')
	static const uint DEFAULT_TOP    = CW_USEDEFAULT; ///< dl32Window default left  (default value for ctors parameter 'top')

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief	Checks equality of two dl32Window instances.
	///
	/// @author	Manu 343726
	/// @date	09/04/2013
	///
	/// @param	other	dl32Window instance to be compared with.
	///
	/// @return	true if both instances wrap the same window. False in another case.
	////////////////////////////////////////////////////////////////////////////////////////////////////
	bool operator ==(const dl32Window& other) const {return _handle == other._handle;}

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief	Checks non-equality of two dl32Window instances.
	///
	/// @author	Manu 343726
	/// @date	09/04/2013
	///
	/// @param	other	dl32Window instance to be compared with.
	///
	/// @return	true if both instances not wrap the same window. False in another case.
	////////////////////////////////////////////////////////////////////////////////////////////////////
	bool operator !=(const dl32Window& other) const {return !(*this == other);}
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief	Manages all instances of dl32Windows class and performs windows "machinery".
///
/// @author	Manu 343726
/// @date	09/04/2013
////////////////////////////////////////////////////////////////////////////////////////////////////
class dl32WindowsManager : public dl32Singleton<dl32WindowsManager,true>
{
    MAKE_SINGLETON(dl32WindowsManager,true)
private:
    virtual void _setup_singleton_instance() { _processingMessages = false; }

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief	"Window-specific" (See remarks) message processing callback.
	///
	/// @author	Manu 343726
	/// @date	09/04/2013
	///
	/// @param	window 	Handle of the window.
	/// @param	message	System message code.
	/// @param	wParam 	The wParam field of the message.
	/// @param	lParam 	The lParam field of the message.
	/// 				
	/// @remarks Every window has its own window procedure (In theory). In practice, we can't define a 
	/// 		 callback as a member function, so we use a static version (Or a global version instead).
	/// 		 Although Windows (The operative system) calls the specific windows procedure to process
	/// 		 a message (Message sent to that window), we have no manner to directly identify the 
	/// 		 window, because we use one window procedure for all dl32Window instances.
	/// 		 So we have to do that "identification process" at the window procedure (See the 
	/// 		 dl32WindowsManager::WindowProcedure() implementation).
	////////////////////////////////////////////////////////////////////////////////////////////////////
	static LRESULT CALLBACK _WindowProcedure( WINDOWS_PROCEDURE_ARGS );

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief	System messages loop.
	/// @detailts Processes system messages from the thread message queue.
	///
	/// @author	Manu
	/// @date	16/04/2013
	////////////////////////////////////////////////////////////////////////////////////////////////////
	void _messageLoop();

	bool _processingMessages; 
public:
	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief	Gets the window procedure.
	///
	/// @author	Manu 343726
	/// @date	09/04/2013
	///
	/// @return	The window procedure.
	////////////////////////////////////////////////////////////////////////////////////////////////////
	static int getWindowProcedure() {return (LONG_PTR)_WindowProcedure;}

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief	Gets a window through its handle.
	///
	/// @author	Manu 343726
	/// @date	10/04/2013
	///
	/// @param	handle	Window handle.
	///
	/// @return	The window.
	////////////////////////////////////////////////////////////////////////////////////////////////////
	static dl32Window& getWindow(dl32WindowHandle handle) { return *( (dl32Window*)GetWindowLongPtr( handle , GWLP_USERDATA ) ); }

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief	Starts the system messages processing loop.
	///
	/// @author	Manu
	/// @date	16/04/2013
	////////////////////////////////////////////////////////////////////////////////////////////////////
	void start() { if(!_processingMessages) { _processingMessages = true; _messageLoop(); }; }

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief	Stops the system messages processing loop.
	///
	/// @author	Manu
	/// @date	16/04/2013S
	////////////////////////////////////////////////////////////////////////////////////////////////////
	void stop() { _processingMessages = false; }
        
                ////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief	Creates a new window and registers it in the window manager.
	///
	/// @author	Manu 343726
	/// @date	09/04/2013
	///
	/// @param	title 	Title for the new window.
	/// @param	width 	(Optional) Window width. default value is dl32Window::DEFAULT_WIDTH.
	/// @param	height	(Optional) Window height. default value is dl32Window::DEFAULT_HEIGHT.
	/// @param	left  	(Optional) Window left. default value is dl32Window::DEFAULT_LEFT.
	/// @param	top   	(Optional) Window top. default value is dl32Window::DEFAULT_TOP.
	////////////////////////////////////////////////////////////////////////////////////////////////////
	static shared_ptr<dl32Window> createWindow(const string& title , uint width = dl32Window::DEFAULT_WIDTH , uint height = dl32Window::DEFAULT_HEIGHT , uint left = dl32Window::DEFAULT_LEFT , uint top = dl32Window::DEFAULT_TOP);

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief	Creates a wrapper for an existing window (@see dl32Window details).
	///
	/// @author	Manu 343726
	/// @date	09/04/2013
	///
	/// @param	handle	Handle of the existing window.
	////////////////////////////////////////////////////////////////////////////////////////////////////
	static shared_ptr<dl32Window> createWindow(dl32WindowHandle handle);
};
#endif