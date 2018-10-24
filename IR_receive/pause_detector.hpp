#ifndef PAUSE_DETECTOR_HPP
#define PAUSE_DETECTOR_HPP

#include "hwlib.hpp"
#include "rtos.hpp"

#include "pause_listener.hpp"

class pause_detector : public rtos::task<> {
private:
	rtos::clock pause_detector_clock;
	hwlib::pin_in & irp;
	pause_listener & listener;
public:
	pause_detector( const char * name, hwlib::pin_in & irp, pause_listener & listener ):
		task( 1, name ),
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
					wait( pause_detector_clock );
					if( irp.get() ) {
						pause_length += 100;
					}
					else{
						listener.pause_detected( pause_length );
//						hwlib::cout << pause_length << "\n";
						state = states::signal;
					}
					break;
				}
				
				case states::signal: {
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