// ==========================================================================
//
// File      : ShootControl.hpp
// Copyright : bartvannetburg@hotmail.com 2018
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// ==========================================================================

// this file contains Doxygen lines
/// @file

#ifndef SHOOTCONTROL_HPP
#define SHOOTCONTROL_HPP

#include "hwlib.hpp"
#include "rtos.hpp"

#include "IRControl.hpp"
#include "PlayerData.hpp"
#include "Weapon.hpp"

/// \brief
/// This class encodes and sends the message to be sent to the IRControl class and controls the laser.
class ShootControl : public PlayerData, Weapon, rtos::task<> {
private:
    hwlib::target::pin_out laser = hwlib::target::pin_out( hwlib::target::pins::d4 );
    rtos::flag shoot_flag;
    IRControl & IR_control;
	PlayerData & player_data;
	Weapon & weapon;
	hwlib::pin_out & laser_pin;
	
	const unsigned int send_duration = 100;
	
	/// \brief
	/// This function encodes the message with the playerID and weaponID.
	/// \details
	/// The function makes the xor bits by using xor on the playerID and weaponID.
    uint16_t encode( unsigned const int player_ID, unsigned const int wapen_ID ) {
        uint16_t data = 1;
        uint8_t playerData = player_ID;
        data += ( playerData << 1 );
        uint8_t gameData = wapen_ID;
        data += ( gameData << 6 );
        uint8_t controlBits = 0;
        controlBits += ( ( ( data & 2 ) >> 1 ) ^ ( ( data & 64 ) >> 6 ) );
        controlBits += ( ( ( ( data & 4 ) >> 2 ) ^ ( ( data & 128 ) >> 7) ) << 1 );
        controlBits += ( ( ( ( data & 8 ) >> 3 ) ^ ( ( data & 256 ) >> 8 ) ) << 2 );
        controlBits += ( ( ( ( data & 16 ) >> 4 ) ^ ( ( data & 512 ) >> 9 ) ) << 3 );
        controlBits += ( ( ( ( data & 32 ) >> 5 ) ^ ( ( data & 1024 ) >> 10 ) ) << 4 );
        data += ( controlBits << 11 );
        return data;
    }
    
     
    
public:
	/// \brief
	/// This is the constructor for the shootControl.
	/// \details
	/// The constructor expects the class IRControl the enitities player_data and weapon as well as a output pin for the laser.
    ShootControl( const char * name, int priority, IRControl& IR_control, PlayerData & player_data, Weapon & weapon, hwlib::pin_out & laser_pin ):
        task( priority, name ),
        shoot_flag( this, "shoot_flag" ),
        IR_control( IR_control ),
		player_data( player_data ),
		weapon( weapon ),
		laser_pin( laser_pin )
        {}
	
	/// \brief
	/// This function sets the shoot_flag.
	/// \detils
	/// This function is used to be called by another class.
    void shoot() {
        shoot_flag.set();
    }
	
	/// \brief
	/// This function contains the state machine.
	/// \details
	/// This function contains only one state called idle.
	/// The idle state waits for a shoot flag.
	/// When the shoot flag is set the laser turns on and the function setSendData is called.
	/// After 100ms the laser is turned off and the state starts waiting for the shoot_flag again.
    void main() override {
        enum status { idle };
        status state = status::idle;
        for(;;) {
            switch( state ) {
                case status::idle:{
                    wait( shoot_flag );
                    laser_pin.set(1);
                    IR_control.setSendData( encode( player_data.getPlayerID(), weapon.getWeaponID() ) );
					hwlib::wait_ms( send_duration );
                    laser_pin.set(0);
                    break;
                }
                
            }
            
        }
    }
};

#endif //SHOOTCONTROL_HPP