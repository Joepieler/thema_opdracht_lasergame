// ==========================================================================
//
// File      : GameLeader.hpp
// Copyright : bartvannetburg@hotmail.com 2018
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// ==========================================================================

// this file contains Doxygen lines
/// @file

#ifndef GAME_LEADER_HPP
#define GAME_LEADER_HPP

#include "hwlib.hpp"
#include "rtos.hpp"

#include "SendControl.hpp"

/// \brief
/// This class is used to make the game.
class GameLeader : public rtos::task<> {
private:
    rtos::flag game_length_flag;
    rtos::flag start_game_flag;
    
    rtos::pool<uint8_t> game_length_pool;
    
    SendControl & send_control;
    
    int start_game_value = 0;
public:
/// \brief
/// This is the constructor for the GameLeader.
/// \details
/// The constructor expects a SendControl as parameter.
    GameLeader( const char * name, int priority, SendControl & send_control ):
        task( priority, name ),
        game_length_flag( this, "game length flag" ),
        start_game_flag( this, "start game flag" ),
        game_length_pool( "game length pool" ),
        send_control( send_control )
        {}
        
/// \brief
/// This function writes the given length in the game_length_pool and sets the game_length_flag.
    void setGameLength( uint8_t length ) {
        game_length_pool.write( length );
        game_length_flag.set();
    }

/// \brief
/// This function sets the start_game_flag.
    void startGame() {
        start_game_flag.set();  
    }

/// \brief
/// This is the state function for the GameLeader class.
/// \details
/// This function has one state: WAITING_FOR_COMMAND.
/// The function waits for game_length_flag and then he will send the value in the game_length_pool.
/// Else he will wait for the start_game_flag en then he will send the start_game_value.
    void main() override {
        enum states { WAITING_FOR_COMMAND };
        states state = states::WAITING_FOR_COMMAND;
        for(;;) {
            switch( state ) {
                case states::WAITING_FOR_COMMAND: {
                    auto event = wait( game_length_flag + start_game_flag );
                    if( event == game_length_flag ) {
                        send_control.send( game_length_pool.read() );
                    }
                    else if( event == start_game_flag ) {
                        send_control.send( start_game_value );
                    }
                    break;
                }
            }
        }
    }
};

#endif