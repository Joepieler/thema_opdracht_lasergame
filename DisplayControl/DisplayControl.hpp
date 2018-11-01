#ifndef DISPLAY_CONTROL_HPP
#define DISPLAY_CONTROL_HPP

#include "hwlib.hpp"
#include "rtos.hpp"

class DisplayControl : public rtos::task<>{
private:
    	rtos::flag 
        display_command_flag, 
        display_countdown_flag, 
        display_deaths_flag, 
        display_ammo_flag, 
        display_health_flag, 
        display_player_flag, 
        display_weapon_flag;
        
    rtos::pool<uint8_t>
        command_pool,
        countdown_pool,
        deaths_pool, 
        ammo_pool, 
        health_pool, 
        player_pool;
    rtos::pool<const char *>
        weapon_pool;
    
    const uint8_t display_size_y = 64;
    const uint8_t display_size_x = 128;
    const uint8_t devider = 80;
    hwlib::font_default_8x8 font = hwlib::font_default_8x8();
	const uint8_t font_height = 8;

    hwlib::glcd_oled & display;

    
    hwlib::window_part value_names = hwlib::window_part( display, hwlib::location (0, 0), hwlib::location (80, display_size_y) );	
    hwlib::window_ostream value_names_ostream = hwlib::window_ostream(value_names, font);
    
    hwlib::window_part ammo_value = hwlib::window_part( display, hwlib::location (devider, 0), hwlib::location (display_size_x, font_height) );
    hwlib::window_ostream ammo_value_ostream = hwlib::window_ostream(ammo_value, font);

    hwlib::window_part health_value = hwlib::window_part( display, hwlib::location (devider, font_height), hwlib::location (display_size_x, font_height * 2) );
    hwlib::window_ostream health_value_ostream = hwlib::window_ostream(health_value, font);
    
    hwlib::window_part weapon_value = hwlib::window_part( display, hwlib::location (devider, font_height * 2), hwlib::location (display_size_x, font_height * 3) );
    hwlib::window_ostream weapon_value_ostream = hwlib::window_ostream(weapon_value, font);
    
    hwlib::window_part player_value = hwlib::window_part( display, hwlib::location (devider, font_height * 3), hwlib::location (display_size_x, font_height * 4) );
    hwlib::window_ostream player_value_ostream = hwlib::window_ostream(player_value, font);
    
    hwlib::window_part deaths_value = hwlib::window_part( display, hwlib::location (devider, font_height * 4), hwlib::location (display_size_x, font_height * 5) );
    hwlib::window_ostream deaths_value_ostream = hwlib::window_ostream(deaths_value, font);
    
    hwlib::window_part countdown_value = hwlib::window_part( display, hwlib::location (devider, font_height * 5), hwlib::location (display_size_x, font_height * 6) );
	hwlib::window_ostream countdown_value_ostream = hwlib::window_ostream(countdown_value, font);
    
    hwlib::window_part command_value = hwlib::window_part( display, hwlib::location (devider, font_height * 6), hwlib::location (display_size_x, font_height * 7) );
    hwlib::window_ostream command_value_ostream = hwlib::window_ostream(command_value, font);

public:

    DisplayControl( const char * name, hwlib::glcd_oled & display):
		task( 3, name ),
        display_command_flag(this, "display command flag"),
        display_countdown_flag(this, "display countdown flag"), 
        display_deaths_flag(this, "display deaths flag"), 
        display_ammo_flag(this, "display ammo flag"), 
        display_health_flag(this, "display health flag"), 
        display_player_flag(this, "display player flag"), 
        display_weapon_flag(this, "display weapon flag"),
        
        command_pool("display command pool"),
        countdown_pool("display countdown pool"), 
        deaths_pool("display deaths pool"), 
        ammo_pool("display ammo pool"), 
        health_pool("display health pool"), 
        player_pool("display player pool"), 
        weapon_pool("display weapon pool"),
        
        display(display)
	{
        display.clear();
        value_names_ostream 
        << "\t0000Ammo:"
        << "\t0001Health:"
        << "\t0002Weapon:"
        << "\t0003Player:"
        << "\t0004Deaths:"
        << "\t0005Countdown:"
        << "\t0006Command:"
        << hwlib::flush;

    }
//////////////////////////////////// interface /////////////////////////////////////////////////////////    
    void showCommand(uint8_t value){
        display_command_flag.set();
        command_pool.write(value);
    }
    
    void showCountdown(uint8_t value){
        display_countdown_flag.set();
        countdown_pool.write(value);
    }
    
    void showDeaths(uint8_t value){
        display_deaths_flag.set();
        deaths_pool.write(value);
    }
    
    void showAmmo(uint8_t value){
        display_ammo_flag.set();       
        ammo_pool.write(value);
    }
    
    void showHealth(uint8_t value){
        display_health_flag.set();
        health_pool.write(value);
    }
    
    void showPlayer(uint8_t value){
        display_player_flag.set();
        player_pool.write(value);
    }
    
    void showWeapon(const char * value){
        display_weapon_flag.set();
        weapon_pool.write(value);
    }
////////////////////////////////////////////end interface///////////////////////////////////////////////////

    void updateCommandValue(uint8_t value){
        command_value_ostream 
            << "\t0000"
            << (int)value
            << hwlib::flush;
    }
    
    void updateCountdownValue(uint8_t value){
        countdown_value_ostream 
            << "\t0000"
            << (int)value
            << hwlib::flush;
    }
    
    void updateDeathsValue(uint8_t value){
        deaths_value_ostream
            << "\t0000"
            << (int)value
            << hwlib::flush;
    } 
    void updateAmmoValue(uint8_t value){
        ammo_value_ostream
            << "\t0000"
            << (int)value
            << hwlib::flush;
    } 
    void updateHealtValue(uint8_t value){
        health_value_ostream
            << "\t0000"
            << (int)value
            << hwlib::flush;
    } 
    void updatePlayerValue(uint8_t value){
        player_value_ostream
            << "\t0000"
            << (int)value
            << hwlib::flush;
    }
    void updateWeaponValue(const char * value){
        weapon_value_ostream
            << "\t0000"
            << value
            << hwlib::flush;
    }


    void main() override {
		enum states { WAIT_FOR_MESSAGE};
		states state = states::WAIT_FOR_MESSAGE;
        for(;;){
			switch( state ) {
                case states::WAIT_FOR_MESSAGE: {
                    auto event = wait(
                        display_command_flag + 
                        display_countdown_flag + 
                        display_deaths_flag + 
                        display_ammo_flag + 
                        display_health_flag + 
                        display_player_flag + 
                        display_weapon_flag
                    );
                    
                    if(event == display_command_flag){
                        updateCommandValue(command_pool.read());
                        
                    } else if(event == display_countdown_flag){
                        updateCountdownValue(countdown_pool.read());
                        
                    } else if(event == display_deaths_flag){
                        updateDeathsValue(deaths_pool.read());
                        
                    } else if(event == display_ammo_flag){
                        updateAmmoValue(ammo_pool.read());
                        
                    } else if(event == display_health_flag){
                        updateHealtValue(health_pool.read());
                        
                    } else if(event == display_player_flag){
                        updatePlayerValue(player_pool.read());
                        
                    } else if(event == display_weapon_flag){
                        updateWeaponValue(weapon_pool.read());
                        
                    }

                }
            }
        }
    }
};
#endif // DISPLAY_CONTROL_HPP