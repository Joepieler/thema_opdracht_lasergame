// ==========================================================================
//
// File      : DisplayControl.hpp
// Copyright : bartvannetburg@hotmail.com 2018
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// ==========================================================================

// this file contains Doxygen lines
/// @file

#ifndef DISPLAY_CONTROL_HPP
#define DISPLAY_CONTROL_HPP

#include "hwlib.hpp"
#include "rtos.hpp"

/// \brief
/// This class is used to show the command value on the screen.
class DisplayControl : public rtos::task<> {
private:
    rtos::flag display_command_flag;
    rtos::flag clear_flag;
        
    rtos::pool<uint8_t> command_pool;
    
    const uint8_t display_size_y = 64;
    const uint8_t display_size_x = 128;
    const uint8_t devider = 80;
    const uint8_t font_height = 8;
    
    hwlib::font_default_8x8 font = hwlib::font_default_8x8();

    hwlib::glcd_oled & display;

    hwlib::window_part value_names = hwlib::window_part( display, hwlib::location( 0, 0 ), hwlib::location( devider, font_height ) );	
    hwlib::window_ostream value_names_ostream = hwlib::window_ostream( value_names, font );
    
    hwlib::window_part command_value = hwlib::window_part( display, hwlib::location( 0, font_height * 3 ), hwlib::location( display_size_x, font_height * 4 ) );
    hwlib::window_ostream command_value_ostream = hwlib::window_ostream( command_value, font );
    
    int display_delay = 500;
    int star_value = 42;
public:
/// \brief
/// This is the constructor for a display.
/// \details
/// The constructor expects a display.
    DisplayControl( const char * name, int priority, hwlib::glcd_oled & display ):
        task( priority, name ),
        display_command_flag( this, "display command flag" ),
        clear_flag( this, "display clear flag" ),
        command_pool( "display command pool" ),
        display( display )
    {
        display.clear();
        value_names_ostream 
            << "\t0000Command:"
            << hwlib::flush;
    }
    
/// \brief
/// This function sets a flag en writes the value in the command_pool.
    void showCommand( uint8_t value ) {
        display_command_flag.set();
        command_pool.write( value );
    }
    
/// \brief
/// This function sets a flag to clear the screen.
    void clear() {
        display_command_flag.set();
    }
    
/// \brief
/// This function updates the command value on the screen.
/// \details
/// If the value is 42, the function makes a star of it. This is special for the star on the gamepad.
/// Else the function will print the int value on the screen. 
    void updateCommandValue( uint8_t value ) {
        if( value == star_value ) {
            command_value_ostream 
                << "\t0300" 
                << char( value ) 
                << hwlib::flush;
        }
        else {
            command_value_ostream 
                << "\t0300"
                << int( value )
                << hwlib::flush;
        }
    }
    
/// \brief
/// This function clears the command value on the screen.
    void clearCommandValue() {
        command_value.clear();
    }

/// \brief
/// This is the state function for the display class.
/// \details
/// This function has one state: WAIT_FOR_MESSAGE.
/// The function will wait for the display_command_flag and will wait and call a function where he will read the pool.
/// Else if he will wait for the clear_flag and will call a function.
    void main() override {
        enum states { WAIT_FOR_MESSAGE };
        states state = states::WAIT_FOR_MESSAGE;
        for(;;) {
            switch( state ) {
                case states::WAIT_FOR_MESSAGE: {
                    auto event = wait( display_command_flag + clear_flag );
                    if( event == display_command_flag ){
                        hwlib::wait_ms( display_delay );
                        updateCommandValue( command_pool.read() );
                    } 
                    else if( event == clear_flag ) {
                        clearCommandValue();
                    } 
                    break;
                }
            }
        }
    }
};

#endif