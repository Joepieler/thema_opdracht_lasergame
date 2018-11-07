// ==========================================================================
//
// File      : MsgDecoder.hpp
// Copyright : bartvannetburg@hotmail.com 2018
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// ==========================================================================

// this file contains Doxygen lines
/// @file

#ifndef MSG_DECODER_HPP
#define MSG_DECODER_HPP

#include "hwlib.hpp"
#include "rtos.hpp"

#include "PauseListener.hpp"
#include "MsgListener.hpp"

/// \brief
/// This class decodes the incoming message and sends it to another class.
class MsgDecoder : public rtos::task<>, public PauseListener {
private:
	rtos::channel<unsigned int, 1024> pauses;
	MsgListener & listener;
	
	const uint16_t player_mask = 0b0000000000111110;
	const uint16_t weapon_mask = 0b0000011111000000;
	const uint16_t control_mask = 0b1111100000000000;
	
	const unsigned int min_initial_pause = 2000;
	const unsigned int max_initial_pause = 4000;
	const unsigned int min_zero_bit = 1200;
	const unsigned int max_zero_bit = 2000;
	const unsigned int min_one_bit = 400;
	const unsigned int max_one_bit = 1200;
	const unsigned int max_bits = 16;
	
public:
	/// \brief
	/// This is the constructor for the messagedecoder.
	/// \details
	/// The constructor expects a listener to send the decoded message to.
	MsgDecoder( const char * name, int priority, MsgListener & listener ):
		task( priority, name ),
		pauses( this, "pauses" ),
		listener( listener )
	{}
	
	/// \brief
	/// This function overwrites the virtual pauseDetected function.
	/// \details
	/// This function writes the parameter pause_length into the channel pauses.
	virtual void pauseDetected( int pause_length ) override {
		pauses.write( pause_length );
	}
	/// \brief
	/// This function checks if the message is valid.
	/// \details
	/// This function compares the xor bits with the with xor of the player and weapon bits.
	/// The function returns true if the message is valid and false if it is not.
	bool check( unsigned int m ) {
		uint8_t player = ( (m & player_mask ) >> 1 );
		uint8_t weapon = ( (m & weapon_mask ) >> 6 );
		uint8_t control = ( (m & control_mask ) >> 11 );
		uint8_t control2 = ( player ^ weapon );
		if( control == control2 ) {
			return true;
		}
		return false;
	}
	
	/// \brief
	/// This function contains the state machine.
	/// \details
	/// The functions contains two states.
	/// In the idle state it waits for a pause and checks if the pause is valid.
	/// In the message state it checks if the following pause is valid and inserts a 1 or a 0 in the message respectively.
	/// If 16 valid pauses are detected the message gets sent with the messageDetected function and it returns to the idle state.
	void main() override {
		enum states { idle, message };
		states state = states::idle;
		unsigned int counter = 0;
		unsigned int initial_pause = 0;
		uint16_t msg = 0;
		uint16_t previous_msg = 0;
		
		for(;;) {
			switch( state ) {
				case states::idle: {
					wait( pauses );
					initial_pause = pauses.read();
					if( initial_pause > min_initial_pause ) {
						counter = 0;
						msg = 0;
						state = states::message;
					}
					break;
				}
				
				case states::message: {
					wait( pauses );
					auto p = pauses.read();
					if ( p > min_one_bit && p < max_one_bit ) {
						msg = msg << 1;
						msg |= 1;
						counter++;
					}
					else if( p > min_zero_bit && p < max_zero_bit ) {
						msg = msg << 1;
						msg |= 0;
						counter++;
					}
					else {
						state = states::idle;
					}
					
					if( counter == max_bits ) {
						if(check(msg)){
							if( initial_pause > min_initial_pause && initial_pause < max_initial_pause && msg == previous_msg ) {
								previous_msg = 0;
								initial_pause = 0;
								state = states::idle;
							}
							else {
								previous_msg = msg;
								initial_pause = 0;
								uint8_t player = ( (msg & player_mask) >> 1 );
								uint8_t weapon = ( (msg & weapon_mask) >> 6 );
								ir_msg msg = { player, weapon };
								listener.msgReceived( msg );
								state = states::idle;
							}
						}
						else{
							state = states::idle;
						}
					}
					break;
				}
			}
		}
	}
};

#endif