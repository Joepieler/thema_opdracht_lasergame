#ifndef PLAYERDATA_HPP
#define PLAYERDATA_HPP

class PlayerData {
private:
	uint8_t player_ID;
	uint8_t health;
	uint8_t max_health;
	uint8_t deaths;
	uint8_t death_length;
public:
	PlayerData():
		health( 100 ),
		max_health( 100 ),
		deaths( 0 ),
		death_length( 6 )
	{}
	
	void setPlayerID( uint8_t player ) {
		player_ID = player;
	}
	
	void setHealth( uint8_t amount ) {
		health = amount;
	}
	
	void setDeaths( uint8_t amount ) {
		deaths = amount;
	}
	
	uint8_t getPlayerID() {
		return player_ID;
	}
	
	uint8_t getHealth() {
		return health;
	}
	
	uint8_t getMaxHealth() {
		return max_health;
	}
	
	uint8_t getDeaths() {
		return deaths;
	}
	
	uint8_t getDeathLength() {
		return death_length;
	}
};

#endif