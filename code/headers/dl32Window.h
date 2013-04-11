#ifndef WINDOW_H
#define WINDOW_H

#include <Windows.h>
#include <string>
#include "dl32Events.h"
#include "dl32String.h"
#include "dl32Math.h"
#include "dl32Exceptions.h"

using namespace std;

/// @brief	Defines an alias representing handle of a dl32Window.
typedef HWND dl32WindowHandle;

//Declaración adelantada para el manager.
class dl32Window {};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief	Manages all instances of dl32Windows class and performs windows "machinery".
///
/// @author	Manu 343726
/// @date	09/04/2013
////////////////////////////////////////////////////////////////////////////////////////////////////
class dl32WindowsManager
{
private:
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
	static LRESULT CALLBACK _WindowProcedure(WINDOWS_PROCEDURE_ARGS);
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
};

class dl32WindowsRegistrationFailException : public dl32Exception
{

};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief	cpp_lib32 window class.
/// @details This class is just a Win32 HWND wrapper, thats simplifies windows operations. 
///
/// @author	Manu 343726
/// @date	09/04/2013
////////////////////////////////////////////////////////////////////////////////////////////////////
class dl32Window
{
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
	/// @param	isNewWindow	(Optional) If this dl32Window instance is a wrapper of an existing window,
	/// 					this function not performs the window creation operations.
	////////////////////////////////////////////////////////////////////////////////////////////////////
	void _setup(bool isNewWindow = true);
public:
	static const dl32WindowHandle INVALID_WINDOW_HANDLE;	///< Invalid window handle value.

	static const uint DEFAULT_WIDTH  = 800; ///< dl32Window default width (default value for ctors parameter 'width')
	static const uint DEFAULT_HEIGHT = 600; ///< dl32Window default width (default value for ctors parameter 'height')
	static const uint DEFAULT_LEFT   = CW_USEDEFAULT; ///< dl32Window default left  (default value for ctors parameter 'left')
	static const uint DEFAULT_TOP    = CW_USEDEFAULT; ///< dl32Window default left  (default value for ctors parameter 'top')

	//DEFAULT_LEFT , DEFAULT_TOP ; ver http://msdn.microsoft.com/en-us/library/windows/desktop/ms632679(v=vs.85).aspx (x param e y param)

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief	Creates a new window and registers it in the window manager.
	///
	/// @author	Manu 343726
	/// @date	09/04/2013
	///
	/// @param	title 	Title for the new window.
	/// @param	width 	(Optional) Window width. default value is dl32Window::DEFAULT_WIDTH.
	/// @param	height	(Optional) Window height. default value is dl32Window::DEFAULT_HEIGHT.
	////////////////////////////////////////////////////////////////////////////////////////////////////
	dl32Window(const string& title , uint width = DEFAULT_WIDTH , uint height = DEFAULT_HEIGHT , uint left = DEFAULT_LEFT , uint top = DEFAULT_TOP);

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief	Creates a wrapper for an existing window (@see dl32Window details).
	///
	/// @author	Manu 343726
	/// @date	09/04/2013
	///
	/// @param	handle	Handle of the existing window.
	////////////////////////////////////////////////////////////////////////////////////////////////////
	dl32Window(dl32WindowHandle handle) : _handle( handle ) {}

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
#endif