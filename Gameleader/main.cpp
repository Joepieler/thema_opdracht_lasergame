#include "hwlib.hpp"
#include "rtos.hpp"
#include "BuzzerControl.hpp"
#include "GameLeader.hpp"
#include "KeypadControl.hpp"
#include "DisplayControl.hpp"

int main(void){
    // kill the watchdog
    WDT->WDT_MR = WDT_MR_WDDIS;
    hwlib::wait_ms(500);
    
    auto scl     = hwlib::target::pin_oc(hwlib::target::pins::scl);
    auto sda     = hwlib::target::pin_oc(hwlib::target::pins::sda);
    auto i2c_bus = hwlib::i2c_bus_bit_banged_scl_sda(scl, sda);
    auto display = hwlib::glcd_oled(i2c_bus, 0x3c);
    
    auto IR_control      = IRControl(0, "IR control");
    auto send_control    = SendControl(1, "send control",IR_control);
    auto game_leader     = GameLeader(3, "game leader", send_control);
    auto display_control = DisplayControl(4, "display control", display);
    auto keypad_control  = KeypadControl(2, "keypad control", game_leader, display_control);
    
    (void) IR_control;
    (void) send_control;
    (void) game_leader;
    (void) keypad_control;
    
    rtos::run();
}