#ifndef KEYBOARDCONTROL_HPP
#define KEYBOARDCONTROL_HPP

#include "hwlib.hpp"
#include "keypad.hpp"
#include "rtos.hpp"

class KeypadPlayer : public Keypad, rtos::task<>{
private:
    Keypad keypad = Keypad();
    rtos::clock receive_clock;
    
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
    KeypadPlayer(const char* name):
        task(name),
        receive_clock(this, 100 * rtos::ms, "receive_clock")
    {}
    
    void main()override{
        enum status {idle, player, wapen};
        status state = status::idle;
        for (;;){
            switch(state){
                case status::idle:{
                    wait(receive_clock);
                    char keyvalue = keypad.getc();
                    if (keyvalue == 'A'){
                        state = status::player;
                    }else if (keyvalue == 'B'){
                        state = status::wapen;
                    }
                    break;
                }
                case status::player:{
                    wait(receive_clock);
                    char keyvalue = keypad.getc();
                    if(isNumber(keyvalue)){
//                        setPlayerNumber(getNumber(keyvalue));
                        state = status::idle;
                    }
                    break;
                }
                case status::wapen:{
                    wait(receive_clock);
                    char keyvalue = keypad.getc();
                    if(isNumber(keyvalue)){
//                        setWeapon(getNumber(keyvalue));
                        state = status::idle;
                    }
                }
            }
        }
    }
};



#endif // KEYBOARDCONTROL_HPP