#ifndef SHOOTCONTROL_HPP
#define SHOOTCONTROL_HPP

#include "hwlib.hpp"
#include "rtos.hpp"
#include "button_listener.hpp"
#include "IRControl.hpp"

class ShootControl : public PlayerData, Weapon, rtos::task<>{
private:
    hwlib::target::pin_out laser = hwlib::target::pin_out(hwlib::target::pins::d4);
    rtos::clock receive_clock;
    rtos::flag shoot_flag;
    IRControl & IR_control;
    
    uint16_t encode(unsigned const int player_ID, unsigned const int wapen_ID){
        uint16_t data = 1;
        uint8_t playerData = player_ID;
        data += (playerData<<1);
        uint8_t gameData = player_ID;
        data += (gameData<<6);
        uint8_t controlBits = 0;
        controlBits += ( ((data & 2)>>1) ^ ((data & 64)>>6) );
        controlBits += (( ((data & 4)>>2) ^ ((data & 128)>>7) )<<1);
        controlBits += (( ((data & 8)>>3) ^ ((data & 256)>>8) )<<2);
        controlBits += (( ((data & 16)>>4) ^ ((data & 512)>>9) )<<3);
        controlBits += (( ((data & 32)>>5) ^ ((data & 1024)>>10) )<<4);
        data += (controlBits << 11);
        return data;
    }
    
     
    
public:
    ShootControl(const char * name, IRControl& IR_control):
        task(4, name),
        receive_clock(this, 100 * rtos::us, "receive_clock"),
        shoot_flag(this, "shoot_flag"),
        IR_control(IR_control)
        {}
        
    void shoot(){
        shoot_flag.set();
    }

    void main() override{
        enum status {idle};
        status state = status::idle;
        for (;;){
            switch(state){
                case status::idle:{
                    //wait for butten input
                    wait(shoot_flag);
                    laser.set(1);
                    IR_control.setSendData(encode(PlayerData.playerID.get() ,PlayerData.wapenID.get()));
                    laser.set(0);
                    break;
                }
                
            }
            
        }
    }
};

#endif //SHOOTCONTROL_HPP
