// ==========================================================================
//
// File      : PlayerData.hpp
// Copyright : bartvannetburg@hotmail.com 2018
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// ==========================================================================

// this file contains Doxygen lines
/// @file

#ifndef PLAYERDATA_HPP
#define PLAYERDATA_HPP

/// \brief
/// This class contains all the playerdata.
class PlayerData {
private:
	uint8_t player_ID;
	uint8_t health;
	uint8_t max_health;
	uint8_t deaths;
	uint8_t death_length;
public:
	/// \brief
	/// This is the constructor for the playerdata.
	/// \details
	/// The constructor sets all the playerdata to default values.
	PlayerData():
		health( 100 ),
		max_health( 100 ),
		deaths( 0 ),
		death_length( 6 )
	{}
	
	/// \brief
	/// This function sets the playerID.
	void setPlayerID( uint8_t player ) {
		player_ID = player;
	}
	
	/// \brief
	/// This function sets the player health.
	void setHealth( uint8_t amount ) {
		health = amount;
	}
	
	/// \brief
	/// This function sets the player deaths.
	void setDeaths( uint8_t amount ) {
		deaths = amount;
	}
	
	/// \brief
	/// This function returns the playerID.
	uint8_t getPlayerID() {
		return player_ID;
	}
	
	/// \brief
	/// This function returns the player health.
	uint8_t getHealth() {
		return health;
	}
	
	/// \brief
	/// This function returns the maximum player health.
	uint8_t getMaxHealth() {
		return max_health;
	}
	
	/// \brief
	/// This function returns the player deaths.
	uint8_t getDeaths() {
		return deaths;
	}
	
	/// \brief
	/// This function returns the player death length.
	uint8_t getDeathLength() {
		return death_length;
	}
};

#endif