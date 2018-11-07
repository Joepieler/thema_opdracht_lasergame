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

#ifndef KEYPADCONTROL_HPP
#define KEYPADCONTROL_HPP

#include "hwlib.hpp"
#include "Keypad.hpp"
#include "rtos.hpp"

#include "DisplayControl.hpp"
#include "PlayerControl.hpp"

/// \brief
/// This class is used to control the keypad.
class KeypadControl : public Keypad, rtos::task<> {
private:
	DisplayControl & display_control;
	rtos::clock receive_clock;
	PlayerControl & player_control;
	Weapon & weapon;
	PlayerData & player_data;
    Keypad keypad = Keypad();

/// \brief
/// This function will look if it is a number.    
    bool isNumber( char c ) {
         if( c > '/' && ':' > c ) {
             return true;
         }
         return false;
    }

/// \brief
/// This function is given a character and returns a number.
    int getNumber( char c ) {
        return ( c-'0' );
    }
public:
/// \brief
/// This is the constructor for keypad control.
/// \details
/// The constructor needs DisplayControl, PlayerControl, Weapon, PlayerData. 
    KeypadControl( const char* name, int priority, DisplayControl & display_control, PlayerControl & player_control, Weapon & weapon, PlayerData & player_data ):
        task( priority, name ),
		display_control( display_control ),
		receive_clock( this, 100 * rtos::ms, "receive_clock" ),
		player_control( player_control ),
		weapon( weapon ),
		player_data( player_data )
    {}
    
/// \brief
/// This is the state function for the keypad class.
/// \details
/// This function has 3 states: IDLE, PLAYER and WEAPON.
/// If the state is IDLE, the function waits for the receive_clock and will look for the pressed key.If the pressed key is A then the state will be PLAYER.
/// Else if the pressed key is B, the state will be WEAPON.
/// If the state is PLAYER, the function waits for the receive_clock and will look for the pressed key. If keyvalue is a number but not 0, the setPlayerNumber and ShowCommand functions will be called and the state will be IDLE.
/// If state is WEAPON, the function will wait for the receive_clock and will look for the pressed key. If keyvalue is a number but not 0, the setWeapon and showCommand functions will be called and the state will be IDLE.
    void main() override {
        enum states { IDLE, PLAYER, WEAPON };
        states state = states::IDLE;
        for(;;) {
            switch( state ) {
                case states::IDLE: {
                    wait( receive_clock );
                    char keyvalue = keypad.getc();
                    if( keyvalue == 'A' ) {
                        state = states::PLAYER;
                    }
                    else if ( keyvalue == 'B' ) {
                        state = states::WEAPON;
                    }
                    break;
                }
                case states::PLAYER:{
                    wait( receive_clock );
                    char keyvalue = keypad.getc();
                    if( isNumber(keyvalue) && keyvalue != '0' ) {
                        player_control.setPlayerNumber(getNumber(keyvalue));
                        display_control.showCommand(getNumber(keyvalue));
                        state = states::IDLE;
                    }
                    break;
                }
                case states::WEAPON:{
                    wait( receive_clock );
                    char keyvalue = keypad.getc();
                    if( isNumber( keyvalue ) && keyvalue != '0' ) {
                        player_control.setWeapon( getNumber( keyvalue ) );
                        display_control.showCommand( getNumber( keyvalue ) );
                        state = states::IDLE;
                    }
                    break;
                }
            }
        }
    }
};

#endif 