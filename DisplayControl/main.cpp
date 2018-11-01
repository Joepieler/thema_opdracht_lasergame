#include "hwlib.hpp"
#include "rtos.hpp"
#include "DisplayControl.hpp"

int main( void ){	
  
	WDT->WDT_MR = WDT_MR_WDDIS;
    
    auto scl = hwlib::target::pin_oc( hwlib::target::pins::scl );
    auto sda = hwlib::target::pin_oc( hwlib::target::pins::sda );
   
    auto i2c_bus = hwlib::i2c_bus_bit_banged_scl_sda( scl,sda );
    
    auto display = hwlib::glcd_oled( i2c_bus, 0x3c );  
    
	auto display_control = DisplayControl("display_control", display);

    display_control.showWeapon("sniper");
    display_control.showDeaths(4);
    display_control.showCountdown(5);
    display_control.showHealth(100);
    display_control.showDeaths(0);

	rtos::run();
}