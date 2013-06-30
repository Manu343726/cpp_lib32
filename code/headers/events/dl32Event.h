/* 
 * File:   dl32Event.h
 * Author: manu343726
 *
 * Created on 30 de junio de 2013, 0:18
 */

#ifndef DL32EVENT_H
#define	DL32EVENT_H

#include <functional>
#include <vector>

/****************************************************************************************************************
 * cpp_lib32 high-level events. Can be used to wrap system events (One or more system messages) or to implement *
 * class events.                                                                                                *
 *                                                                                                              *
 * To understand system messages/events wrapping, see dl32SystemEventsManager class at dl32Window.h             * 
 ***************************************************************************************************************/

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief	 cpp_lib32 event type
/// @details     Events are represented as a list of handlers, where handlers are pointers to functions 
/// 		 that handle the event. This functions always be a void function with one or two
/// 		 parameters (A reference to the object that raises the event and event argumments) passed
/// 		 by reference, in other words, functions with prototype "void EventHandler(SENDERTYPE &sender , ARGSTYPE &args)".
///              Template parameter "SENDERTYPE" is the type of the sender.
/// 		 Template parameter "ARGSTYPE" is the type of these event args. For non-args event, use
/// 		 void, the template is specialized for that case.
/// 		 
/// 		 The type of the function-pointers that can be handlers are defined in the HandlerType
/// 		 typedef.
/// @tparam	SENDERTYPE Event sender type.
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
    typedef typename dl32SelectType<dl32SameType<ARGSTYPE,void>::value, ARGSTYPE, ARGSTYPE&>::type ArgummentsType;

    /// @brief	Defines an alias representing the argumments type without reference.
    typedef ARGSTYPE ArgummentsType_NoRef;

    /// @brief	Defines an alias representing function-pointer type of a event handler.
    typedef void(*HandlerType)(SenderType,ArgummentsType);

private:
    struct HandlerData
    {
        HandlerType handler;
        
        using is_expected_sender_type = std::function<bool(SenderType)>;
        is_expected_sender_type is_expected_sender;
        
        HandlerData(HandlerType handler , is_expected_sender_type expected_sender_check) : handler(handler) , is_expected_sender(expected_sender_check) {}
    };
    
    std::vector<HandlerData> _handlers;
public:


    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief	Raises the event.
    /// @param [in,out]	sender Reference to the event sender.
    /// @param [in,out]	args Argumments of the event.
    /// @tparam ARGS_BY_REF If is set to true, event argumments are passed by reference to the handlers.
    ///                     If is set to false, event argumments are passed by value to the handlers.
    /// @remarks Template parameter ARGUMMENTS_TYPE is a bridge to make SFINAE work. Is not designed to be setted by the user.
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    template<bool ARGS_BY_REF = true , typename ARGUMMENTS_TYPE = ARGSTYPE>
    void RaiseEvent(typename dl32EnableIf<!dl32SameType<ARGUMMENTS_TYPE,void>::value,SenderType>::type sender, typename dl32SelectType<ARGS_BY_REF , ArgummentsType , ArgummentsType_NoRef>::type args) {
        static_assert(dl32SameType<ARGUMMENTS_TYPE , ARGSTYPE>::value , "template parameter 'ARGUMMENTS_TYPE' is not designed to be setted by the user. Please not use it");
        
        for (auto it = _handlers.begin(); it != _handlers.end(); ++it)
            if( it->is_expected_sender( sender ) )
                it->handler(sender, args);
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief	Raises the event.
    /// @param [in,out]	sender Reference to the event sender.
    /// @remarks Specialitation for non-argumments events.
    /// @remarks Template parameter ARGUMMENTS_TYPE is a bridge to make SFINAE work. Is not designed to be setted by the user.
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

#endif	/* DL32EVENT_H */

