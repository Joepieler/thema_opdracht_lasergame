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
	rtos::flag display_countdown_flag;
	rtos::flag display_deaths_flag;
	rtos::flag display_ammo_flag;
	rtos::flag display_health_flag; 
	rtos::flag display_player_flag; 
	rtos::flag display_weapon_flag;
        
    rtos::pool<uint8_t> command_pool;
	rtos::pool<uint8_t> countdown_pool;
	rtos::pool<uint8_t> deaths_pool;
	rtos::pool<uint8_t> ammo_pool;
	rtos::pool<uint8_t> health_pool; 
	rtos::pool<uint8_t> player_pool;
    rtos::pool<const char *> weapon_pool;
    
    const uint8_t display_size_y = 64;
    const uint8_t display_size_x = 128;
    const uint8_t devider = 80;
	const uint8_t font_height = 8;
	const uint8_t font_width = 8;
	const int display_delay = 100;
	
	hwlib::font_default_8x8 font = hwlib::font_default_8x8();

    hwlib::glcd_oled & display;

    
    hwlib::window_part value_names = hwlib::window_part( display, hwlib::location( 0, 0 ), hwlib::location( devider, display_size_y ) );	
    hwlib::window_ostream value_names_ostream = hwlib::window_ostream( value_names, font );
    
    hwlib::window_part ammo_value = hwlib::window_part( display, hwlib::location( devider, 0 ), hwlib::location( display_size_x - ( font_width * 6 ), font_height ) );
    hwlib::window_ostream ammo_value_ostream = hwlib::window_ostream( ammo_value, font );

    hwlib::window_part health_value = hwlib::window_part( display, hwlib::location( devider, font_height ), hwlib::location( display_size_x - ( font_width * 5 ), font_height * 2 ) );
    hwlib::window_ostream health_value_ostream = hwlib::window_ostream( health_value, font );
    
    hwlib::window_part weapon_value = hwlib::window_part( display, hwlib::location( devider, font_height * 2 ), hwlib::location( display_size_x, font_height * 3 ) );
    hwlib::window_ostream weapon_value_ostream = hwlib::window_ostream( weapon_value, font );
    
    hwlib::window_part player_value = hwlib::window_part( display, hwlib::location( devider, font_height * 3 ), hwlib::location( display_size_x - ( font_width * 6 ), font_height * 4 ) );
    hwlib::window_ostream player_value_ostream = hwlib::window_ostream( player_value, font );
    
    hwlib::window_part deaths_value = hwlib::window_part( display, hwlib::location( devider, font_height * 4 ), hwlib::location( display_size_x - ( font_width * 6 ), font_height * 5 ) );
    hwlib::window_ostream deaths_value_ostream = hwlib::window_ostream( deaths_value, font );
    
    hwlib::window_part countdown_value = hwlib::window_part( display, hwlib::location( devider, font_height * 5 ), hwlib::location( display_size_x - ( font_width *  7), font_height * 6 ) );
	hwlib::window_ostream countdown_value_ostream = hwlib::window_ostream( countdown_value, font );
    
    hwlib::window_part command_value = hwlib::window_part( display, hwlib::location( devider, font_height * 6 ), hwlib::location( display_size_x - ( font_width * 7 ), font_height * 7 ) );
    hwlib::window_ostream command_value_ostream = hwlib::window_ostream( command_value, font );
public:
/// \brief
/// This is the constructor for a display.
/// \details
/// The constructor expects a display.
    DisplayControl( const char * name, int priority, hwlib::glcd_oled & display ):
		task( priority, name ),
        display_command_flag( this, "display command flag" ),
        display_countdown_flag( this, "display countdown flag" ), 
        display_deaths_flag( this, "display deaths flag" ), 
        display_ammo_flag( this, "display ammo flag" ), 
        display_health_flag( this, "display health flag" ), 
        display_player_flag( this, "display player flag" ), 
        display_weapon_flag( this, "display weapon flag" ),
        command_pool( "display command pool" ),
        countdown_pool( "display countdown pool" ), 
        deaths_pool( "display deaths pool" ), 
        ammo_pool( "display ammo pool" ), 
        health_pool( "display health pool" ), 
        player_pool( "display player pool" ), 
        weapon_pool( "display weapon pool" ),
        display(display)
	{
        display.clear();
        value_names_ostream 
        << "\t0000Ammo:"
        << "\t0001Health:"
        << "\t0002Weapon:"
        << "\t0003Player:"
        << "\t0004Deaths:"
        << "\t0005Countdown:"
        << "\t0006Command:"
        << hwlib::flush;

    }

/// \brief
/// This function sets a flag en writes the value in the command_pool.
    void showCommand( uint8_t value ) {
        display_command_flag.set();
        command_pool.write( value );
    }
    
/// \brief
/// This function sets a flag en writes the value in the countdown_pool.
    void showCountdown( uint8_t value ) {
        display_countdown_flag.set();
        countdown_pool.write( value );
    }
    
/// \brief
/// This function sets a flag en writes the value in the deaths_pool.
    void showDeaths(uint8_t value ){
        display_deaths_flag.set();
        deaths_pool.write( value );
    }
    
/// \brief
/// This function sets a flag en writes the value in the ammo_pool.
    void showAmmo( uint8_t value ) {
        display_ammo_flag.set();       
        ammo_pool.write( value );
    }
    
/// \brief
/// This function sets a flag en writes the value in the health_pool.
    void showHealth( uint8_t value ) {
        display_health_flag.set();
        health_pool.write( value );
    }
    
/// \brief
/// This function sets a flag en writes the value in the player_pool.
    void showPlayer( uint8_t value ) {
        display_player_flag.set();
        player_pool.write( value );
    }
    
/// \brief
/// This function sets a flag en writes the value in the weapon_pool.
    void showWeapon( const char * value ) {
        display_weapon_flag.set();
        weapon_pool.write( value );
    }

/// \brief
/// This function updates the command value on the screen.
    void updateCommandValue( uint8_t value ) {
        command_value_ostream 
            << "\t0000"
            << (int)value
            << hwlib::flush;
    }
    
/// \brief
/// This function updates the countdown value on the screen.
    void updateCountdownValue( uint8_t value ) {
        countdown_value_ostream 
            << "\t0000"
            << (int)value
            << hwlib::flush;
    }
    
/// \brief
/// This function updates the deaths value on the screen.
    void updateDeathsValue( uint8_t value ) {
        deaths_value_ostream
			<< "\t0000"
            << "      "
            << hwlib::flush;
        deaths_value_ostream
            << "\t0000"
            << (int)value
            << hwlib::flush;
    } 
	
/// \brief
/// This function updates the ammo value on the screen.
    void updateAmmoValue( uint8_t value ) {
        ammo_value_ostream
			<< "\t0000"
            << "      "
            << hwlib::flush;
        ammo_value_ostream
            << "\t0000"
            << (int)value
            << hwlib::flush;
    } 
	
/// \brief
/// This function updates the health value on the screen.
    void updateHealtValue( uint8_t value ) {
        health_value_ostream
			<< "\t0000"
            << "      "
            << hwlib::flush;
        health_value_ostream
            << "\t0000"
            << (int)value
            << hwlib::flush;
    } 
	
/// \brief
/// This function updates the player value on the screen.
    void updatePlayerValue( uint8_t value ) {
        player_value_ostream
            << "\t0000"
            << (int)value
            << hwlib::flush;
    }
	
/// \brief
/// This function updates the weapon value on the screen.
    void updateWeaponValue( const char * value  ){
        weapon_value_ostream
            << "\t0000"
            << "      "
            << hwlib::flush;
        weapon_value_ostream
            << "\t0000"
            << value
            << hwlib::flush;
    }

/// \brief
/// This is the state function for the display class.
/// \details
/// This function has one state: WAIT_FOR_MESSAGE.
/// If event is the same as display_command_flag, the function updateCommandValue with as parameter the readen comman_pool value.
/// If event is the same as display_countdown_flag, the function updateCountdownValue with as parameter the readen countdown_pool value.
/// If event is the same as display_deaths_flag, the function updateDeathsValue with as parameter the readen deaths_pool value.
/// If event is the same as display_ammo_flag, the function updateAmmoValue with as parameter the readen ammo_pool value.
/// If event is the same as display_health_flag, the function updateHealthValue with as parameter the readen health_pool value.
/// If event is the same as display_player_flag, the function updatePlayerValue with as parameter the readen player_pool value.
/// If event is the same as display_weapon_flag, the function updateWeaponValue with as parameter the readen Weapon_pool value.
    void main() override {
		enum states { WAIT_FOR_MESSAGE };
		states state = states::WAIT_FOR_MESSAGE;
        for(;;) {
			switch( state ) {
                case states::WAIT_FOR_MESSAGE: {
                    auto event = wait(
                        display_command_flag + 
                        display_countdown_flag + 
                        display_deaths_flag + 
                        display_ammo_flag + 
                        display_health_flag + 
                        display_player_flag + 
                        display_weapon_flag
                    );
                    
                    if( event == display_command_flag ) {
                        updateCommandValue( command_pool.read() );
                    } 
					else if( event == display_countdown_flag ) {
                        updateCountdownValue( countdown_pool.read() );
                    } 
					else if( event == display_deaths_flag ) {
						hwlib::wait_ms( display_delay );
                        updateDeathsValue( deaths_pool.read() );
                    } 
					else if( event == display_ammo_flag) {
						hwlib::wait_ms( display_delay );
                        updateAmmoValue( ammo_pool.read() );
                        
                    } 
					else if( event == display_health_flag ) {
						hwlib::wait_ms( display_delay );
                        updateHealtValue( health_pool.read() );
                    } 
					else if( event == display_player_flag ) {
                        updatePlayerValue( player_pool.read() );
                    } 
					else if( event == display_weapon_flag ) {
                        updateWeaponValue( weapon_pool.read() );
                    }
				break;
                }
            }
        }
    }
};

#endif