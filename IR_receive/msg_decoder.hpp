#ifndef MSG_DECODER_HPP
#define MSG_DECODER_HPP

#include "hwlib.hpp"
#include "rtos.hpp"

#include "pause_listener.hpp"
#include "msg_listener.hpp"

#include <array>

class msg_decoder : public rtos::task<>, public pause_listener {
private:
	rtos::channel<unsigned int, 1024> pauses;
	msg_listener & listener;
public:
	msg_decoder( const char * name, msg_listener & listener):
		task( 2, name ),
		pauses( this, "pauses" ),
		listener( listener )

	{}
	
	virtual void pause_detected( int pause_length ) override {
		pauses.write( pause_length );
	}
	
	void printByte(uint8_t byte) {
	    for(unsigned int i = 0; i < 8; ++i) {
		    hwlib::cout << ((byte >> (7 - i)) & 1) << ' ';
	    }
	    hwlib::cout << "\n";
    }
	
	void printBytes(uint16_t byte) {
	    for(unsigned int i = 0; i < 16; ++i) {
		    hwlib::cout << ((byte >> (15 - i)) & 1) << ' ';
	    }
	    hwlib::cout << "\n";
    }

    void check(unsigned int m) {
        uint8_t player = ( (m & 0b0000000000111110) >> 1 );
		uint8_t weapon = ( (m & 0b0000011111000000) >> 6 );
		uint8_t control = ( (m & 0b1111100000000000) >> 11 );
//		printByte(player);
//		printByte(weapon);
		uint8_t control2 = (player ^ weapon);
//		printByte(control);
//		printByte(control2);
		printBytes(m);
		ir_msg msg = { player, weapon };
		if(control == control2){
			listener.msg_received(msg);
		}
    }
	
	void main() override {
		enum states { idle, message };
		states state = states::idle;
		unsigned int counter;
		uint16_t msg = 0;
		uint16_t redundant_msg = 0;
		
		for(;;){
			switch( state ) {
				case states::idle: {
					wait( pauses );
					auto p = pauses.read();
					if( p > 4000 ) {
						counter = 0;
						msg = 0;
						redundant_msg = 0;
						state = states::message;
					}
					break;
				}
				
				case states::message: {
					wait( pauses );
					auto p = pauses.read();
					if( p > 600 && p < 4000 ){
						if ( p > 600 && p < 1000 ){
							if(counter < 16){
								msg |= 1;
							}
							else {
								redundant_msg |= 1;
							}
						}
						else if( p > 1400 && p < 2000 ){
							if(counter < 16){
								msg |= 0;
							}
							else {
								redundant_msg |= 0;
							}
						}
					}
					else {
						state = states::idle;
					}
					if(counter < 15){
						msg = msg << 1;
					}
					else {
						redundant_msg = redundant_msg << 1;
					}
					counter++;
					
					if( counter == 31 ){
						
						msg |= 1;
						redundant_msg |= 1;
						printBytes(msg);
						printBytes(redundant_msg);
						state = states::idle;
					}
					break;
				}
			}
		}
	}
};

#endif // MSG_DECODER_HPP