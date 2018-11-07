// ==========================================================================
//
// File      : Weapon.hpp
// Copyright : bartvannetburg@hotmail.com 2018
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// ==========================================================================

// this file contains Doxygen lines
/// @file

#ifndef WEAPONS_HPP
#define WEAPONS_HPP

/// \brief
/// This class contains the data of the weapon.
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
	/// \brief
	/// This is the constructor for the weapon class
	/// \details
	/// The constructor sets all the weapon stats based on the number entered in the constructor.
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
	
	/// \brief
	/// This function returns the weapon name.
	const char * getWeaponName( uint8_t weapon_ID ) {
		if( weapon_ID == 1 ) {
			return "AR";
		}
		else if( weapon_ID == 2 ) {
			return "SNIPER";
		}
		return "None";
	}
	
	/// \brief
	/// This function returns the weapon damage.
	uint8_t getWeaponDamage( uint8_t weapon_ID ) {
		if( weapon_ID == 1 ) {
			return 40;
		}
		else if( weapon_ID == 2 ) {
			return 100;
		}
		return 0;
	}
	
	/// \brief
	/// This function returns the weaponID.
	uint8_t getWeaponID() {
		return weapon_ID;
	}
	
	/// \brief
	/// This function returns the ammo of the weapon.
	uint8_t getAmmo() {
		return ammo;
	}
	
	/// \brief
	/// This function returns the maximum ammo of the weapon.
	uint8_t getMaxAmmo() {
		return max_ammo;
	}
	
	/// \brief
	/// This function returns the shot delay of the weapon.
	uint8_t getShotDelay() {
		return shot_delay;
	}
	
	/// \brief
	/// This function returns the reload time of the weapon.
	uint8_t getReloadTime() {
		return reload_time;
	}
	
	/// \brief
	/// This function sets the ammo for the weapon.
	void setAmmo( uint8_t amount ) {
		ammo = amount;
	} 
};

#endif