#ifndef BUZZERCONTROL_HPP
#define BUZZERCONTROL_HPP

#include "hwlib.hpp"
#include "rtos.hpp"

class BuzzerControl : public rtos::task<>{
private:
    hwlib::pin_out & buzz_pin;
    rtos::flag buzz_on_flag;
    rtos::timer buzz_length_timer;
    rtos::timer buzz_interval_timer;
    rtos::pool<int> buzz_pool;
    int buzz_length = 0;
    int buzz_interval = 100;
public:
    BuzzerControl(hwlib::pin_out & buzz_pin):
        task("BuzzerControl"),
        buzz_pin(buzz_pin),
        buzz_on_flag(this, "buzz on flag"),
        buzz_length_timer(this, "buzz length timer"),
        buzz_interval_timer(this, "buzz interval timer"),
        buzz_pool("buzz pool")
        {}
        
        void buzzFlag(){
            buzz_on_flag.set();
        }
        
        void makeSound(int buzz_length){
            buzz_pool.write(buzz_length);
            buzz_on_flag.set();
        }
        
        void main() override{
            enum states {IDLE, ON, OFF};
            states state = states::IDLE;
            for(;;){
                switch(state){
                    case states::IDLE:{
                        wait(buzz_on_flag);
                        buzz_length = buzz_pool.read();
                        buzz_length_timer.set(buzz_length * rtos::us);
                        state = states::ON;
                        break;
                    case states::ON:{
                        buzz_pin.set(1);
                        buzz_interval_timer.set(buzz_interval * rtos::us);
                        wait(buzz_interval_timer);
                        state = states::OFF;
                        break;
                    case states::OFF:{
                        buzz_pin.set(0);
                        buzz_interval_timer.set(buzz_interval * rtos::us);
                        auto event = wait(buzz_length_timer + buzz_interval_timer);
                        if(event == buzz_length_timer){
                            state = states::IDLE;
                        }
                        else if(event == buzz_interval_timer){
                            state = states::ON;
                        }
                        break;
                    }
                    }
                    }
                }
            }
        }
};

#endif //BUZZERCONTROL_HPP
    
    