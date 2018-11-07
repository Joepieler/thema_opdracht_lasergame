// ==========================================================================
//
// File      : GameLogs.hpp
// Copyright : bartvannetburg@hotmail.com 2018
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// ==========================================================================

// this file contains Doxygen lines
/// @file

#ifndef GAMELOGS_HPP
#define GAMELOGS_HPP

#include "hwlib.hpp"

#include <array>

/// \brief
/// This class is used to track all the hits during the game and to print them at the end.
class GameLogs {
private:
        std::array<uint8_t, 150> player_log = {};
        std::array<const char *, 150> weapon_log = {};
        unsigned int counter = 0;
public:
		/// \brief
		/// This is function adds a log.
		/// \details
		/// The function expects a player number and a weapon name.
		/// It then adds the player number and weapon name to two seperate arrays.
		/// The counter tracks how many logs there are.
        void addLog( uint8_t player, const char *weapon ) {
            player_log[counter] = player;
            weapon_log[counter] = weapon;
            counter++;
        }
        
		/// \brief
		/// This function prints all the logs.
		/// \details
		/// The print function shows the player you have been hit by and the weapon you were hit by.
        void printLogs() {
            for( unsigned int i = 0; i < counter; i++ ) {
                hwlib::cout << "Hit by player: " << int( player_log[i] ) << " with: " << weapon_log[i] << "\n";
            }
        }
        
		/// \brief
		/// This function cleats the logs.
		/// \details
		/// Clearing the logs is done by setting the counter to 0 and not removing the values from the arrays.
        void clearLogs() {
            counter = 0;
        }
};

#endif