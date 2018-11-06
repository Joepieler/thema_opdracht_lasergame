#ifndef SHOOTCONTROL_HPP
#define SHOOTCONTROL_HPP

#include "hwlib.hpp"
#include "rtos.hpp"

#include "IRControl.hpp"
#include "PlayerData.hpp"
#include "Weapon.hpp"

class ShootControl : public PlayerData, Weapon, rtos::task<>{
private:
    hwlib::target::pin_out laser = hwlib::target::pin_out(hwlib::target::pins::d4);
    rtos::flag shoot_flag;
    IRControl & IR_control;
	PlayerData & player_data;
	Weapon & weapon;
    
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
    ShootControl(const char * name, int priority, IRControl& IR_control, PlayerData & player_data, Weapon & weapon):
        task(priority, name),
        shoot_flag(this, "shoot_flag"),
        IR_control(IR_control),
		player_data(player_data),
		weapon(weapon)
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
                    //laser.set(1);
                    IR_control.setSendData(encode(player_data.getPlayerID(), weapon.getWeaponID()));
                    //laser.set(0);
                    break;
                }
                
            }
            
        }
    }
};

#endif //SHOOTCONTROL_HPP