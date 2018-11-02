#ifndef SHOOTCONTROL_HPP
#define SHOOTCONTROL_HPP

#include "hwlib.hpp"
#include "rtos.hpp"
#include "button_listener.hpp"
#include "IRControl.hpp"

class ShootControl : public ButtonListener, IRControl, rtos::task<>{
private:
    hwlib::target::pin_out laser = hwlib::target::pin_out(hwlib::target::pins::d4);
    rtos::clock receive_clock;
    rtos::flag triggerFlag;
    IRControl IR_control;
public:
    ShootControl(IRControl & IR_control):
        task("ShootControl"),
        receive_clock(this, 100 * rtos::us, "receive_clock"),
        triggerFlag(this, "trigger"),
        IR_control(IR_control)        
        {}
        
    virtual void buttonDetected() override{
        triggerFlag.set();
    }
        
        
    void shoot(const int playernumber, const int typewapen){
        laser.set(1);
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
        send(data);
        laser.set(0);
    }    
    

    
    
    void main() override{
        enum status {idle, transmit};
        status state = status::idle;
        for (;;){
            switch(state){
                case status::idle:{
                    //wait for butten input
                    wait(triggerFlag);
                    state = status::transmit;
                    break;
                }
                case status::transmit:{
                    shoot(1,2);
                    hwlib::wait_ms(3000);
                    state = status::idle;
                   break;
                }
                
            }
            
        }
    }
};

#endif //SHOOTCONTROL_HPP