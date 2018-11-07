// ==========================================================================
//
// File      : PlayerControl.hpp
// Copyright : bartvannetburg@hotmail.com 2018
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// ==========================================================================

// this file contains Doxygen lines
/// @file

#ifndef PLAYERCONTROL_HPP
#define PLAYERCONTROL_HPP

#include "hwlib.hpp"
#include "rtos.hpp"

#include "MsgListener.hpp"
#include "ButtonListener.hpp"
#include "PlayerData.hpp"
#include "Weapon.hpp"
#include "BuzzerControl.hpp"
#include "GameLogs.hpp"
#include "Weapon.hpp"
#include "PlayerData.hpp"
#include "ShootControl.hpp"
#include "DisplayControl.hpp"

/// \brief
/// This class controls the player tasks.
class PlayerControl : public rtos::task<>, public MsgListener, public ButtonListener {
private:
	rtos::channel< ir_msg, 1024 > msg_channel;
	rtos::channel< uint8_t, 1024 > player_number_channel;
	rtos::channel< uint8_t, 1024 > player_weapon_channel;
	
	rtos::flag trigger_flag;
	rtos::flag reload_flag;
	rtos::flag print_data_flag;
	
	rtos::timer reload_timer;
	rtos::timer shot_delay_timer;
	rtos::timer death_timer;
	rtos::timer game_timer;
	
	ShootControl & shoot_control;
	DisplayControl & display_control;
	BuzzerControl & buzzer_control;
	PlayerData & player_data;
	Weapon & weapon;
	GameLogs & game_logs;
	
	hwlib::pin_out & reload_led;
	hwlib::pin_out & dead_led;
	
	const unsigned int countdown_interval = 1100;
	const unsigned int buzzer_hit_length = 100;
	const unsigned int buzzer_start_game_length = 100;
	const unsigned int initialization_pause = 10000;
	const unsigned int led_on = 1;
	const unsigned int led_off = 0;
public:
	/// \brief
	/// This is the constructor for the PlayerControl.
	/// \details
	/// The constructor expects shootcontrol, displaycontrol, buzzercontrol, playerdata, weapon, and gamelogs to send messages to or get data from.
	/// The constructor also expects two pins for the reload and dead leds.
	PlayerControl( const char * name, int priority, ShootControl & shoot_control, DisplayControl & display_control, BuzzerControl & buzzer_control, PlayerData & player_data, Weapon & weapon, GameLogs & game_logs, auto & reload_led, auto & dead_led ):
		task( priority, name ),
		msg_channel( this, "msg_channel" ),
		player_number_channel( this, "player_number_channel"),
		player_weapon_channel( this, "player_weapon_channel"),
		trigger_flag( this, "trigger_flag" ),
		reload_flag( this, "reload_flag" ),
		print_data_flag( this, "print_data_flag" ),
		reload_timer( this, "reload_timer" ),
		shot_delay_timer( this, "shot_delay_timer" ),
		death_timer( this, "death_timer" ),
		game_timer( this, "game_timer" ),
		shoot_control( shoot_control ),
		display_control( display_control ),
		buzzer_control( buzzer_control ),
		player_data( player_data ),
		weapon( weapon ),
		game_logs( game_logs ),
		reload_led( reload_led ),
		dead_led( dead_led )
	{}
	
	/// \brief
	/// This function writes in the player_number_channel.
	void setPlayerNumber( uint8_t player_number ) {
		player_number_channel.write( player_number );
	}
	
	/// \brief
	/// This function writes in the player_weapon_channel.
	void setWeapon( uint8_t weapon_number ) {
		player_weapon_channel.write( weapon_number );
	}
	
	/// \brief
	/// This function overwrites the msgReceived function and writes the messsage in the msg_channel.
	virtual void msgReceived( const ir_msg & msg ) override {
		msg_channel.write( msg );
	};
	
	/// \brief
	/// This function overwrites the buttonPressed function and sets the flag depending on which button is pressed.
	virtual void buttonPressed( unsigned int & button_number ) override {
		if( button_number == 0 ) {
			trigger_flag.set();
		}
		else if( button_number == 1 ) {
			reload_flag.set();
		}
	}
	
	/// \brief
	/// This function contains the state machine.
	/// \details
	/// This function consists of 5 states.
	/// The state init_game waits for the player_number and player_weapon from the keypad and a message from the msgdecoder.
	/// The message is used to set the game time and start the game.
	/// When the game starts the state becomes playing.
	/// The playing state consists of 3 states and starts with alive_able_to_shoot.
	/// All three substates of playing wait for a game timer to end the game print the logs and transition back to the init state.
	/// The alive_able_to_shoot state waits for the trigger or reload flag to set a timer and transition to the alive_not_able_to_shot state.
	/// It also waits for a message to check if the player gets hit.
	/// If the player dies the state transitions to the dead state.
	/// The alive_not_able_to_shoot state also waits for a message to check if the player gets hit.
	/// If the player dies the state transition to the dead state.
	/// If the timer set by the alive_able_to_shoot state runs out the state transitions back to the alive_able_to_shoot.
	/// The dead state resets the player health ammo and waits for the death timer.
	/// When the timer runs out the state transitions back to alive_able_to_shoot.
	void main() override {
		enum states { INIT_GAME, PLAYING };
		enum playing_states { ALIVE_ABLE_TO_SHOOT, ALIVE_NOT_ABLE_TO_SHOOT, DEAD };
		states state = INIT_GAME;
		playing_states playing_state;
		ir_msg msg;
		uint8_t game_length;
		
		for(;;) {
			switch(state) {
				case states::INIT_GAME: {
					auto event = wait( player_number_channel + player_weapon_channel + msg_channel );
					if( event == player_number_channel ) {
						player_data.setPlayerID( player_number_channel.read() );
						display_control.showPlayer( player_data.getPlayerID() );
					}
					else if( event == player_weapon_channel ) {
						weapon.setWeapon( player_weapon_channel.read() );
						display_control.showWeapon( weapon.getWeaponName(weapon.getWeaponID()) );
					}
					else if( event == msg_channel ){
						msg = msg_channel.read();
						if( msg.player == 0 && msg.data < 16 && msg.data > 0 ) {
							game_length = msg.data;
							buzzer_control.makeSound( buzzer_hit_length );
						}
						else if( msg.player == 0 && msg.data == 0) {
							player_data.setHealth( player_data.getMaxHealth() );
							weapon.setAmmo( weapon.getMaxAmmo() );
							player_data.setDeaths( 0 );
							buzzer_control.makeSound( buzzer_start_game_length );
							hwlib::wait_ms( buzzer_start_game_length );
							display_control.showHealth( player_data.getHealth() );
							display_control.showAmmo( weapon.getAmmo() );
							display_control.showDeaths( player_data.getDeaths() );
							hwlib::wait_ms( initialization_pause );
							for(int i = 5; i >= 0; i--){
								display_control.showCountdown(i);
								hwlib::wait_ms( countdown_interval );
							}
							trigger_flag.clear();
							reload_flag.clear();
							msg_channel.clear();
							reload_led.set(0);
							dead_led.set(0);
							game_timer.set( ( game_length * 60000 ) * rtos::ms );
							state = states::PLAYING;
							playing_state = ALIVE_ABLE_TO_SHOOT;
						}
					}
					break;
				}
				
				case states::PLAYING: {
					switch(playing_state){
						case playing_states::ALIVE_ABLE_TO_SHOOT: {
							auto event = wait( trigger_flag + reload_flag + msg_channel + game_timer );
							if( event == game_timer ) {
								state = states::INIT_GAME;
								reload_led.set(1);
								dead_led.set(1);
								game_logs.printLogs();
								game_logs.clearLogs();
							}
							else if( event == trigger_flag && weapon.getAmmo() > 0 ) {
								shoot_control.shoot();
								weapon.setAmmo( weapon.getAmmo() - 1 );
								display_control.showAmmo( weapon.getAmmo() );
								shot_delay_timer.set( (weapon.getShotDelay() * 100) * rtos::ms );
								playing_state = playing_states::ALIVE_NOT_ABLE_TO_SHOOT;
							}
							else if( event == reload_flag ){
								reload_timer.set( (weapon.getReloadTime() * 1000) * rtos::ms );
								weapon.setAmmo( weapon.getMaxAmmo() );
								reload_led.set( led_on );
								playing_state = playing_states::ALIVE_NOT_ABLE_TO_SHOOT;
							}
							else if( event == msg_channel) {
								msg = msg_channel.read();
								if( msg.player > 0 && msg.player != player_data.getPlayerID() ) {
									buzzer_control.makeSound( buzzer_hit_length );
									game_logs.addLog( msg.player, weapon.getWeaponName(msg.data) );
									if( weapon.getWeaponDamage(msg.data) >= player_data.getHealth() ) {
										player_data.setHealth( 0 );
										display_control.showHealth( player_data.getHealth() );
										dead_led.set( led_on );
										playing_state = playing_states::DEAD;
									}
									else{ 
										player_data.setHealth( (player_data.getHealth() - weapon.getWeaponDamage( msg.data ) ) );
										display_control.showHealth( player_data.getHealth() );
									}
								}
							}
							break;
						}
						
						case playing_states::ALIVE_NOT_ABLE_TO_SHOOT: {
							auto event = wait( shot_delay_timer + reload_timer + msg_channel + game_timer );
							if( event == game_timer ){
								state = states::INIT_GAME;
								reload_led.set(1);
								dead_led.set(1);
								game_logs.printLogs();
								game_logs.clearLogs();
							}
							else if( event == msg_channel ) {
								msg = msg_channel.read();
								if( msg.player > 0 && msg.player != player_data.getPlayerID() ) {
									buzzer_control.makeSound( buzzer_hit_length );
									game_logs.addLog( msg.player, weapon.getWeaponName(msg.data) );
									if( weapon.getWeaponDamage(msg.data) >= player_data.getHealth() ) {
										player_data.setHealth( 0 );
										display_control.showHealth( player_data.getHealth() );
										dead_led.set( led_on );
										playing_state = playing_states::DEAD;
									}
									else{ 
										player_data.setHealth( (player_data.getHealth() - weapon.getWeaponDamage(msg.data) ) );
										display_control.showHealth( player_data.getHealth() );
									}
								}
							}
							else if( event == shot_delay_timer ) {
								trigger_flag.clear();
								playing_state = playing_states::ALIVE_ABLE_TO_SHOOT;
							}
							else if( event == reload_timer ) {
								reload_flag.clear();
								display_control.showAmmo( weapon.getAmmo() );
								reload_led.set( led_off );
								playing_state = playing_states::ALIVE_ABLE_TO_SHOOT;
							}
							break;
						}
						
						case playing_states::DEAD: {
							reload_led.set(0);
							death_timer.set( (player_data.getDeathLength() * 1000) * rtos::ms );
							player_data.setDeaths( (player_data.getDeaths() + 1 ) );
							display_control.showDeaths( player_data.getDeaths() );
							auto event = wait( death_timer + game_timer );
							if( event == game_timer ) {
								state = states::INIT_GAME;
								reload_led.set(1);
								dead_led.set(1);
								game_logs.printLogs();
								game_logs.clearLogs();
							}
							else if ( event == death_timer ) {
								reload_flag.clear();
								trigger_flag.clear();
								player_data.setHealth( player_data.getMaxHealth() );
								weapon.setAmmo( weapon.getMaxAmmo() );
								display_control.showHealth( player_data.getHealth() );
								display_control.showAmmo( weapon.getAmmo() );
								msg_channel.clear();
								dead_led.set( led_off );
								playing_state = playing_states::ALIVE_ABLE_TO_SHOOT;
							}
						}
						break;
					}
					break;
				}
			}
		}
	}
};

#endif
