#ifndef GAMELOGS_HPP
#define GAMELOGS_HPP

#include "hwlib.hpp"
#include <array>

class GameLogs{
private:
        std::array<uint8_t, 150> player_log = {};
        std::array<const char *, 150> weapon_log = {};
        unsigned int counter = 0;
public:
        void addLog(uint8_t player, const char *weapon){
            player_log[counter] = player;
            weapon_log[counter] = weapon;
            counter++;
        }
        
        void printLog(){
            for(unsigned int i=0; i<counter; i++){
                hwlib::cout << "Hit by player: " << int(player_log[i]) << " with: " << weapon_log[i] << "\n";
            }
        }
        
        void clearLogs(){
            counter = 0;
        }
};

#endif //GAMELOGS_HPP