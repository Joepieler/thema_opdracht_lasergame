#ifndef IR_TRANSMIT_HPP
#define IR_TRANSMIT_HPP

#include "hwlib.hpp"
#include "rtos.hpp"

class IR_transmit : public rtos::task<>{
private:
	hwlib::target::d2_36kHz transmitter = hwlib::target::d2_36kHz();
    hwlib::target::pin_in trigger = hwlib::target::pin_in(hwlib::target::pins::d7);
    rtos::clock receive_clock;
	const int zero_pulse = 1600;
	const int one_pulse = 800;
    const int zero_pause = 800;
    const int one_pause = 1600;
    const int middel_pause = 3000;
    const int end_pause = 4000;
public:
    IR_transmit():
        task("pause_detector"),
        receive_clock(this, 100 * rtos::us, "receive_clock")
        {}
        
    uint16_t shot(const int playernumber, const int typewapen){
       uint16_t data = 1;
        uint8_t playerData = playernumber;
        data += (playerData<<1);
        uint8_t gameData = typewapen;
        data += (gameData<<6);
        uint8_t controlBits = 0;
        controlBits += ( ((data & 2)>>1) ^ ((data & 64)>>6) );
        controlBits += (( ((data & 4)>>2) ^ ((data & 128)>>7) )<<1);
        controlBits += (( ((data & 8)>>3) ^ ((data & 256)>>8) )<<2);
        controlBits += (( ((data & 16)>>4) ^ ((data & 512)>>9) )<<3);
        controlBits += (( ((data & 32)>>5) ^ ((data & 1024)>>10) )<<4);
        data += (controlBits<<11);
        return data;
    }    
    
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
        transmitter.set(1);
        if (data == false){
            hwlib::wait_us(one_pulse);
            transmitter.set(0);
            hwlib::wait_us(one_pause);
            
        }else{
            hwlib::wait_us(zero_pulse);
            transmitter.set(0);
            hwlib::wait_us(zero_pause);
        }
    }
    void set(bool b){
        transmitter.set(b);
    }
    
    void main() override{
        enum status {idle, transmit};
        status state = status::idle;
        for (;;){
            switch(state){
                case status::idle:{
                    //wait for butten input
                    if (trigger.get()){
                        state = status::transmit;
                    }
                    break;
                }
                case status::transmit:{
                   send(shot(10,9));
                   state = status::idle;
                   hwlib::wait_ms(500);
                   break;
                }
                
            }
            
        }
    }
};

#endif //IR_TRANSMIT_HPP
