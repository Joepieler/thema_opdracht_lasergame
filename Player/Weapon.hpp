#ifndef WEAPONS_HPP
#define WEAPONS_HPP

class Weapon {
private:
	const char * weapon_name = "None";
	uint8_t weapon_ID = 0;
	uint8_t damage = 0;
	uint8_t shot_delay = 0;
	uint8_t ammo = 0;
	uint8_t max_ammo = 0;
	uint8_t reload_time = 0;
public:
	void setWeapon( uint8_t weapon ) {
		if( weapon == 1 ){
			weapon_name = "AR";
			weapon_ID = weapon;
			damage = 40;
			shot_delay = 1;
			ammo = 30;
			max_ammo = 30;
			reload_time = 4;
		}
		else if( weapon == 2){
			weapon_name = "SNIPER";
			weapon_ID = weapon;
			damage = 100;
			shot_delay = 20;
			ammo = 5;
			max_ammo = 5;
			reload_time = 5;
		}
	}
	
	const char * getWeaponName( uint8_t weapon_ID ) {
		if( weapon_ID == 1 ) {
			return "AR";
		}
		else if( weapon_ID == 2 ) {
			return "SNIPER";
		}
		return "None";
	}
	
	uint8_t getWeaponDamage( uint8_t weapon_ID ) {
		if( weapon_ID == 1 ) {
			return 40;
		}
		else if( weapon_ID == 2 ) {
			return 100;
		}
		return 0;
	}
	
	uint8_t getWeaponID() {
		return weapon_ID;
	}
	
	uint8_t getAmmo() {
		return ammo;
	}
	
	uint8_t getMaxAmmo() {
		return max_ammo;
	}
	
	uint8_t getShotDelay() {
		return shot_delay;
	}
	
	uint8_t getReloadTime() {
		return reload_time;
	}
	
	void setAmmo( uint8_t amount ) {
		ammo = amount;
	} 
};

#endif