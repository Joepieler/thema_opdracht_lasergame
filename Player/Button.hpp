// ==========================================================================
//
// File      : Button.hpp
// Copyright : bartvannetburg@hotmail.com 2018
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// ==========================================================================

// this file contains Doxygen lines
/// @file

#ifndef BUTTON_HPP
#define BUTTON_HPP

#include "hwlib.hpp"
#include "rtos.hpp"

#include "ButtonListener.hpp"

/// \brief
/// This class is used to make a button object.
class Button : public rtos::task<> {
private:
    hwlib::pin_in & pin_button;
    ButtonListener& listener;
    rtos::clock button_clock;
    unsigned int button_number;
public:
	/// \brief
	/// This is the constructor for a button.
	/// \details
	/// The constructor expects a listener which is the class that owns the button.
    Button( const char * name, int priority, hwlib::pin_in & pin_button, ButtonListener & listener, unsigned int button_number ):
	task( priority, name ),
        pin_button( pin_button ),
        listener( listener ),
        button_clock( this, 100 * rtos::ms, "button_clock" ),
        button_number( button_number )
        {}
		
	/// \brief
	/// The main contains the state machine.
	/// \details
	/// The main only consists of one state the wait_for_button_press state.
	/// The state waits for a clock that gets called every 100 ms.
	/// When the clock is called the button pin gets checked for input.
	/// If there is a input the function buttonpressed in the listener gets called.
	void main() override {
        
	enum states { WAIT_FOR_BUTTON_PRESS };
	states state = states::WAIT_FOR_BUTTON_PRESS;
        for(;;) {
            switch( state ) {
                case states::WAIT_FOR_BUTTON_PRESS: {
                    wait( button_clock );
                    if( pin_button.get() ) {
                        listener.buttonPressed( button_number );
                    }
                    break;
                }
            }
        }
    }
};

#endif
