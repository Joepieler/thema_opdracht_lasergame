#ifndef IRControl_HPP
#define IRControl_HPP

#include "hwlib.hpp"


class IRControl {
private:
    hwlib::target::d2_36kHz IR_led = hwlib::target::d2_36kHz();
    const int zero_pulse = 1600;
	const int one_pulse = 800;
    const int zero_pause = 800;
    const int one_pause = 1600;
    const int middel_pause = 3000;
    const int end_pause = 4000;
public:
    
    void send(const uint16_t & data){
        uint16_t data1 = data;
        uint16_t data2 = data;
        uint16_t data3 = 32768;
        for (unsigned int i = 0; i < 16; i ++){
            pulse((data1 & data3 ) >> 15);
            data1 = data1 << 1;
        }
        pulse(false);
        hwlib::wait_us(middel_pause - zero_pulse);
        for (unsigned int i = 0; i < 16; i ++){
            pulse((data2 & data3 ) >> 15);
            data2 = data2 << 1;
        }
        pulse(false);
        hwlib::wait_us(end_pause - zero_pulse);
    }
    
    void pulse(bool data){
        IR_led.set(1);
        if (data == false){
            hwlib::wait_us(one_pulse);
            IR_led.set(0);
            hwlib::wait_us(one_pause);
            
        }else{
            hwlib::wait_us(zero_pulse);
            IR_led.set(0);
            hwlib::wait_us(zero_pause);
        }
    }
};


#endif //IRControl_HPP
