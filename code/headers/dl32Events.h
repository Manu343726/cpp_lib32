#ifndef DL32EVENTS_H
#define DL32EVENTS_H

#include "dl32Config.h"
#include "dl32Math.h"

#include <Windows.h>

#include <unordered_map>
#include <vector>

using namespace std;


/// @brief	Mouse buttons enumeration
enum class dl32MouseButton
{
	NONE,   ///< No button
	RIGHT,  ///< Right button
	CENTER, ///< Center button (Mouse wheel button, normally)
	LEFT    ///< Left button
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief	Basic mouse data.
///
/// @author	Manu343726
/// @date	07/04/2013
////////////////////////////////////////////////////////////////////////////////////////////////////
struct dl32MouseData
{
	dl32Point2D Location;   ///< Mouse-pointer in local coordinates. 
	dl32MouseButton Button; ///< Pressed mouse button (@see dl32MouseButton).
	int Delta;              ///< Mouse wheel delta.
};

/// @brief	cpp_lib32 keyboard codes
typedef char dl32KeyCode;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief	Basic keystroke data.
///
/// @author	Manu343726
/// @date	07/04/2013
////////////////////////////////////////////////////////////////////////////////////////////////////
struct dl32KeyStrokeData
{
	bool PreviousPressed;  ///< The previous key state. true if its down before, false if its up.
	dl32KeyCode Key;       ///< Key code.
	int RepeatCount;       ///< If the previous key state is "pressed", number of times the keystroke is autorepeated as a result of the user holding down the key. Cero in other case.
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief	 cpp_lib32 event type
/// @details Events are represented as a list of handlers, where handlers are pointers to functions 
/// 		 that handle the event. This functions always be a void function with cero or one
/// 		 parameter (The argumments of the event) passed by reference, in other words, functions 
/// 		 with prototype "void EventHandler(ARGSTYPE &args)".
/// 		 Template parameter "ARGSTYPE" is the type of these event args. For non-args event, use
/// 		 void.
/// 		 
/// 		 The type of the function-pointers that can be handlers are defined in the HandlerType
/// 		 typedef.
/// @tparam	ARGTYPE	Event argumments type. For non-args events, use void (See void template 
/// 				specialitation of this class).
////////////////////////////////////////////////////////////////////////////////////////////////////
template <typename ARGSTYPE>
class dl32Event
{
public:
	/// @brief	Defines an alias representing type of event argumments.
	typedef ARGSTYPE& ArgummentsType;

	/// @brief	Defines an alias representing function-pointer type of a event handler.
	typedef void (*HandlerType)(ArgummentsType);
private:
	vector<HandlerType> _handlers;
public:


	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief	Raises the event.
	/// @param [in,out]	args Argumments of the event.
	////////////////////////////////////////////////////////////////////////////////////////////////////
	void RaiseEvent(ArgummentsType args)
	{
		for(auto it = _handlers.begin() ; it != _handlers.end() ; ++it)
			(*it)(args);
	}


	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief	Adds a function as a handler for this event.
	/// @param	handler	Pointer to the function that will be a handler.
	////////////////////////////////////////////////////////////////////////////////////////////////////
	void AddHandler(HandlerType handler) {_handlers.push_back(handler);}


	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief	Removes the handler described by handler.
	/// @param	handler	The handler.
	////////////////////////////////////////////////////////////////////////////////////////////////////
	void RemoveHandler(HandlerType handler) {_handlers.erase(handler);}
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief	cpp_lib32 event type (Template specialitation for non-args events)
/// @details Specialitation of dl32Event for non-args events. Non-args events handlers are functions
/// 		 with protoptype "void EventHandler()". 
/// 		 
/// 		 
/// 		 The type of the function-pointers that can be handlers are defined in the HandlerType
/// 		 typedef.
////////////////////////////////////////////////////////////////////////////////////////////////////
template<>
class dl32Event<void>
{
public:
	/// @brief	Defines an alias representing type of event argumments.
	typedef void ArgummentsType;

	/// @brief	Defines an alias representing function-pointer type of a event handler.
	typedef void (*HandlerType)();
private:
	vector<HandlerType> _handlers;
public:

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief	Raises the non-args event.
	////////////////////////////////////////////////////////////////////////////////////////////////////
	void RaiseEvent()
	{
		for(auto it = _handlers.begin() ; it != _handlers.end() ; ++it)
			(*it)();
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief	Adds a function as a handler for this event.
	/// @param	handler	Pointer to the function that will be a handler.
	////////////////////////////////////////////////////////////////////////////////////////////////////
	void AddHandler(HandlerType handler) {_handlers.push_back(handler);}

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief	The function passed stops being a handler of the event.
	/// @param	handler	Function-pointer to the handler.
	////////////////////////////////////////////////////////////////////////////////////////////////////
	void RemoveHandler(HandlerType handler) {_handlers.erase(handler);}
};



/// @brief	Alias for a non-args event
typedef dl32Event<void> dl32NonArgsEvent;

/**********************************************************************************
* cpp_lib32 basic user input events declarations (As alias of dl32Event template) *
**********************************************************************************/

/* Mouse events: */

/// @brief	Defines an alias representing the cpp_lib32 mouse event type. This event provides a dl32MouseData args with mouse input data.
typedef dl32Event<dl32MouseData> dl32MouseEvent;

typedef dl32MouseEvent dl32MouseClickEvent;       ///< Defines the MouseClickEvent as an alias of the MouseEvent type.	
typedef dl32MouseEvent dl32MouseDoubleClickEvent; ///< Defines the MouseDoubleClickEvent as an alias of the MouseEvent type.
typedef dl32MouseEvent dl32MouseMoveEvent;        ///< Defines the MouseMoveEvent as an alias of the MouseEvent type.
typedef dl32MouseEvent dl32MouseUpEvent;          ///< Defines the MouseUpEvent as an alias of the MouseEvent type.
typedef dl32MouseEvent dl32MouseDownEvent;        ///< Defines the MouseDownEvent as an alias of the MouseEvent type.
typedef dl32MouseEvent dl32MouseWheelEvent;       ///< Defines the MouseWheelEvent as an alias of the MouseEvent type.
typedef dl32MouseEvent dl32MouseEnterEvent;       ///< Defines the MouseEnterEvent as an alias of the MouseEvent type.
typedef dl32MouseEvent dl32MouseLeaveEvent;       ///< Defines the MouseLeaveEvent as an alias of the MouseEvent type.


/* Keyboard events: */

/// @brief	Defines an alias representing the cpp_lib32 keyboard event type. This event provides a dl32KeyStrokeData args with keyboard input data.
typedef dl32Event<dl32KeyStrokeData> dl32KeyboardEvent;
typedef dl32KeyboardEvent dl32KeyPressEvent; ///< Defines the KeyPressed event as an alias of the KeyboardEvent type.
typedef dl32KeyboardEvent dl32KeyDownEvent;  ///< Defines the KeyDown event as an alias of the KeyboardEvent type.
typedef dl32KeyboardEvent dl32KeyUpEvent;    ///< Defines the KeyUp event as an alias of the KeyboardEvent type.


/* Window area events: */

/// @brief	 Defines an alias for a window area event. The 2D AABB provided by event args is the window area.
/// @details "Area" refers to the screen zone ocuppied by the window, not only window size. Therefore, window movemments are included in this category.
typedef dl32Event<dl32AABB2D> dl32AreaChangedEvent;
typedef dl32AreaChangedEvent dl32MoveEvent;        ///< Defines the window movemment event as an alias of the AreaChanged event (@see dl32AreaChangedEvent details).
typedef dl32AreaChangedEvent dl32ResizeEvent;      ///< Defines the window resize event as an alias of the AreaChanged event.
typedef dl32AreaChangedEvent dl32ResizeBeginEvent; ///< Defines the window resize begining event as an alias of the AreaChanged event.
typedef dl32AreaChangedEvent dl32ResizeEndEvent;   ///< Defines the window resize ending event as an alias of the AreaChanged event.


/* Miscelaneous: */

typedef dl32NonArgsEvent dl32PaintEvent;      ///< Defines the paint event as a non-args event. 
typedef dl32Event<bool> dl32WindowCloseEvent; ///< Window close event. Booleam argumment allows the user to cancel the window close process (Set to true to cancel). 


////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief	A macro that defines windows procedures arguments.
///
/// @author	Manu343726
/// @date	07/04/2013
////////////////////////////////////////////////////////////////////////////////////////////////////
#define WINDOWS_PROCEDURE_ARGS HWND window , UINT message , WPARAM wParam , LPARAM lParam

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief	A macro that defines windows procedure argumments bypass.
///
/// @author	Manu343726
/// @date	07/04/2013
////////////////////////////////////////////////////////////////////////////////////////////////////
#define WINDOWS_PROCEDURE_BYPASS window , message , wParam , lParam
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief	 Translates system messages to cpp_lib32 events.
/// @details This interface provides a method for create user-defined events. User can use this 
/// 		 interface to translate a specific system message to a cpp_lib32 event.
///
/// @author	Manu343726
/// @date	07/04/2013
////////////////////////////////////////////////////////////////////////////////////////////////////
class dl32EventDispatcher
{
public:
	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief	Destructor.
	///
	/// @author	Manu343726
	/// @date	07/04/2013
	////////////////////////////////////////////////////////////////////////////////////////////////////
	virtual ~dl32EventDispatcher() = 0;

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief	Gets a windows message data and raises the high-level event.
	/// @details This function performs any data-translation operations and raises the high-level event 
	/// 		 with the translated data as the event args.
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
/// @brief	This class provides a set of functions that translate common system messages data to 
/// 		high-level events args.
///
/// @author	Manu343726
/// @date	07/04/2013
////////////////////////////////////////////////////////////////////////////////////////////////////
class dl32SystemMessagesDataTranslator
{
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

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief	 cpp_lib32 high-level events manager.
/// @details Any cpp_lib32 event that acts as a wrapper of a system message (Or messages), are managed
/// 		 by this class.
///
/// @author	Manu343726
/// @date	07/04/2013
////////////////////////////////////////////////////////////////////////////////////////////////////
class dl32EventsManager
{
private:
	unordered_map<UINT,dl32EventDispatcher*> _dispatchers;  ///< Hashmap of event dispatchers. System messages are used as keys.

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief	Sets up the dispatchers for the cpp_lib32 window class default events.
	/// @details cpp_lib32 provides a set of common events defined by default. 
	///
	/// @author	Manu343726
	/// @date	07/04/2013
	////////////////////////////////////////////////////////////////////////////////////////////////////
	void setUpDefaultEvents();

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief	Default constructor. Private, dl32EventsManager implements singleton design pattern.
	///
	/// @author	Manu343726
	/// @date	07/04/2013
	////////////////////////////////////////////////////////////////////////////////////////////////////
	dl32EventsManager() {}
public:
	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief	Destructor.
	///
	/// @author	Manu343726
	/// @date	07/04/2013
	////////////////////////////////////////////////////////////////////////////////////////////////////
	~dl32EventsManager();

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief	Gets the singleton instance of dl32EventsManager class
	///
	/// @author	Manu343726
	/// @date	07/04/2013
	///
	/// @return	.
	////////////////////////////////////////////////////////////////////////////////////////////////////
	dl32EventsManager& instance()
	{ 
		//http://stackoverflow.com/questions/1008019/c-singleton-design-pattern
		static dl32EventsManager instance;

		return instance;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief	A shortcut to the dl32EventsManager instance
	///
	/// @author	Manu343726
	/// @date	07/04/2013
	////////////////////////////////////////////////////////////////////////////////////////////////////
	#define EventsManager dl32EventsManager::instance()

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
	///			 "systemMessages" is a std::vector<UINT>, not only one UINT.
	/// @remarks For example, system has three different messages (WM_LBUTTONDOWN , WM_RBUTTONDOWN , 
	/// 		 WM_MBUTTONDOWN) for mouse down events, one per button. User can define a general 
	/// 		 MouseDown event that catches this three messages.
	/// 		 
	////////////////////////////////////////////////////////////////////////////////////////////////////
	bool setUpEvent(dl32EventDispatcher* dispatcher,const vector<UINT>& systemMessages);

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
	bool setUpEvent(dl32EventDispatcher* dispatcher , UINT systemMessage);

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
	void dispatch(WINDOWS_PROCEDURE_ARGS);
};

#endif