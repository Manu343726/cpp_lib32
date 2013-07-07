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
#include "dl32OperatorOverloadingHelpers.h"

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
/// @brief	 cpp_lib32 basic event data.
///
/// @tparam	SENDER Event sender type.
/// @tparam	ARGS   Event argumments types. For non-args events, don't pass anything to this variadic
///                    pack.
////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename SENDER , typename... ARGS>
class dl32BasicEventData
{
public:
    using SenderParam      = SENDER&;
    using SenderParamNoRef = SENDER;
    using GlobalHandler    = void(*)(SenderParam , ARGS&...);
    
    static const unsigned int args_count = sizeof...(ARGS);
    static const bool non_args_event     = args_count == 0;
    
protected:
    using _sender_check_function = std::function<bool(const SENDER&)>;
    static const _sender_check_function _trivial_checker;
};

template<typename SENDER , typename... ARGS>
const typename dl32BasicEventData<SENDER,ARGS...>::_sender_check_function dl32BasicEventData<SENDER,ARGS...>::_trivial_checker = [](const SENDER&) { return true;  };

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief	 cpp_lib32 common event operations. CRTP class used to share common operations
///              implementation between every type of event.
///
/// @tparam     EVENT  Template template param with the type of the event.
/// @tparam	SENDER Event sender type.
/// @tparam	ARGS   Event argumments types. For non-args events, don't pass anything to this variadic
///                    pack.
////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename EVENT>
class dl32CommonEventOperations;

template<template<typename , typename...> class EVENT ,typename SENDER , typename... ARGS>
class dl32CommonEventOperations<EVENT<SENDER,ARGS...>>
{
private:
    using event_type = EVENT<SENDER,ARGS...>;
        
    event_type* _event_this;

protected:    
    dl32CommonEventOperations() : _event_this( static_cast<event_type*>( this ) ) {};
    
public:
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief	Raises the event.
    /// @param [in,out]	sender Reference to the event sender.
    /// @param [in,out]	args Argumments of the event.
    /// @tparam ARGS_BY_REF If is set to true, event argumments are passed by reference to the handlers.
    ///                     If is set to false, event argumments are passed by value to the handlers.
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    template<bool ARGS_BY_REF = true>
    void RaiseEvent(SENDER& sender, dl32MakeReferenceIf<ARGS_BY_REF,ARGS>... args) {
        for(auto it = _event_this->_handlers.begin(); it != _event_this->_handlers.end(); ++it)
            (*it)( sender , args... );
    }
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief	 cpp_lib32 event type. Only global/static functions are allowed to be handlers.
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
/// @tparam	SENDER Event sender type.
/// @tparam	ARGS   Event argumments types. For non-args events, don't pass anything to this variadic
///                    pack.
////////////////////////////////////////////////////////////////////////////////////////////////////
template <typename SENDER, typename... ARGS>
class dl32EventGlobalHandlersOnly : public dl32BasicEventData<SENDER,ARGS...> , public dl32CommonEventOperations<dl32EventGlobalHandlersOnly<SENDER,ARGS...>>
{
    friend class dl32CommonEventOperations<dl32EventGlobalHandlersOnly<SENDER,ARGS...>>;
private:    
    class HandlerData
    {
    private:
        typename dl32BasicEventData<SENDER,ARGS...>::_sender_check_function _check_sender;
        typename dl32BasicEventData<SENDER,ARGS...>::GlobalHandler          _pointer_to_non_member;
        
    public:     
        HandlerData(typename dl32BasicEventData<SENDER,ARGS...>::GlobalHandler handler , 
                    typename dl32BasicEventData<SENDER,ARGS...>::_sender_check_function sender_checker)  
                    : _check_sender( sender_checker ) , 
                      _pointer_to_non_member( handler )
        {}
        
        void operator()(SENDER& sender , ARGS&... args)
        {
            if( !_check_sender( sender ) ) return;
            
            _pointer_to_non_member( sender , args... );
        }
        
        friend bool operator==( const HandlerData& a , const HandlerData& b)
        {
            return a._pointer_to_non_member == b._pointer_to_non_member;
        }
        
        
        friend bool operator==( const HandlerData& data , typename dl32BasicEventData<SENDER,ARGS...>::GlobalHandler handler)
        {
            return data._pointer_to_non_member == handler;
        }
    };
    
    std::vector<HandlerData> _handlers;
    
public:   
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief	Adds a function as a handler for this event.
    /// @param	handler	Pointer to the function that will be a handler.
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    void AddHandler(typename dl32BasicEventData<SENDER,ARGS...>::GlobalHandler handler) {
        _handlers.push_back( { handler , [](const SENDER&){ return true; } } );
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief	Adds a function as a handler for this event.
    /// @param	handler	Pointer to the function that will be a handler.
    /// @param  expected_sender Reference to the sender that the handler expects. If the event is raised,
    ///         and the sender of the raise is not this expected sender, the handler is not called.
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    void AddHandler(typename dl32BasicEventData<SENDER,ARGS...>::GlobalHandler handler , const SENDER& expected_sender) {
        _handlers.push_back( { handler , [=](const SENDER& sender) { return sender == expected_sender; } } );
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief	Removes the handler described by handler.
    /// @param	handler	The handler.
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    void RemoveHandler(typename dl32BasicEventData<SENDER,ARGS...>::GlobalHandler handler) {
        _handlers.erase( std::find_if( std::begin( _handlers ) , [=](const HandlerData& data) { return data == handler; } ) );
    }
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief	 cpp_lib32 event type for class sender.
/// @details     Events are represented as a list of handlers, where handlers are pointers to functions 
/// 		 that handle the event. This functions always be a void function with one or more
/// 		 parameters (A reference to the object that raises the event and the set of event argumments) passed
/// 		 by reference, in other words, functions with prototype "void EventHandler(SENDERTYPE &sender , ARGSTYPES... &args)".
///              In addition, this tempate allows to use SENDER member functions as handlers of the event. 
///
///              Also dl32Event template is designed to provide non-args evets (Events without argumments, 
///              with the sender reference as parameter only). To make a non-args event, dont pass anything,
///              to the template parameter "ARGSTYPE". If any type is passed, the template interprets that the
///              instance is a non-args event. 
/// 		 
/// 		 The type of the function-pointers that can be handlers are defined in the HandlerType
/// 		 typedef.
///
/// @tparam	SENDER Event sender type.
/// @tparam	ARGS   Event argumments types. For non-args events, don't pass anything to this template
///                    parameter.
////////////////////////////////////////////////////////////////////////////////////////////////////
template <typename SENDER, typename... ARGS>
class dl32EventMemberHandlersAllowed : public dl32BasicEventData<SENDER,ARGS...> , public dl32CommonEventOperations<dl32EventMemberHandlersAllowed<SENDER,ARGS...>>
{
    friend class dl32CommonEventOperations<dl32EventMemberHandlersAllowed<SENDER,ARGS...>>;
public:
    using MemberHandler = void(SENDER::*)(ARGS...); ///< Defines the type of SENDER member functions that can be handlers of the event.
    static const bool sender_members_handlers_allowed = true; ///< If the sender is a class, it is allowed to use member functions as handlers of the event.
    
private:
    class HandlerData
    {
    private:
        bool _is_sender_handler_member;
        typename dl32BasicEventData<SENDER,ARGS...>::_sender_check_function _check_sender;
        
        MemberHandler _pointer_to_member;
        typename dl32BasicEventData<SENDER,ARGS...>::GlobalHandler _pointer_to_non_member;     
        
    public:
        HandlerData( MemberHandler handler ,
                     typename dl32BasicEventData<SENDER,ARGS...>::_sender_check_function sender_checker) 
                     : _is_sender_handler_member( true ) , 
                       _check_sender( sender_checker ) , 
                       _pointer_to_member( handler ) , 
                       _pointer_to_non_member( nullptr )
        {}
                
        HandlerData(typename dl32BasicEventData<SENDER,ARGS...>::GlobalHandler handler , 
                    typename dl32BasicEventData<SENDER,ARGS...>::_sender_check_function sender_checker)  
                    : _is_sender_handler_member( false ) , 
                      _check_sender( sender_checker ) , 
                      _pointer_to_member( nullptr ) , 
                      _pointer_to_non_member( handler )
        {}
        
        void operator()(SENDER& sender , ARGS&... args)
        {
            if( !true ) return;
            
            if( _is_sender_handler_member )
                ( sender.*_pointer_to_member )( args... );
            else
                _pointer_to_non_member( sender , args... );
        }
        
        friend bool operator==( const HandlerData& a , const HandlerData& b)
        {
            return a._pointer_to_non_member == b._pointer_to_non_member || a._pointer_to_member == b._pointer_to_member;
        } 
        
        friend bool operator==( const HandlerData& data , typename dl32BasicEventData<SENDER,ARGS...>::GlobalHandler handler)
        {
            return data._pointer_to_non_member == handler;
        }
        
        friend bool operator==( const HandlerData& data , MemberHandler handler)
        {
            return data._pointer_to_member == handler;
        }
    };
    
    std::vector<HandlerData> _handlers;
    
public:   
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief	Adds a function as a handler for this event.
    /// @param	handler	Pointer to the function that will be a handler.
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    void AddHandler(typename dl32BasicEventData<SENDER,ARGS...>::GlobalHandler handler) {
        _handlers.push_back( { handler , [](const SENDER&){ return true; } } );
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief	Adds a function as a handler for this event.
    /// @param	handler	Pointer to the function that will be a handler.
    /// @param  expected_sender Reference to the sender that the handler expects. If the event is raised,
    ///         and the sender of the raise is not this expected sender, the handler is not called.
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    void AddHandler(typename dl32BasicEventData<SENDER,ARGS...>::GlobalHandler handler , const SENDER& expected_sender) {
        _handlers.push_back( { handler , [=](const SENDER& sender) { return sender == expected_sender; } } );
    }
    
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief	Adds a function as a handler for this event.
    /// @param	handler	Pointer to the member function that will be a handler.
    /// @param  expected_sender Reference to the sender wich the member function belongs.
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    void AddHandler(MemberHandler handler , const SENDER& expected_sender) {
        _handlers.push_back( HandlerData( handler , [=](const SENDER& sender) { return sender == expected_sender; } ) );
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief	Removes the handler described by handler.
    /// @param	handler	The handler.
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    void RemoveHandler(typename dl32BasicEventData<SENDER,ARGS...>::GlobalHandler handler) {
        _handlers.erase( std::find_if( std::begin( _handlers ) , [=](const HandlerData& data) { return data == handler; } ) );
    }
    
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief	Removes the handler described by handler.
    /// @param	handler	The handler.
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    void RemoveHandler(MemberHandler handler) {
        _handlers.erase( std::find_if( std::begin( _handlers ) , [=](const HandlerData& data) { return data == handler; } ) );
    }
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief	 cpp_lib32 event type
/// @details     Events are represented as a list of handlers, where handlers are pointers to functions 
/// 		 that handle the event. This functions always be a void function with one or more
/// 		 parameters (A reference to the object that raises the event and the set of event argumments) passed
/// 		 by reference, in other words, functions with prototype "void EventHandler(SENDERTYPE &sender , ARGSTYPES... &args)".
///              In addition, if SENDER is a class type, this tempate allows to use SENDER member functions as handlers of the event. 
///
///              Also dl32Event template is designed to provide non-args evets (Events without argumments, 
///              with the sender reference as parameter only). To make a non-args event, dont pass anything,
///              to the template parameter "ARGSTYPE". If any type is passed, the template interprets that the
///              instance is a non-args event. 
///
/// @tparam	SENDER Event sender type.
/// @tparam	ARGS   Event argumments types. For non-args events, don't pass anything to this template
///                    parameter.
////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename SENDER , typename... ARGS>
class dl32Event : public dl32SelectType<dl32IsClass<SENDER>::value , dl32EventMemberHandlersAllowed<SENDER,ARGS...> , dl32EventGlobalHandlersOnly<SENDER,ARGS...>>::type {};
#endif	/* DL32EVENT_H */

