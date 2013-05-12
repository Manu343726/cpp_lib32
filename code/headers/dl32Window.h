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

/**********************************************************************************
 * cpp_lib32 basic user input events declarations (As alias of dl32Event template) *
 **********************************************************************************/

template<typename ARGSTYPE>
using dl32WindowEvent = dl32Event<dl32Window, ARGSTYPE>;

/* Mouse events: */

/// @brief	Defines an alias representing the cpp_lib32 mouse event type. This event provides a dl32MouseData args with mouse input data.
typedef dl32WindowEvent<dl32MouseData> dl32MouseEvent;

typedef dl32MouseEvent dl32MouseClickEvent; ///< Defines the MouseClickEvent as an alias of the MouseEvent type.	
typedef dl32MouseEvent dl32MouseDoubleClickEvent; ///< Defines the MouseDoubleClickEvent as an alias of the MouseEvent type.
typedef dl32MouseEvent dl32MouseMoveEvent; ///< Defines the MouseMoveEvent as an alias of the MouseEvent type.
typedef dl32MouseEvent dl32MouseUpEvent; ///< Defines the MouseUpEvent as an alias of the MouseEvent type.
typedef dl32MouseEvent dl32MouseDownEvent; ///< Defines the MouseDownEvent as an alias of the MouseEvent type.
typedef dl32MouseEvent dl32MouseWheelEvent; ///< Defines the MouseWheelEvent as an alias of the MouseEvent type.
typedef dl32MouseEvent dl32MouseEnterEvent; ///< Defines the MouseEnterEvent as an alias of the MouseEvent type.
typedef dl32MouseEvent dl32MouseLeaveEvent; ///< Defines the MouseLeaveEvent as an alias of the MouseEvent type.


/* Keyboard events: */

/// @brief	Defines an alias representing the cpp_lib32 keyboard event type. This event provides a dl32KeyStrokeData args with keyboard input data.
typedef dl32WindowEvent<dl32KeyStrokeData> dl32KeyboardEvent;
typedef dl32KeyboardEvent dl32KeyPressEvent; ///< Defines the KeyPressed event as an alias of the KeyboardEvent type.
typedef dl32KeyboardEvent dl32KeyDownEvent; ///< Defines the KeyDown event as an alias of the KeyboardEvent type.
typedef dl32KeyboardEvent dl32KeyUpEvent; ///< Defines the KeyUp event as an alias of the KeyboardEvent type.


/* Window area events: */

/// @brief	 Defines an alias for a window area event. The 2D AABB provided by event args is the window area.
/// @details "Area" refers to the screen zone ocuppied by the window, not only window size. Therefore, window movemments are included in this category.
typedef dl32WindowEvent<dl32AABB2D> dl32AreaChangedEvent;
typedef dl32AreaChangedEvent dl32MoveEvent; ///< Defines the window movemment event as an alias of the AreaChanged event (@see dl32AreaChangedEvent details).
typedef dl32AreaChangedEvent dl32ResizeEvent; ///< Defines the window resize event as an alias of the AreaChanged event.
typedef dl32AreaChangedEvent dl32ResizeBeginEvent; ///< Defines the window resize begining event as an alias of the AreaChanged event.
typedef dl32AreaChangedEvent dl32ResizeEndEvent; ///< Defines the window resize ending event as an alias of the AreaChanged event.


/* Miscelaneous: */

typedef dl32WindowEvent<void> dl32PaintEvent; ///< Defines the paint event as a non-args event. 
typedef dl32WindowEvent<bool> dl32WindowCloseEvent; ///< Window close event. Booleam argumment allows the user to cancel the window close process (Set to true to cancel). 

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief	 Translates system messages to cpp_lib32 events.
/// @details     This interface provides a method for create user-defined events. User can use this 
/// 		 interface to translate a specific system message to a cpp_lib32 event.
///              Note that a system-message event allways has a dl32Window as sender.
///
/// @author	Manu343726
/// @date	07/04/2013
////////////////////////////////////////////////////////////////////////////////////////////////////
class dl32EventDispatcher {
public:
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief	Destructor.
    ///
    /// @author	Manu343726
    /// @date	07/04/2013
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    virtual ~dl32EventDispatcher() {
    };

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief	Gets a windows message data and raises the high-level event.
    /// @details This function performs any data-translation operations and raises the high-level event 
    /// 		 with the translated data as event args.
    /// 		
    /// @param	window 	Handle of the window.
    /// @param	message	System message code.
    /// @param	wParam 	The wParam field of the message.
    /// @param	lParam 	The lParam field of the message.
    /// 				
    /// @author	Manu343726
    /// @date	07/04/2013
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    virtual void dispatch(WINDOWS_PROCEDURE_ARGS) = 0;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief	Defines a generic system event dispatcher.
///
/// @author	Manu 343726
/// @date	13/04/2013
///
/// @tparam	EVENTARGSTYPE Args type of the event to be dispatched.
////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename EVENTARGSTYPE>
class dl32GenericEventDispatcher : public dl32EventDispatcher {
    friend class dl32SystemEventsManager;
private:
    using event_type = dl32Event<dl32Window, EVENTARGSTYPE>;
    event_type _event;
    typename event_type::DispatcherType _dispatchFunction;
public:

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief	Constructor.
    ///
    /// @author	Manu 343726
    /// @date	14/04/2013
    ///
    /// @param	dispatchFunction	The dispatch function.
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    dl32GenericEventDispatcher(typename event_type::DispatcherType dispatchFunction) : _dispatchFunction(dispatchFunction) {
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief	Destructor.
    ///
    /// @author	Manu 343726
    /// @date	14/04/2013
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    ~dl32GenericEventDispatcher() {
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief	Gets a windows message data and raises the high-level event.
    /// @details This function performs any data-translation operations and raises the high-level event 
    /// 		 with the translated data as event args.
    /// 		
    /// @param	window 	Handle of the window.
    /// @param	message	System message code.
    /// @param	wParam 	The wParam field of the message.
    /// @param	lParam 	The lParam field of the message.
    /// 				
    /// @author	Manu343726
    /// @date	07/04/2013
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    void dispatch(WINDOWS_PROCEDURE_ARGS) {
        _event.RaiseEvent(dl32Window::getWindow( window ) , _dispatchFunction(WINDOWS_PROCEDURE_BYPASS));
    }
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief	This class provides a set of functions that translate common system messages data to 
/// 		high-level events args.
///
/// @author	Manu343726
/// @date	07/04/2013
////////////////////////////////////////////////////////////////////////////////////////////////////
class dl32SystemMessagesDataTranslator {
public:
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief	Gets mouse data from a mouse event system message. 
    ///
    /// @author	Manu343726
    /// @date	07/04/2013
    ///
    /// @param	window 	Handle of the window.
    /// @param	message	System message code.
    /// @param	wParam 	The wParam field of the message.
    /// @param	lParam 	The lParam field of the message.
    ///
    /// @return	The mouse data.
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    static dl32MouseData getMouseData(WINDOWS_PROCEDURE_ARGS);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief	Gets keystroke data from a keystroke event system message. 
    ///
    /// @author	Manu343726
    /// @date	07/04/2013
    ///
    /// @param	window 	Handle of the window.
    /// @param	message	System message code.
    /// @param	wParam 	The wParam field of the message.
    /// @param	lParam 	The lParam field of the message.
    ///
    /// @return	The mouse data.
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    static dl32KeyStrokeData getKeyStrokeData(WINDOWS_PROCEDURE_ARGS);
};

DL32EXCEPTION_SUBCLASS_NODOC(dl32NoSuchEventException);

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief	 cpp_lib32 high-level events manager.
/// @details     Any cpp_lib32 event that acts as a wrapper of a system message (Or messages), are managed
/// 		 by this class.
///
/// @author	Manu343726
/// @date	07/04/2013
////////////////////////////////////////////////////////////////////////////////////////////////////
class dl32SystemEventsManager {
private:
    unordered_map<UINT, dl32EventDispatcher*> _dispatchers; ///< Hashmap of event dispatchers. System messages are used as keys.

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief	Sets up the dispatchers for the cpp_lib32 window class default events.
    /// @details cpp_lib32 provides a set of common events defined by default. 
    ///
    /// @author	Manu343726
    /// @date	07/04/2013
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    void setUpDefaultEvents();

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief	Copy constructor. Private, dl32SystemEventsManager implements singleton design pattern.
    ///
    /// @author	Manu
    /// @date	15/04/2013
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    dl32SystemEventsManager(const dl32SystemEventsManager&) {
    };

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief	Default constructor. Private, dl32SystemEventsManager implements singleton design pattern.
    ///
    /// @author	Manu343726
    /// @date	07/04/2013
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    dl32SystemEventsManager() {
    }

    static dl32SystemEventsManager* _instance;

    static void destroyInstance() {
        delete _instance;
        _instance = nullptr;
    }
public:
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief	Destructor.
    ///
    /// @author	Manu343726
    /// @date	07/04/2013
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    ~dl32SystemEventsManager();

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief	Gets the singleton instance of dl32SystemEventsManager class
    ///
    /// @author	Manu343726
    /// @date	07/04/2013
    ///
    /// @return     Reference to the class instance.
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    static dl32SystemEventsManager& instance() {
        if (_instance == nullptr) {
            _instance = new dl32SystemEventsManager();

            atexit(destroyInstance);
        }

        return *_instance;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief	A shortcut to the dl32SystemEventsManager instance
    ///
    /// @author	Manu343726
    /// @date	07/04/2013
    ////////////////////////////////////////////////////////////////////////////////////////////////////
#define EventsManager dl32SystemEventsManager::instance()

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief	Sets up a new high-level event through its dispatcher.
    /// @details User can define new high-level events that "wrap" specific system messages. Also, 
    /// 		 cpp_lib32 provides a set of common system events preconfigured in the events manager.
    /// 		 (@see setUpDefaultEvents()). 
    /// @details Note that a configured event cannot be removed from the events manager. Every configured
    /// 		 event lives until the end of the application.
    ///
    /// @author	Manu343726
    /// @date	07/04/2013
    ///
    /// @param [in,out]	dispatcher	Pointer to the event dispatcher.
    /// @param	systemMessages	  	List of system messages codes that raises the new event.
    ///
    /// @return	true if any system message provided is in use. False in other case.
    /// 
    /// @remarks A dl32Event can wrap more than one system message, that is the reason that the parameter
    ///          "systemMessages" is a std::vector<UINT>, not only one UINT.
    /// @remarks For example: System has three different messages (WM_LBUTTONDOWN , WM_RBUTTONDOWN , 
    ///          WM_MBUTTONDOWN) for mouse down events, one per button. User can define a general 
    ///          MouseDown event that catches this three messages.
    /// 	
    /// @return	true if all of the system messages provided are not in use. False in other case.	 
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    bool setUpEvent(dl32EventDispatcher* dispatcher, const vector<UINT>& systemMessages);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief	Sets up a new high-level event through its dispatcher.
    /// @details User can define new high-level events that "wrap" specific system messages. Also, 
    /// 		 cpp_lib32 provides a set of common system events preconfigured in the events manager.
    /// 		 (@see setUpDefaultEvents()). 
    /// @details Note that a configured event cannot be removed from the events manager. Every configured
    /// 		 event lives until the end of the application.
    ///
    /// @author	Manu343726
    /// @date	07/04/2013
    ///
    /// @param [in,out]	dispatcher	Pointer to the event dispatcher.
    /// @param	systemMessages	  	System message code that raises the new event.
    ///
    /// @return	true if system message provided is not in use. False in other case.
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    bool setUpEvent(dl32EventDispatcher* dispatcher, UINT systemMessage);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief	Sets up a new high-level event through its dispatcher.
    /// @details This overloaded version of setUptEvent() sets up a new event dispatcher by a dispatch
    /// 		 function provided by the user.
    ///
    /// @author	Manu 343726
    /// @date	14/04/2013
    ///
    /// @tparam	typename EVENTARGSTYPE	Type of the event argumments.
    /// @param	dispatchFunction	The dispatch function.
    /// @param	systemMessages  	List of system messages codes that raises the new event.
    ///
    /// @return	true if all of the system messages provided are not in use. False in other case.
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    template<typename EVENTTYPE>
    bool setUpEvent(typename EVENTTYPE::DispatcherType dispatchFunction, const vector<UINT>& systemMessages) {
        return setUpEvent(new dl32GenericEventDispatcher<typename EVENTTYPE::ArgummentsType_NoRef> (dispatchFunction), systemMessages);
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief	Sets up a new high-level event through its dispatcher.
    /// @details This overloaded version of setUptEvent() sets up a new event dispatcher by a dispatch
    /// 		 function provided by the user.
    ///
    /// @author	Manu 343726
    /// @date	14/04/2013
    ///
    /// @tparam	typename EVENTARGSTYPE	Type of the event.
    /// @param	dispatchFunction	The dispatch function.
    /// @param	systemMessages  	System message code that raises the new event.
    ///
    /// @return	true if system message provided is not in use. False in other case.
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    template<typename EVENTTYPE>
    bool setUpEvent(typename EVENTTYPE::DispatcherType dispatchFunction, UINT systemMessage) {
        return setUpEvent(new dl32GenericEventDispatcher<typename EVENTTYPE::ArgummentsType_NoRef > (dispatchFunction), systemMessage);
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief	Gets a reference to the event that wraps a specific system message.
    ///
    /// @author	Manu
    /// @date	15/04/2013
    ///
    /// @exception	dl32NoSuchEventException	Thrown when there is not any event that wraps the provided
    /// 										system message at the manager.
    ///
    /// @tparam	EVENTTYPE	Type of the event.
    /// @param	systemMessage	System message code.
    ///
    /// @return	A reference to the event.
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    template<class EVENTTYPE>
    EVENTTYPE& getEvent(UINT systemMessage) throw ( dl32NoSuchEventException) {
        auto it = _dispatchers.find(systemMessage);

        if (it != std::end(_dispatchers))
            return ( reinterpret_cast<dl32GenericEventDispatcher<typename EVENTTYPE::ArgummentsType_NoRef>*> (it->second))->_event;
        else
            throw dl32NoSuchEventException();
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief	Gets a windows message data and raises the specific high-level event.
    /// 		
    /// @param	window 	Handle of the window.
    /// @param	message	System message code.
    /// @param	wParam 	The wParam field of the message.
    /// @param	lParam 	The lParam field of the message.
    /// 				
    /// @author	Manu343726
    /// @date	07/04/2013
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    void dispatch(WINDOWS_PROCEDURE_ARGS) 
    {
        auto it = _dispatchers.find(message);

        if (it != std::end(_dispatchers))
            it->second->dispatch(window, message, wParam, lParam);
    }
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
	static dl32Window& getWindow(dl32WindowHandle handle) { return dl32Window::getWindow( handle ); }

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