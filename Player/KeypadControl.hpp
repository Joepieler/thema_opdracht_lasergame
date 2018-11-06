#ifndef KEYPADCONTROL_HPP
#define KEYPADCONTROL_HPP

#include "hwlib.hpp"
#include "Keypad.hpp"
#include "rtos.hpp"

#include "DisplayControl.hpp"
#include "PlayerControl.hpp"

class KeypadControl : public Keypad, rtos::task<>{
private:
	DisplayControl & display_control;
	rtos::clock receive_clock;
	PlayerControl & player_control;
	Weapon & weapon;
	PlayerData & player_data;
    Keypad keypad = Keypad();
    
    bool isNumber(char c){
         if (c > '/' && ':' > c){
             return true;
         }
         return false;
    }
    
    int getNumber(char c){
        return (c-'0');
    }
public:
    KeypadControl(const char* name, int priority, DisplayControl & display_control, PlayerControl & player_control, Weapon & weapon, PlayerData & player_data):
        task(priority, name),
		display_control(display_control),
		receive_clock(this, 100 * rtos::ms, "receive_clock"),
		player_control(player_control),
		weapon(weapon),
		player_data(player_data)
        
		
    {}
    
    void main()override{
        enum states {idle, player, wapen};
        states state = states::idle;
        for (;;){
            switch(state){
                case states::idle:{
                    wait(receive_clock);
                    char keyvalue = keypad.getc();
                    if (keyvalue == 'A'){
                        state = states::player;
                    }else if (keyvalue == 'B'){
                        state = states::wapen;
                    }
                    break;
                }
                case states::player:{
                    wait(receive_clock);
                    char keyvalue = keypad.getc();
                    if(isNumber(keyvalue) && keyvalue != '0'){
                        player_control.setPlayerNumber(getNumber(keyvalue));
						display_control.showCommand(getNumber(keyvalue));
                        state = states::idle;
                    }
                    break;
                }
                case states::wapen:{
                    wait(receive_clock);
                    char keyvalue = keypad.getc();
                    if(isNumber(keyvalue) && keyvalue != '0'){
                        player_control.setWeapon(getNumber(keyvalue));
						display_control.showCommand(getNumber(keyvalue));
                        state = states::idle;
                    }
					break;
                }
            }
        }
    }
};



#endif // KEYBOARDCONTROL_HPP