#ifndef KEYBOARDCONTROL_HPP
#define KEYBOARDCONTROL_HPP

#include "hwlib.hpp"
#include "keypad.hpp"
#include "rtos.hpp"

class KeypadGameleader : public Keypad, rtos::task<>{
private:
    const int max_command = 15;
    Keypad keypad = Keypad();
    rtos::clock receive_clock;
    
    int getNumber(char c){
        return (c-'0');
    }
public:
    KeypadGameleader(const char* name):
        task(name),
        receive_clock(this, 100 * rtos::ms, "receive_clock")
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
                        setStartGame();
                        command = '*';
//                        displayCommand(command);
                    }else if (keyvalue == '#'){
                        if (command > 0 && command <= max_command){
                            state = status::SEND_COMMAND;
                        }else{
                            command = 0;
                        }
                    }
                    break;
                }
                case status::SEND_COMMAND:{
                    wait(receive_clock);
//                    display.displayCommand(command);
                    char keyvalue = keypad.getc();
                    if(keyvalue == '#'){
                        setGameLength(command);
                    }else if (keyvalue == '*'){
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



#endif // KEYBOARDCONTROL_HPP