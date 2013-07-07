/* 
 * File:   dl32Event_test.cpp
 * Author: manu343726
 *
 * Created on 30 de junio de 2013, 16:05
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

#include "dl32TestConfig.h"

#if DL32TESTS_CURRENTTEST == DL32TEST_REFACTORING_EVENTTEST

#include "dl32Event.h"
#include "dl32Demangling.h"

#include <iostream>
#include <chrono>
#include <thread>

using std::chrono::duration_cast;
using std::chrono::microseconds;
using std::chrono::steady_clock;

void test(); //Forward declaration of test.


class TurnCounter
{
private:
    unsigned int _count;
    unsigned int _event_trigger;
    
    void _on_event()
    {
        std::cout << "****************************************" << std::endl;
        cout << _event_trigger << " increment. Current count: " << _count << endl;
        std::cout << "****************************************" << std::endl;
    }
    
public:
    dl32Event<TurnCounter> IncrementEvent; //Non-args event
    
    TurnCounter(unsigned int trigger = 1) : _count( 0 ) , _event_trigger( trigger )
    {
        IncrementEvent.AddHandler( &TurnCounter::_on_event  , *this );
        
        //cout << dl32Demangle( decltype( &TurnCounter::_on_event ) ) << endl;
    }
    
    TurnCounter& operator++()
    {
        _count++;
        
        if( _count % _event_trigger == 0 ) 
            IncrementEvent.RaiseEvent(*this);
    }
    
    unsigned int count() const { return _count; }
    
    void reset() { _count = 0; }
    
    friend bool operator==(const TurnCounter& a , const TurnCounter& b)
    {
        return a._count == b._count && a._event_trigger == b._event_trigger;
    }
};

//                            sender type                        event args
using TurnEnd  = dl32Event< decltype( test ) , steady_clock::duration , unsigned int>;
using TurnStep = dl32Event< decltype( test ) , steady_clock::duration , float>;

void OnTurnEnd( typename TurnStep::SenderParam sender , steady_clock::duration& total_time , unsigned int& total_turns )
{
    std::cout << "=================================================================" << std::endl;
    std::cout << "Circle turn end: Total turns = " << total_turns << " turn time = " << duration_cast<std::chrono::seconds>( total_time ).count() << " seconds" << std::endl;
    std::cout << "=================================================================" << std::endl;
}

void OnTurnStep( typename TurnStep::SenderParam sender , steady_clock::duration& total_time , float& current_angle )
{
    std::cout << "Circle turn step... Angle = " << current_angle << " total time = " << duration_cast<std::chrono::seconds>( total_time ).count() << " seconds" << std::endl;
}

int main()
{
    test();
}

void test()
{   
    /* Events setup */
    
    TurnEnd turn_end_event;
    TurnStep turn_step_event;
    
    turn_end_event .AddHandler( OnTurnEnd  );
    turn_step_event.AddHandler( OnTurnStep );
    
    /* Events test: Circle turning */
    
    const float PI = 3.141592654;
    const float step = 0.057;
    TurnCounter counter( 2 ); //Member handled event triggered every 10 turns.
    
    auto begin = steady_clock::now();
    
    for(float angle = 0 ; angle <= 2*PI ; angle += step)
    {
        auto end = steady_clock::now();
        auto interval = end - begin;
        
        turn_step_event.RaiseEvent( test , interval , angle );
        
        if( angle + step >= 2*PI)
        {
            ++counter;
            angle = 0;
            
            turn_end_event.RaiseEvent<false>( test , interval , counter.count() );
            
            begin = steady_clock::now();
        }
        
        std::this_thread::sleep_for( std::chrono::milliseconds( 10 ) ); //Si no va demasiado rápido y es muy dificil de seguir
    }
    
}
#endif /* CURRENT_TEST */
