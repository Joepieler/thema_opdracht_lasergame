#ifndef BUTTON_HPP
#define BUTTON_HPP

#include "hwlib.hpp"
#include "rtos.hpp"
#include "ButtonListener.hpp"


class Button  :public rtos::task<> {
private:
    hwlib::pin_in & pin_button;
    ButtonListener& listener;
    rtos::clock receive_clock;
    unsigned int buttonnumber;
public:
    Button( const char * name, int priority, hwlib::pin_in & pin_button, ButtonListener & listener, unsigned int buttonnumber ):
	task( priority, name ),
        pin_button( pin_button ),
        listener( listener ),
        receive_clock( this, 100 * rtos::ms, "receive_clock" ),
        buttonnumber( buttonnumber )
        {}
    
      void main() override {
        
        enum states { WAIT_FOR_BUTTON_PRESS };
        states state = states::WAIT_FOR_BUTTON_PRESS;
        for(;;) {
            switch( state ) {
                case states::WAIT_FOR_BUTTON_PRESS: {
                    wait( receive_clock );
                    if ( pin_button.get() ) {
                        listener.buttonPressed( buttonnumber );
                    }
                    break;
                }
            }
        }
    }
};

#endif
