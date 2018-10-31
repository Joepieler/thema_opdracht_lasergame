#ifndef PLAYERCONTROL_HPP
#define PLAYERCONTROL_HPP

#include "hwlib.hpp"
#include "rtos.hpp"

#include "msg_listener.hpp"
#include "PlayerData.hpp"
#include "Weapon.hpp"

class PlayerControl : public rtos::task<> : public msg_listener {
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
	SpeakerControl & speaker_control;
	PlayerData & player_data;
	Weapon & weapon;
	GameLogs & game_logs;
	
	player_ID
public:
	PlayerControl( const char * name, ShootControl & shoot_control, DisplayControl & display_control, SpeakerControl & speaker_control, PlayerData & player_data, Weapon & weapon, GameLogs & game_logs ):
		task( 4, name ),
		msg_channel( this, "msg_channel" ),
		trigger_flag( this, "trigger_flag" ),
		reload_flag( this, "reload_flag" ),
		print_data_flag( this, "print_data_flag" ),
		reload_timer( this, "reload_timer" ),
		shot_delay_timer( this, "shot_delay_timer" ),
		death_timer( this, "death_timer" ),
		game_timer( this, "game_timer" ),
		shoot_control( shoot_control ),
		display_control( display_control ),
		speaker_control( speaker_control ),
		player_data( player_data ),
		weapon( weapon ),
		game_logs( game_logs )
	{}
	
	void setPlayerNumber( uint8_t player_number ) {
		player_number_channel.write( player_number );
	}
	
	void setWeapon( uint8_t weapon_number ) {
		player_weapon_channel.write( weapon_number );
	}
	
	virtual void msg_received( const ir_msg & msg ) override {
		msg_channel.write( msg );
	};
	
	void main() override {
		enum states { INIT_GAME, PLAYING, POST_GAME };
		enum playing_states { ALIVE, DEAD };
		states state = INIT_GAME;
		ir_msg msg;
		uint8_t game_length;
		bool shootable = true;
		
		for(;;) {
			switch(state) {
				case states::INIT_GAME: {
					player_data.setHealth( player_data.getMaxHealth() );
					weapon.setAmmo( weapon.getMaxAmmo() );
					player_data.setDeaths( 0 );
					auto event = wait( player_number_channel + player_weapon_channel + msg_channel );
					if( event == player_number_channel ) {
						player_data.setPlayerID( player_number_channel.read() );
					}
					else if( event == player_weapon_channel ) {
						weapon.setWeapon( player_weapon_channel.read() );
					}
					else if( event == msg_channel ){
						msg = msg_channel.read();
						if( msg.player == 0 && msg.data < 16 ) {
							game_length = msg.data;
						}
						else if( msg.player == 0 && msg.data >= 16) {
							for( unsigned int i = 5; i >= 0; i--){
								display_control.showCountdown( i );
								hwlib::wait_ms( 1000 );
							}
							game_timer.set( game_length );
							state = states::PLAYING;
						}
					}
					break;
				}
				
				case states::PLAYING: {
					playing_states playing_state = ALIVE;
					
					switch(playing_state){
						case playing_states::ALIVE: {
							auto event = wait( trigger_flag + reload_flag + shot_delay_timer + reload_timer + msg_channel + game_timer );
							if( event == game_timer ){
								state = states::POST_GAME;
							}
							else if( event == trigger_flag && shootable == true && weapon.getAmmo() > 0 ) {
								shoot_control.shoot(player_data.getPlayerID(), weapon.getWeaponID());
								weapon.setAmmo( weapon.getAmmo() - 1 );
								shootable = false;
								shot_delay_timer.set( weapon.getShotDelay() );
							}
							else if( event == reload_flag ){
								shootable = false;
								reload_timer.set( weapon.getReloadTime() );
								weapon.setAmmo( weapon.getMaxAmmo() );
							}
							else if( event == shot_delay_timer || event == reload_timer ) {
								shootable = true;
							}
							else if( event == msg_channel) {
								msg = msg_channel.read();
								if( msg.player > 0 && msg.player != player_data.getPlayerID() ){
									if( weapon.getWeaponDamage(msg.data) > player_data.getHealth() ){
										player_data.setHealth( 0 );
										playing_state = playing_states::DEAD;
									}
									else{ 
										player_data.setHealth( (player_data.getHealth() - weapon.getWeaponDamage(msg.data) ) );
									}
								}
							}
							break;
						}
						
						case playing_states::DEAD: {
							death_timer.set( player_data.getDeathLength() );
							player_data.setHealth( player_data.getMaxHealth() );
							weapon.setAmmo( weapon.getMaxAmmo() );
							game_logs.addLog( msg.player, msg.data );
							player_data.setDeaths( (player_data.getDeaths() + 1 ) );
							auto event = wait( death_timer + game_timer );
							if( event == game_timer ) {
								state = states::POST_GAME;
							}
							else if ( event == death_timer ) {
								playing_state = playing_states::ALIVE
							}
						}
					}
					break;
				}
				
				case states::POST_GAME: {
					state = states::INIT_GAME;
					break;
				}
			}
		}
	}
};

#endif