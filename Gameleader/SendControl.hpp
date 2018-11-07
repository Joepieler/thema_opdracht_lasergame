// ==========================================================================
//
// File      : sendControl.hpp
// Copyright : bartvannetburg@hotmail.com 2018
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// ==========================================================================

// this file contains Doxygen lines
/// @file

#ifndef SEND_CONTROL_HPP
#define SEND_CONTROL_HPP

#include "hwlib.hpp"
#include "rtos.hpp"

#include "IRControl.hpp"

/// \brief
/// This class is used to send the data.
class SendControl : public rtos::task<> {
private:
    hwlib::target::pin_out laser = hwlib::target::pin_out( hwlib::target::pins::d4 );
    rtos::clock receive_clock;
    rtos::flag send_flag;
    IRControl & IR_control;
    uint8_t game_leader = 0;
    rtos::pool<uint8_t> send_pool;
    
/// \brief
/// This function can encode.
/// \details
/// The function wants a player_ID and a weapon_ID. The function will make a message of 16 bits and sets the player_ID and weapon_ID on the right place in the message. He also makes the XOR's on the right place.
    uint16_t encode( unsigned const int player_ID, unsigned const int weapon_ID ) {
        uint16_t data = 1;
        uint8_t playerData = player_ID;
        data += ( playerData << 1 );
        uint8_t gameData = weapon_ID;
        data += ( gameData << 6 );
        uint8_t controlBits = 0;
        controlBits += (  ((data & 2)>>1) ^ ((data & 64)>>6) );
        controlBits += (( ((data & 4)>>2) ^ ((data & 128)>>7) )<<1);
        controlBits += (( ((data & 8)>>3) ^ ((data & 256)>>8) )<<2);
        controlBits += (( ((data & 16)>>4) ^ ((data & 512)>>9) )<<3);
        controlBits += (( ((data & 32)>>5) ^ ((data & 1024)>>10) )<<4);
        data += ( controlBits << 11 );
        return data;
    }
public:
/// \brief
/// This is the constructer of send control.
/// \details
/// The constructer wants IR control.
    SendControl( const char * name, int priority, IRControl & IR_control):
        task( priority, name ),
        receive_clock( this, 100 * rtos::us, "receive_clock" ),
        send_flag( this, "send_flag" ),
        IR_control( IR_control ),
        send_pool( "send pool" )
        {}
     
/// \brief
/// This function writes the data in the send_pool and sets the send_flag.   
    void send( uint8_t data ) {
        send_pool.write( data );
        send_flag.set();
    }

/// \brief
/// This is the state function for send control.
/// \details
/// This function has 1 state: IDLE. The function waits for the send_flag and the call the setSendData with as parameter an encoded game_leader with send_pool readed data.
    void main() override {
        enum status { IDLE };
        status state = status::IDLE;
        for (;;) {
            switch( state ) {
                case status::IDLE: {
                    //wait for butten input
                    wait( send_flag );
                    IR_control.setSendData( encode( game_leader, send_pool.read() ) );
                    break;
                }
            }
        }
    }
};

#endif