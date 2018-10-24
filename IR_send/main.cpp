#include "hwlib.hpp"
#include "rtos.hpp"
#include "IR_transmit.hpp"

int main(void){
    // kill the watchdog
    WDT->WDT_MR = WDT_MR_WDDIS;
    hwlib::wait_ms(500);

    auto transmit = IR_transmit();
    (void) transmit;
    rtos::run();
}