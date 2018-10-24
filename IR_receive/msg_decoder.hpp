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

    void check(unsigned int m, unsigned int n) {
        uint8_t player = ( (m & 0b0000000000111110) >> 1 );
		uint8_t weapon = ( (m & 0b0000011111000000) >> 6 );
		uint8_t control = ( (m & 0b1111100000000000) >> 11 );
		uint8_t control2 = (player ^ weapon);
		uint8_t player2 = ( (m & 0b0000000000111110) >> 1 );
		uint8_t weapon2 = ( (m & 0b0000011111000000) >> 6 );
		uint8_t control3 = ( (m & 0b1111100000000000) >> 11 );
		uint8_t control4 = (player2 ^ weapon2);
		if(control == control2){
//			ir_msg msg = {player, control};
			listener.msg_received(m);
		}
		else if(control3 == control4){
//			ir_msg msg = {player2, control2};
			listener.msg_received(n);
		}
    }
	
	void main() override {
		enum states { idle, message };
		states state = states::idle;
		unsigned int counter;
		uint16_t msg = 0;
		uint16_t msg2 = 0;
		
		for(;;){
			switch( state ) {
				case states::idle: {
					wait( pauses );
					auto p = pauses.read();
					if( p > 4000 ) {
						counter = 0;
						msg = 0;
						state = states::message;
					}
					break;
				}
				
				case states::message: {
					wait( pauses );
					auto p = pauses.read();
					if ( p > 600 && p < 1000 ){
						msg = msg << 1;
						msg |= 1;
						counter++;
					}
					else if( p > 1000 && p < 2000 ){
						msg = msg << 1;
						msg |= 0;
						counter++;
					}
					else {
						state = states::idle;
					}
					
					if(counter == 16){
						msg2 = msg;
						wait( pauses );
						p = pauses.read();
						if(p > 2000 && p < 4000){
							msg = 0;
						}
						else {
							state = states::idle;
						}
					}
					
					if(counter == 32){
						check(msg2, msg);
						state = states::idle;
					}
					break;
				}
			}
		}
	}
};

#endif // MSG_DECODER_HPP