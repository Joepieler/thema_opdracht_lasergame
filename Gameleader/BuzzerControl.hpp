// ==========================================================================
//
// File      : BuzzerControl.hpp
// Copyright : bartvannetburg@hotmail.com 2018
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// ==========================================================================

// this file contains Doxygen lines
/// @file

#ifndef BUZZER_CONTROL_HPP
#define BUZZER_CONTROL_HPP

#include "hwlib.hpp"
#include "rtos.hpp"

/// \brief
/// This class is used to control the buzzer.
class BuzzerControl : public rtos::task<> {
private:
    hwlib::pin_out & buzz_pin;
    
    rtos::flag buzz_on_flag;
    
    rtos::timer buzz_length_timer;
    rtos::timer buzz_interval_timer;
    
    rtos::pool<int> buzz_pool;
    
    int buzz_length = 0;
    int buzz_interval = 100;
    const int buzzer_on = 1;
    const int buzzer_off = 0;
public:
/// \brief
/// This is the constructer for buzzer control.
/// \details
/// The constructer wants a pin_out buzz pin.
    BuzzerControl( const char * name, int priority, hwlib::pin_out & buzz_pin ):
        task( name, priority ),
        buzz_pin( buzz_pin ),
        buzz_on_flag( this, "buzz on flag" ),
        buzz_length_timer( this, "buzz length timer" ),
        buzz_interval_timer( this, "buzz interval timer" ),
        buzz_pool("buzz pool")
        {}
        
/// \brief
/// This function writes the buzz_length in the buzz_pool and sets the buzz_on_flag.
    void makeSound( int buzz_length ) {
        buzz_pool.write( buzz_length );
        buzz_on_flag.set();
    }
      
/// \brief
/// This is the state function for the buzzer.
/// \details
/// This function has 3 states: IDLE, ON, OFF. If the state is IDLE, the function waits for the buzz_on_flag, then he reads the buzz_pool, set the length timer and switch the state to ON.
/// If the state is ON, buzz_pin is set high, interval_timer is set, he waits for the buzz_interval_timer and the state will be OFF.
/// If the state is OFF, buzz_pin is set low, interval_timer is set and if event is the same as buzz_length_timer, the state will be IDLE.
/// If event is the ame as buzz_interval_timer, the state will be ON.
    void main() override {
        enum states { IDLE, ON, OFF };
        states state = states::IDLE;
        for(;;) {
            switch( state ) {
                case states::IDLE: {
                    wait( buzz_on_flag );
                    buzz_length = buzz_pool.read();
                    buzz_length_timer.set( buzz_length * rtos::us );
                    state = states::ON;
                    break;
                }
                case states::ON: {
                    buzz_pin.set( buzzer_on );
                    buzz_interval_timer.set( buzz_interval * rtos::us );
                    wait( buzz_interval_timer );
                    state = states::OFF;
                    break;
                }   
                case states::OFF: {
                    buzz_pin.set( buzzer_off );
                    buzz_interval_timer.set( buzz_interval * rtos::us );
                    auto event = wait( buzz_length_timer + buzz_interval_timer );
                    if( event == buzz_length_timer ) {
                        state = states::IDLE;
                    }
                    else if( event == buzz_interval_timer ) {
                        state = states::ON;
                    }
                    break;
                }
            }
        }
    }
};

#endif
    
    