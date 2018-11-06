#ifndef PAUSE_DETECTOR_HPP
#define PAUSE_DETECTOR_HPP

#include "hwlib.hpp"
#include "rtos.hpp"

#include "PauseListener.hpp"

class PauseDetector : public rtos::task<> {
private:
	rtos::clock pause_detector_clock;
	hwlib::pin_in & irp;
	PauseListener & listener;
	
	const unsigned int pause_interval = 100;
public:
	PauseDetector( const char * name, int priority, hwlib::pin_in & irp, PauseListener & listener ):
		task( priority, name ),
		pause_detector_clock( this, 100 * rtos::us, "pause_detector_clock" ),
		irp( irp ),
		listener( listener )
	{}
	
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

#endif // PAUSE_DETECTOR_HPP