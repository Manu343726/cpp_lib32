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

#ifndef DL32EVENTS_H
#define DL32EVENTS_H

#include "dl32Config.h"
#include "dl32Math.h"
#include "dl32Typing.h"

#include <Windows.h>

#include <unordered_map>
#include <vector>
#include <functional>
#include <algorithm>

using namespace std;


/// @brief	Mouse buttons enumeration
enum class dl32MouseButton {
    NONE, ///< No button
    RIGHT, ///< Right button
    CENTER, ///< Center button (Mouse wheel button, normally)
    LEFT ///< Left button
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief	Basic mouse data.
///
/// @author	Manu343726
/// @date	07/04/2013
////////////////////////////////////////////////////////////////////////////////////////////////////
struct dl32MouseData {
    dl32Point2D Location; ///< Mouse-pointer in local coordinates. 
    dl32MouseButton Button; ///< Pressed mouse button (@see dl32MouseButton).
    int Delta; ///< Mouse wheel delta.

    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief dl32MouseData ctor
    ///
    /// @param location Mouse coordinates
    /// @param button Pressed button
    /// @param delta MouseWheel delta 
    ///
    /// @author	Manu343726
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    dl32MouseData(dl32Point2D location, dl32MouseButton button, int delta) : Location(location), Button(button), Delta(delta) {
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief dl32MouseData default ctor.
    ///
    /// @author	Manu343726
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    dl32MouseData() {
    }

    //Apaño:

    operator dl32MouseData & () {
        return *this;
    }
};

/// @brief	cpp_lib32 keyboard codes
typedef char dl32KeyCode;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief	Basic keystroke data.
///
/// @author	Manu343726
/// @date	07/04/2013
////////////////////////////////////////////////////////////////////////////////////////////////////
struct dl32KeyStrokeData {
    bool PreviousPressed; ///< The previous key state. true if its down before, false if its up.
    dl32KeyCode Key; ///< Key code.
    int RepeatCount; ///< If the previous key state is "pressed", number of times the keystroke is autorepeated as a result of the user holding down the key. Cero in other case.
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief	A macro that defines windows procedure arguments types.
///
/// @author	Manu 343726
/// @date	13/04/2013
////////////////////////////////////////////////////////////////////////////////////////////////////
#define WINDOWS_PROCEDURE_ARGS_TYPES HWND , UINT , WPARAM , LPARAM

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief	A macro that defines windows procedures arguments.
/// 		
/// @param	window 	Handle of the window.
/// @param	message	System message code.
/// @param	wParam 	The wParam field of the message.
/// @param	lParam 	The lParam field of the message.
///
/// @author	Manu343726
/// @date	07/04/2013
////////////////////////////////////////////////////////////////////////////////////////////////////
#define WINDOWS_PROCEDURE_ARGS HWND window , UINT message , WPARAM wParam , LPARAM lParam

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief	A macro that defines windows procedures argumments bypass.
///
/// @param	window 	Handle of the window.
/// @param	message	System message code.
/// @param	wParam 	The wParam field of the message.
/// @param	lParam 	The lParam field of the message.
///
/// @author	Manu343726
/// @date	07/04/2013
////////////////////////////////////////////////////////////////////////////////////////////////////
#define WINDOWS_PROCEDURE_BYPASS window , message , wParam , lParam

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief	 cpp_lib32 event type
/// @details     Events are represented as a list of handlers, where handlers are pointers to functions 
/// 		 that handle the event. This functions always be a void function with cero or one
/// 		 parameter (The argumments of the event) passed by reference, in other words, functions 
/// 		 with prototype "void EventHandler(ARGSTYPE &args)".
/// 		 Template parameter "ARGSTYPE" is the type of these event args. For non-args event, use
/// 		 void.
/// 		 
/// 		 The type of the function-pointers that can be handlers are defined in the HandlerType
/// 		 typedef.
/// @tparam	ARGTYPE	Event argumments type. For non-args events, use void.
////////////////////////////////////////////////////////////////////////////////////////////////////
template <typename SENDERTYPE, typename ARGSTYPE>
class dl32Event {
public:
    /// @brief Defines an alias representing type of the event sender.
    typedef SENDERTYPE& SenderType;

    /// @brief Defines an alias representing type of the event sender without reference.
    typedef SENDERTYPE SenderType_NoRef;

    /// @brief	Defines an alias representing type of event argumments.
    typedef typename dl32Select<dl32TypeTraits<ARGSTYPE>::isVoid, ARGSTYPE, ARGSTYPE&>::result ArgummentsType;

    /// @brief	Defines an alias representing the argumments type without reference.
    typedef ARGSTYPE ArgummentsType_NoRef;

    /// @brief	Defines an alias representing function-pointer type of a event handler.
    typedef void(*HandlerType)(SenderType,ArgummentsType);

#if !defined( _MSC_VER ) //MSVC no implementa todavía template varargs...

    /// @brief	Defines an alias representing type of the dispatcher.
    using DispatcherType = std::function<ARGSTYPE(WINDOWS_PROCEDURE_ARGS_TYPES)>;

#else          // ... así que me veo obligado a usar feos funteros

    /// @brief	Defines an alias representing type of the dispatcher.
    using DispatcherType = ARGSTYPE(WINDOWS_PROCEDURE_ARGS_TYPES);

#endif
private:
    struct HandlerData
    {
        HandlerType handler;
        
#if !defined( _MSC_VER ) //Por la misma razón que en la definición de DispatcherType: Compativilidad con MSVC.
        using is_expected_sender_type = std::function<bool(SenderType)>;
#else
        typedef bool (*is_expected_sender_type) (const SenderType&);
#endif
        is_expected_sender_type is_expected_sender;
        
        HandlerData(HandlerType handler , is_expected_sender_type expected_sender_check) : handler(handler) , is_expected_sender(expected_sender_check) {}
    };
    
    vector<HandlerData> _handlers;
public:


    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief	Raises the event.
    /// @param [in,out]	args Argumments of the event.
    /// @tparam ARGS_BY_REF If is set to true, event argumments are passed by reference to the handlers.
    ///                     If is set to false, event argumments are passed by value to the handlers.
    /// @remarks Template parameter ARGUMMENTS_TYPE is a bridge for make SFINAE working. Is not designed to be setted by the user.
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    template<bool PASS_BY_REF = true , typename ARGUMMENTS_TYPE = ARGSTYPE>
    void RaiseEvent(typename dl32EnableIf<!dl32SameType<ARGUMMENTS_TYPE,void>::value,SenderType>::type sender, dl32Select<PASS_BY_REF , ArgummentsType , ArgummentsType_NoRef>::result args) {
        static_assert(dl32SameType<ARGUMMENTS_TYPE , ARGSTYPE>::value , "template parameter 'ARGUMMENTS_TYPE' is not designed to be setted by the user. Please not use it");
        
        for (auto it = _handlers.begin(); it != _handlers.end(); ++it)
            if( it->is_expected_sender( sender ) )
                it->handler(sender, args);
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief	Raises the event.
    /// @Remarks Specialitation for non-argumments events.
    /// @remarks Template parameter ARGUMMENTS_TYPE is a bridge for make SFINAE working. Is not designed to be setted by the user.
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    template<typename ARGUMMENTS_TYPE = ARGSTYPE>
    void RaiseEvent(typename dl32EnableIf<dl32SameType<ARGUMMENTS_TYPE,void>::value,SenderType>::type sender) {
        static_assert(dl32SameType<ARGUMMENTS_TYPE , ARGSTYPE>::value , "template parameter 'ARGUMMENTS_TYPE' is not designed to be setted by the user. Please not use it");
        
        for (auto it = _handlers.begin(); it != _handlers.end(); ++it)
            if( it->is_expected_sender( sender ) )
                it->handler(sender);
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief	Adds a function as a handler for this event.
    /// @param	handler	Pointer to the function that will be a handler.
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    void AddHandler(HandlerType handler) {
        _handlers.push_back( HandlerData( handler , [](const SenderType) { return true; } ) );
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief	Adds a function as a handler for this event.
    /// @param	handler	Pointer to the function that will be a handler.
    /// @param  expected_sender Reference to the sender that the handler expects. If the event is raised,
    ///         and the sender of the raise is not this expected sender, the handler is not called.
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    void AddHandler(HandlerType handler , const SenderType expected_sender) {
        _handlers.push_back( HandlerData( handler , [=](const SenderType sender) { return sender == expected_sender; } ) );
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief	Removes the handler described by handler.
    /// @param	handler	The handler.
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    void RemoveHandler(HandlerType handler) {
        _handlers.erase( std::remove_if( std::begin( _handlers ) , std::end( _handlers ) , [=](const HandlerData& data) { return data.handler == handler; } ) );
    }
};
#endif