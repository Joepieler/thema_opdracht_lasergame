// ==========================================================================
//
// File      : IRControl.hpp
// Copyright : bartvannetburg@hotmail.com 2018
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// ==========================================================================

// this file contains Doxygen lines
/// @file

#ifndef IRControl_HPP
#define IRControl_HPP

#include "hwlib.hpp"
#include "rtos.hpp"

/// \brief
/// This class is used to control the IR signal.
class IRControl : public rtos::task<> {
private:
    hwlib::target::d2_36kHz IR_led = hwlib::target::d2_36kHz();
    rtos::channel< uint16_t, 1024 > send_data_channel;

    const int zero_pulse = 1600;
    const int one_pulse = 800;
    const int zero_pause = 800;
    const int one_pause = 1600;
    const int middel_pause = 3000;
    const int end_pause = 4000;
    
    const int ir_on = 1;
    const int ir_off = 0;
    
/// \brief
/// This function sends the data.
/// \details
/// The function looks if the data is a 1 or a 0 and with that information he calls the pulse function.
    void send( const uint16_t & data ) {
        uint16_t data1 = data;
        uint16_t data2 = data;
        uint16_t data3 = 32768;
        for( unsigned int i = 0; i < 16; i ++ ) {
            pulse( ( data1 & data3 ) >> 15 );
            data1 = data1 << 1;
        }
        pulse( false );
        hwlib::wait_us( middel_pause - zero_pulse );
        for ( unsigned int i = 0; i < 16; i ++ ) {
            pulse( ( data2 & data3 ) >> 15 );
            data2 = data2 << 1;
        }
        pulse( false );
        hwlib::wait_us( end_pause - zero_pulse );
    }
    
/// \brief
/// This function makes a pulse.
/// \details
/// The function makes a pulse based on if it is a 1 or a 0.
    void pulse( bool data ) {
        IR_led.set( ir_on );
        if( data == false ) {
            hwlib::wait_us( one_pulse );
            IR_led.set( ir_off );
            hwlib::wait_us(one_pause);
            
        }
        else {
            hwlib::wait_us( zero_pulse );
            IR_led.set( ir_off );
            hwlib::wait_us( zero_pause );
        }
    }
public:
/// \brief
/// This is the constructor for IR control.
    IRControl( const char * name, int priority ):
    task( priority, name ),
    send_data_channel( this, "send_data_channel" )
    {}
    
/// \brief
/// This function writes the given data in the send_data_channel.
    void setSendData( uint16_t data ) {
		send_data_channel.write( data );
	}

/// \brief
/// This is the state function for the IR control class.
/// \details
/// This function has one state: WAIT_FOR_DATA.
/// The function waits for the send_data_channel en then he will send the data from the send_data_channel.
    void main() override {
        enum status { WAIT_FOR_DATA };
        status state = status::WAIT_FOR_DATA;
        for(;;) {
            switch( state ) {
                case status::WAIT_FOR_DATA: {
                    wait( send_data_channel );
                    send( send_data_channel.read() );
                    break;
                }
            }
        }
    }
};

#endif 