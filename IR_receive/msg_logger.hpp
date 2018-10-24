#ifndef MSG_LOGGER_HPP
#define MSG_LOGGER_HPP

#include "hwlib.hpp"
#include "rtos.hpp"

#include "msg_listener.hpp"

class msg_logger : public rtos::task<>, public msg_listener {
private:
	rtos::channel<ir_msg, 1024> messages;
	hwlib::ostream & static_ostream; 
    hwlib::ostream & dynamic_ostream;
    rtos::timer display_timer; 

public:
    msg_logger( const char * name, hwlib::ostream & static_ostream, hwlib::ostream & dynamic_ostream ):
        task( 3, name ),
        messages( this, "messages" ),
        static_ostream( static_ostream ),
        dynamic_ostream( dynamic_ostream ),
        display_timer( this, "display_timer" )
    {
        show_header();
    }

    virtual void msg_received( const ir_msg & msg ) override {
        messages.write( msg );
    };

    void show_header() {
        static_ostream << "\f \t0000" << "Player: ";
        static_ostream << "\t0002" << "Weapon: ";
        static_ostream << hwlib::flush;
    }

    void show_msg( const ir_msg & msg ) {
        dynamic_ostream << "\f \t0000" << msg.player;
        dynamic_ostream << "\t0002" << msg.weapon;
        dynamic_ostream << hwlib::flush;
    }

    void clear_msg() {
        dynamic_ostream << "\f \t0000" << "none";
        dynamic_ostream << "\t0002" << "none";
        dynamic_ostream << hwlib::flush;
    }

    void main() override {
        enum states {idle, show_message};
        states state = states::idle;

		for(;;) {
			switch( state ) {
				case states::idle: {
					wait( messages );
					state = states::show_message;
					break;
				}
				
				case states::show_message: {
					show_msg( messages.read() );
					display_timer.cancel();
					display_timer.set( 5'000 * rtos::ms );
					auto event = wait( display_timer + messages );
					if( event == display_timer ) {
						clear_msg();
						state = states::idle;
					}
					else if( event == messages ) {
						display_timer.cancel();
					}
					break;
				}
			}
		}
    }
};

#endif // MSG_LOGGER_HPP