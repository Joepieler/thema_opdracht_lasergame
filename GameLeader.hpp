#ifndef GAMELEADER_HPP
#define GAMELEADER_HPP

#include "hwlib.hpp"
#include "rtos.hpp"

class GameLeader : public rtos::task<>{
private:
    rtos::flag game_length_flag;
    rtos::flag start_game_flag;
    rtos::pool<int> game_length_pool;
public:
    GameLeader():
        task("GameLeader"),
        game_length_flag(this, "game length flag"),
        start_game_flag(this, "start game flag"),
        game_length_pool("game length pool")
        {}
        
void setGameLength(unsigned int length){
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
                    send_control.send(0, game_length_pool.read());
                }
                else if(event == start_game_flag){
                    send_control.send(0, 0);
                }
                break;
            }
        }
    }
    }
};

#endif // GAMELEADER_HPP