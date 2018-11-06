#ifndef SENDCONTROL_HPP
#define SENDCONTROL_HPP

#include "hwlib.hpp"
#include "rtos.hpp"
#include "IRControl.hpp"

class SendControl : public rtos::task<>{
private:
    hwlib::target::pin_out laser = hwlib::target::pin_out(hwlib::target::pins::d4);
    rtos::clock receive_clock;
    rtos::flag send_flag;
    IRControl & IR_control;
    rtos::pool<uint8_t> send_pool;
    uint8_t game_leader = 0;
    
    uint16_t encode(unsigned const int player_ID, unsigned const int wapen_ID){
        uint16_t data = 1;
        uint8_t playerData = player_ID;
        data += (playerData<<1);
        uint8_t gameData = wapen_ID;
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
    SendControl(int priority, const char * name, IRControl& IR_control):
        task(priority, name),
        receive_clock(this, 100 * rtos::us, "receive_clock"),
        send_flag(this, "send_flag"),
        IR_control(IR_control),
        send_pool("send pool")
        {}
            void send(uint8_t data){
        send_pool.write(data);
        send_flag.set();
    }

    void main() override{
        enum status {idle};
        status state = status::idle;
        for (;;){
            switch(state){
                case status::idle:{
                    //wait for butten input
                    wait(send_flag);
                    IR_control.setSendData(encode(game_leader,send_pool.read()));
                    break;
                }
            }
        }
    }
};

#endif //SENDCONTROL_HPP