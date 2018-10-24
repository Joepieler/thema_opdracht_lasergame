#ifndef MSG_LOGGER_HPP
#define MSG_LOGGER_HPP

#include "hwlib.hpp"
#include "rtos.hpp"

#include "msg_listener.hpp"

class msg_logger : public rtos::task<>, public msg_listener {
private:
	rtos::channel<uint16_t, 1024> messages;

public:
    msg_logger( const char * name ):
        task( 3, name ),
        messages( this, "messages" )
    {}

    virtual void msg_received( const uint16_t & msg ) override {
        messages.write( msg );
    };

	void main() override {
		for(;;){
			wait(messages);
			auto p = messages.read();
			hwlib::cout << p << "\n";
		}
	}
};

#endif // MSG_LOGGER_HPP