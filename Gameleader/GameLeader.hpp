#ifndef GAMELEADER_HPP
#define GAMELEADER_HPP

#include "hwlib.hpp"
#include "rtos.hpp"
#include "SendControl.hpp"

class GameLeader : public rtos::task<>{
private:
    rtos::flag game_length_flag;
    rtos::flag start_game_flag;
    rtos::pool<uint8_t> game_length_pool;
    SendControl & send_control;
public:
    GameLeader(int priority, const char * name, SendControl & send_control):
        task(priority, name),
        game_length_flag(this, "game length flag"),
        start_game_flag(this, "start game flag"),
        game_length_pool("game length pool"),
        send_control(send_control)
        {}
        
void setGameLength(uint8_t length){
    game_length_pool.write(length);
    game_length_flag.set();
}

void startGame(){
    start_game_flag.set();
}

void main() override{
    enum states {WAITING_FOR_COMMAND};
    states state = states::WAITING_FOR_COMMAND;
    for(;;){
        switch(state){
            case states::WAITING_FOR_COMMAND:{
                auto event = wait(game_length_flag + start_game_flag);
                if(event == game_length_flag){
                    send_control.send(game_length_pool.read());
                }
                else if(event == start_game_flag){
                    send_control.send(0);
                }
                break;
            }
        }
    }
    }
};

#endif // GAMELEADER_HPP