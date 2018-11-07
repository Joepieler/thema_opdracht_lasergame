// ==========================================================================
//
// File      : KeypadControl.hpp
// Copyright : bartvannetburg@hotmail.com 2018
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// ==========================================================================

// this file contains Doxygen lines
/// @file

#ifndef KEYPAD_CONTROL_HPP
#define KEYPAD_CONTROL_HPP

#include "hwlib.hpp"
#include "rtos.hpp"

#include "keypad.hpp"
#include "GameLeader.hpp"
#include "DisplayControl.hpp"

/// \brief
/// This class is used to control the keypad.
class KeypadControl : public Keypad, rtos::task<> {
private:
    const int max_command = 15;
    Keypad keypad = Keypad();
    rtos::clock receive_clock;
    GameLeader & game_leader;
    DisplayControl & display_control;
    
/// \brief
/// This function is given a character and returns a number.
    int getNumber( char c ) {
        return ( c-'0' );
    }
public:
/// \brief
/// This is the constructor for keypad control.
/// \details
/// The constructor wants a GameLeader and a DisplayControl.
    KeypadControl( const char* name, int priority, GameLeader & game_leader, DisplayControl & display_control ):
        task( priority, name ),
        receive_clock( this, 100 * rtos::ms, "receive_clock" ),
        game_leader( game_leader ),
        display_control( display_control )
    {}
    
/// \brief
/// This is the state function for the keypad class.
/// \details
/// This function has 3 states: IDLE, TYPE_COMMAND and SEND_COMMAND.
/// If the state is IDLE, the function waits for the receive_clock and will look for the pressed key.If the pressed key is C then the state will be TYPE_COMMAND.
/// If the state is TYPE_COMMAND, the function waits for the receive_clock and will look for the pressed key. If the pressed key is C, command will be 0 and the function clearCommandValue will be called and then the state will be IDLE.
/// Else if the pressed key is between 0 and 9, command will be multiplied by 10 and the function getNumber will be called.
/// Else if the pressed key is *, the funtion startGame will be called, command will be changed to * and on the screen you can see the command.
/// Else if the pressed key is #, when command is between 1 and max_command,on the screen you can see the command and state will be SEND_COMMAND.
/// If command isn't between 1 and max_command, command will be 0.
/// If the state is SEND_COMMAND, the function waits for the receive_clock and will look for the pressed key. If the pressed key is #, the function setGameLength will be called. 
/// If the pressed key is C, command will be 0, the function clearCommandValue will be called and the state will be IDLE. 
    void main() override {
        enum status { IDLE, TYPE_COMMAND, SEND_COMMAND };
        status state = status::IDLE;
        int command = 0;
        for (;;) {
            switch( state ) {
                case status::IDLE: {
                    wait( receive_clock );
                    char keyvalue = keypad.getc();
                    if( keyvalue == 'C' ) {
                        state = status::TYPE_COMMAND;
                    }
                    break;
                }
                case status::TYPE_COMMAND: {
                    wait( receive_clock );
                    char keyvalue = keypad.getc();
                    if( keyvalue == 'C' ){
                        command = 0;
                        display_control.clearCommandValue();
                        state = status::IDLE;
                    }
                    else if( keyvalue > '0' && keyvalue <='9' ) {
                        command *= 10;
                        command += getNumber( keyvalue );
                    }
                    else if( keyvalue == '*' ) {
                        game_leader.startGame();
                        command = '*';
                        display_control.showCommand( command );
                    }
                    else if( keyvalue == '#' ){
                        if( command > 0 && command <= max_command ) {
                            display_control.showCommand( command );
                            state = status::SEND_COMMAND;
                        }
                        else {
                            command = 0;
                        }
                    }
                    break;
                }
                case status::SEND_COMMAND: {
                    wait( receive_clock );
                    char keyvalue = keypad.getc();
                    if( keyvalue == '#' ) {
                        game_leader.setGameLength( command );
                    }
                    else if( keyvalue == 'C' ) {
                        command = 0;
                        display_control.clearCommandValue();
                        state = status::IDLE;
                    }
                    break;
                }
            }
        }
    }
};

#endif 