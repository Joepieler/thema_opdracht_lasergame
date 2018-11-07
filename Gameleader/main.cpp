#include "hwlib.hpp"
#include "rtos.hpp"

#include "GameLeader.hpp"
#include "KeypadControl.hpp"
#include "DisplayControl.hpp"
#include "IRControl.hpp"
#include "SendControl.hpp"


int main(void){
    // kill the watchdog
    WDT->WDT_MR = WDT_MR_WDDIS;
    hwlib::wait_ms(500);
    
    auto scl = hwlib::target::pin_oc( hwlib::target::pins::scl );
    auto sda = hwlib::target::pin_oc( hwlib::target::pins::sda );
    auto i2c_bus = hwlib::i2c_bus_bit_banged_scl_sda( scl, sda );
    auto display = hwlib::glcd_oled( i2c_bus, 0x3c );
    
    auto IR_control      = IRControl( "IR control", 0 );
    auto send_control    = SendControl( "send control", 1, IR_control );
    auto game_leader     = GameLeader( "game leader", 3, send_control );
    auto display_control = DisplayControl( "display control", 4, display );
    auto keypad_control  = KeypadControl( "keypad control", 2, game_leader, display_control);
    
    (void) IR_control;
    (void) send_control;
    (void) game_leader;
    (void) keypad_control;
    
    rtos::run();
}