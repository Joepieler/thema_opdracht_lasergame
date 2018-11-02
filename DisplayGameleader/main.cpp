#include "hwlib.hpp"
#include "rtos.hpp"
#include "DisplayControl.hpp"

int main( void ){	
  
	WDT->WDT_MR = WDT_MR_WDDIS;
    
    hwlib::wait_ms(500);
    
    auto scl = hwlib::target::pin_oc( hwlib::target::pins::scl );
    auto sda = hwlib::target::pin_oc( hwlib::target::pins::sda );
   
    auto i2c_bus = hwlib::i2c_bus_bit_banged_scl_sda( scl,sda );
    
    auto display = hwlib::glcd_oled( i2c_bus, 0x3c );  
    
	auto display_control = DisplayControl("display_control", display);

    display_control.showCommand(200);
    hwlib::wait_ms(3000);
    display_control.showCommand(5);
    hwlib::wait_ms(3000);
    display_control.clearCommandValue();

	rtos::run();
}