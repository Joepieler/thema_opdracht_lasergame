// ==========================================================================
//
// File      : PauseDetector.hpp
// Copyright : bartvannetburg@hotmail.com 2018
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// ==========================================================================

// this file contains Doxygen lines
/// @file

#ifndef PAUSE_DETECTOR_HPP
#define PAUSE_DETECTOR_HPP

#include "hwlib.hpp"
#include "rtos.hpp"

#include "PauseListener.hpp"

/// \brief
/// This class detects the pauses in a IR signal and sends them to another class.
class PauseDetector : public rtos::task<> {
private:
	rtos::clock pause_detector_clock;
	hwlib::pin_in & irp;
	PauseListener & listener;
	
	const unsigned int pause_interval = 100;
public:
	/// \brief
	/// This is the constructor for the pauseDetector.
	/// \details
	/// The constructor expects a listener to send the detected messages to.
	PauseDetector( const char * name, int priority, hwlib::pin_in & irp, PauseListener & listener ):
		task( priority, name ),
		pause_detector_clock( this, 100 * rtos::us, "pause_detector_clock" ),
		irp( irp ),
		listener( listener )
	{}
	
	/// \brief
	/// This function contains the state machine.
	/// \details
	/// The state machine consists of two states.
	/// The idle state waits every 100 us and checks if a pause is detected.
	/// If the pause ends it gets sent to the listener class and switches to the signal state.
	/// When a new pause starts the state switches to idle.
	void main() override {
		enum states { idle, signal };
		states state = states::idle;
		unsigned int pause_length = 0;
		
		for(;;){
			switch( state ) {
				case states::idle: {
					hwlib::wait_us( pause_interval );
					if( irp.get() ) {
						pause_length += pause_interval;
					}
					else{
						listener.pauseDetected( pause_length );
						state = states::signal;
					}
					break;
				}
				
				case states::signal: {
					hwlib::wait_us( pause_interval );
					if( irp.get() ) {
						pause_length = 0;
						state = states::idle;
					}
					break;
				}
			}
		}
	}
};

#endif