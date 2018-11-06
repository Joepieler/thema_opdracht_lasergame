#ifndef KEYPADCONTROL_HPP
#define KEYPADCONTROL_HPP

#include "hwlib.hpp"
#include "keypad.hpp"
#include "rtos.hpp"

#include "GameLeader.hpp"
#include "DisplayControl.hpp"

class KeypadControl : public Keypad, rtos::task<>{
private:
    const int max_command = 15;
    Keypad keypad = Keypad();
    rtos::clock receive_clock;
    GameLeader & game_leader;
    DisplayControl & display_control;
    
    int getNumber(char c){
        return (c-'0');
    }
public:
    KeypadControl(int priority, const char* name, GameLeader & game_leader, DisplayControl & display_control):
        task(priority, name),
        receive_clock(this, 100 * rtos::ms, "receive_clock"),
        game_leader(game_leader),
        display_control(display_control)
    {}
    
    void main()override{
        enum status {IDLE, TYPE_COMMAND, SEND_COMMAND};
        status state = status::IDLE;
        int command = 0;
        for (;;){
            switch(state){
                case status::IDLE:{
                    wait(receive_clock);
                    char keyvalue = keypad.getc();
                    if (keyvalue == 'C'){
                        state = status::TYPE_COMMAND;
                    }
                    break;
                }
                case status::TYPE_COMMAND:{
                    wait(receive_clock);
                    char keyvalue = keypad.getc();
                    if(keyvalue == 'C'){
//                        display.clear();
                        state = status::IDLE;
                    }else if (keyvalue > '0' && keyvalue <='9'){
                        command *=10;
                        command +=getNumber(keyvalue);
                    }else if(keyvalue == '*'){
                        game_leader.startGame();
                        command = '*';
                        display_control.showCommand(command);
                    }else if (keyvalue == '#'){
                        if (command > 0 && command <= max_command){
                            display_control.showCommand(command);
                            state = status::SEND_COMMAND;
                        }else{
                            command = 0;
                        }
                    }
                    break;
                }
                case status::SEND_COMMAND:{
                    wait(receive_clock);
                    
                    char keyvalue = keypad.getc();
                    if(keyvalue == '#'){
                        game_leader.setGameLength(command);
                    }else if (keyvalue == 'C'){
                        command = 0;
//                        display.clear();
                        state = status::IDLE;
                    }
                    break;
                }
            }
        }
    }
};



#endif // KEYPADCONTROL_HPP