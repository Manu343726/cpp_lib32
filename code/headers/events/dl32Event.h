/* 
 * File:   dl32Event.h
 * Author: manu343726
 *
 * Created on 30 de junio de 2013, 0:18
 */

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

#ifndef DL32EVENT_H
#define	DL32EVENT_H

#include "dl32MetaprogrammingLibrary.h"

#include <functional>
#include <algorithm>
#include <vector>
#include <iterator>

/****************************************************************************************************************
 * cpp_lib32 high-level events. Can be used to wrap system events (One or more system messages) or to implement *
 * class events.                                                                                                *
 *                                                                                                              *
 * To understand system messages/events wrapping, see dl32SystemEventsManager class at dl32Window.h             * 
 ***************************************************************************************************************/

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief	 cpp_lib32 event type
/// @details     Events are represented as a list of handlers, where handlers are pointers to functions 
/// 		 that handle the event. This functions always be a void function with one or more
/// 		 parameters (A reference to the object that raises the event and the set of event argumments) passed
/// 		 by reference, in other words, functions with prototype "void EventHandler(SENDERTYPE &sender , ARGSTYPES... &args)".
///
///              Also dl32Event template is designed to provide non-args evets (Events without argumments, 
///              with the sender reference as parameter only). To make a non-args event, dont pass anything,
///              to the template parameter "ARGSTYPE". If any type is passed, the template interprets that the
///              instance is a non-args event. 
/// 		 
/// 		 The type of the function-pointers that can be handlers are defined in the HandlerType
/// 		 typedef.
///
/// @tparam	SENDERTYPE Event sender type.
/// @tparam	ARGTYPE	Event argumments types. For non-args events, don't pass anything to this template
///             parameter.
////////////////////////////////////////////////////////////////////////////////////////////////////
template <typename SENDERTYPE, typename... ARGSTYPES>
class dl32Event {
public:
    /// @brief Defines an alias representing type of the event sender.
    typedef SENDERTYPE& SenderType;

    /// @brief Defines an alias representing type of the event sender without reference.
    typedef SENDERTYPE SenderType_NoRef;

    /// @brief	Defines an alias representing function-pointer type of a event handler.
    using HandlerType = void(*)(SenderType,ARGSTYPES&...);
    
    static const bool sender_members_handlers_allowed = dl32IsClass<SENDERTYPE>::value; ///< If the sender is a class, it is allowed to use member functions as handlers of the event.
private:
    
    using _checker_type = std::function<bool(const SENDERTYPE&)>;
    
    template<bool IS_CLASS , typename SENDER , typename... ARGS>
    struct _handler_type_wrapper;
    
    template<typename SENDER , typename... ARGS>
    struct _handler_type_wrapper<true , SENDER , ARGS...>  : public dl32TypeWrapper<void(SENDER::*)(ARGS&...)> {};
    
    template<typename SENDER , typename... ARGS>
    struct _handler_type_wrapper<false , SENDER , ARGS...> : public dl32TypeWrapper<void(*)(SENDER& , ARGS&...)> {};
    
    using _handler_type = typename _handler_type_wrapper<dl32IsClass<SENDERTYPE>::value , SENDERTYPE , ARGSTYPES...>::type;

    
    
    class HandlerData
    {
    public:
        static const _checker_type trivial_sender_checker;
        
    private:
        using _generic_function_pointer = void(*)();
        
        bool _is_sender_handler_member;
        _checker_type _check_sender;
        
        _handler_type _pointer_to_member;
        HandlerType                            _pointer_to_non_member;
        
        
    public:
        template<bool IS_ENABLED = sender_members_handlers_allowed>
        HandlerData( typename dl32EnableIf<IS_ENABLED , _handler_type>::type handler ,
                     _checker_type sender_checker = trivial_sender_checker) 
                     : _is_sender_handler_member( true ) , 
                       _check_sender( sender_checker ) , 
                       _pointer_to_member( handler ) , 
                       _pointer_to_non_member( nullptr )
        {}
                
        HandlerData(HandlerType handler , 
                   _checker_type sender_checker = trivial_sender_checker)  
                   : _is_sender_handler_member( false ) , 
                     _check_sender( sender_checker ) , 
                     _pointer_to_member( nullptr ) , 
                     _pointer_to_non_member( handler )
        {}
        
        template<bool IS_ENABLED = sender_members_handlers_allowed>
        typename dl32EnableIf<IS_ENABLED , void>::type 
        operator()(SENDERTYPE& sender , ARGSTYPES&... args)
        {
            if( !_check_sender( sender ) ) return;
            
            if( _is_sender_handler_member )
                ( sender.*_pointer_to_member )( args... );
            else
                _pointer_to_non_member( sender , args... );
        }
        
        template<bool IS_ENABLED = sender_members_handlers_allowed>
        typename dl32EnableIf<!IS_ENABLED , void>::type 
        operator()(SENDERTYPE& sender , ARGSTYPES&... args)
        {
            if( !_check_sender( sender ) ) return;
            
            _pointer_to_non_member( sender , args... );
        }
        
        friend bool operator==( const HandlerData& a , const HandlerData& b)
        {
            return a._pointer_to_non_member == b._pointer_to_non_member || a.pointer_to_mamber == b._pointer_to_member;
        }
        
        
        friend bool operator==( const HandlerData& data , _handler_type handler)
        {
            return data.pointer_to_non_member == handler || data._pointer_to_member == handler;
        }
        
        template<bool IS_ENABLED = sender_members_handlers_allowed>
        friend typename dl32EnableIf<!IS_ENABLED,bool>::type operator==( const HandlerData& data , HandlerType handler)
        {
            return data._pointer_to_non_member == handler;
        }
    };
    
    std::vector<HandlerData> _handlers;
    
public:   
    static const unsigned int event_args_count = sizeof...(ARGSTYPES); ///< Gets the number of event args of this type of event.
    static const bool is_non_args_event = event_args_count == 0; ///< Checks if this type of event is an event without argumments (With sender parameter only).


    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief	Raises the event.
    /// @param [in,out]	sender Reference to the event sender.
    /// @param [in,out]	args Argumments of the event.
    /// @tparam ARGS_BY_REF If is set to true, event argumments are passed by reference to the handlers.
    ///                     If is set to false, event argumments are passed by value to the handlers.
    ///
    /// @remarks Template parameter NUMBER_OF_ARGS is a parameter to make SFINAE work with class data,
    ///          not only with function data. Is not dessigned to be used by the user. Please not use it.
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    template<bool ARGS_BY_REF = true>
    void RaiseEvent(SenderType sender, dl32MakeReferenceIf<ARGS_BY_REF,ARGSTYPES>... args) {
        for (auto it = _handlers.begin(); it != _handlers.end(); ++it)
            (*it)( sender , args... );
    }


    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief	Adds a function as a handler for this event.
    /// @param	handler	Pointer to the function that will be a handler.
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    void AddHandler(const HandlerType& handler) {
        _handlers.push_back( { handler } );
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief	Adds a function as a handler for this event.
    /// @param	handler	Pointer to the function that will be a handler.
    /// @param  expected_sender Reference to the sender that the handler expects. If the event is raised,
    ///         and the sender of the raise is not this expected sender, the handler is not called.
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    void AddHandler(const HandlerType& handler , const SenderType expected_sender) {
        _handlers.push_back( { handler , [=](const SenderType sender) { return sender == expected_sender; } } );
    }
    
    template<typename SENDER = SENDERTYPE>
    typename dl32EnableIf<dl32IsClass<SENDER>::value , void>::type
    AddHandler(_handler_type handler , const SenderType expected_sender) {
        _handlers.push_back( { handler , [=](const SenderType sender) { return sender == expected_sender; } } );
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief	Removes the handler described by handler.
    /// @param	handler	The handler.
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    void RemoveHandler(const HandlerType& handler) {
        _handlers.erase( std::find_if( std::begin( _handlers ) , [=](const HandlerData& data) { return data == handler; } ) );
    }
};

template<typename SENDER , typename... ARGS>
const typename dl32Event<SENDER,ARGS...>::_checker_type dl32Event<SENDER,ARGS...>::HandlerData::trivial_sender_checker = []( const SENDER& ) { return true; };

#endif	/* DL32EVENT_H */

