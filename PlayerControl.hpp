#ifndef PLAYERCONTROL_HPP
#define PLAYERCONTROL_HPP

#include "hwlib.hpp"
#include "rtos.hpp"

#include "msg_listener.hpp"

class PlayerControl : public rtos::task<> : public msg_listener {
private:
	rtos::channel< ir_msg, 1024 > msg_channel;
	rtos::channel< 
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
	
	void setPlayerNumber( uint8_t player_number ){
		
	}
	
	void setWeapon( uint8_t weapon_number ){
		
	}
	
	void main() override{
		
	}
};

#endif