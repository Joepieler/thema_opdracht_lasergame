#ifndef BUTTON_HPP
#define BUTTON_HPP

#include "hwlib.hpp"
#include "rtos.hpp"
#include "button_listener.hpp"


class Button  :public rtos::task<>{
private:
    hwlib::pin_in & pin_button;
    ButtonListener& listener;
    rtos::clock receive_clock;
    const char* buttonname;
public:
    Button(hwlib::pin_in & pin_button, ButtonListener & listener, const char* buttonname):
        pin_button(pin_button),
        listener(listener),
        receive_clock(this, 100 * rtos::ms, "receive_clock"),
        buttonname(buttonname)
        {}
    
      void main() override{
        
        enum states {WAIT_FOR_BUTTON_PRESS};
        states state = states::WAIT_FOR_BUTTON_PRESS;
        for(;;){
            switch(state){
                case states::WAIT_FOR_BUTTON_PRESS:{
                    //np signal
                    wait(receive_clock);
                    if (pin_button.get()){
                        listener.buttonPressed(buttonname);
                    }
                    break;
                }
            }
        }
    }
};




#endif
