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
/// @brief	Manages al instances of dl32Windows class and performs its "machinery".
///
/// @author	Manu 343726
/// @date	09/04/2013
////////////////////////////////////////////////////////////////////////////////////////////////////
class dl32WindowsManager
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
	dl32WindowHandle _handle;
public:
	static const uint DEFAULT_WIDTH  = 800; ///< dl32Window default width (default value for ctors parameter 'width')
	static const uint DEFAULT_HEIGHT = 600; ///< dl32Window default width (default value for ctors parameter 'height')
	static const uint DEFAULT_LEFT   = CW_USEDEFAULT; ///< dl32Window default left  (default value for ctors parameter 'left')
	static const uint DEFAULT_TOP    = CW_USEDEFAULT; ///< dl32Window default left  (default value for ctors parameter 'top')

	//DEFAUKT_LEFT , DEFAULT_TOP ; ver http://msdn.microsoft.com/en-us/library/windows/desktop/ms632679(v=vs.85).aspx

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
	/// @brief	Checks equality for two dl32Window instances.
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
	/// @brief	Checks non-equality for two dl32Window instances.
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