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
public:
    Button(hwlib::pin_in & pin_button, ButtonListener & listener):
        pin_button(pin_button),
        listener(listener),
        receive_clock(this, 100 * rtos::ms, "receive_clock")
        {}
    
      void main() override{
        
        enum states {idle, signal};
        states state = states::idle;
        for(;;){
            switch(state){
                case states::idle:{
                    //np signal
                    wait(receive_clock);
                    if (pin_button.get()){
                        state = states::signal;
                        listener.buttonDetected();
                    }
                    break;
                }
                case states::signal:{
                    //signal
                    state = states::idle;
                    break;
                }
            }
        }
    }
};




#endif