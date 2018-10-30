#include "hwlib.hpp"
#include "rtos.hpp"
#include "ShootControl.hpp"
#include "button.hpp"
#include "IRControl.hpp"



int main(void){
    // kill the watchdog
    WDT->WDT_MR = WDT_MR_WDDIS;
    hwlib::wait_ms(500);
    
    auto trigger = hwlib::target::pin_in(hwlib::target::pins::d5);
    auto IR_control = IRControl();
    auto transmit = ShootControl(IR_control);
    auto ir_pause_detector = Button(trigger, transmit);
    (void) ir_pause_detector;
    (void) transmit;
    rtos::run();
}
