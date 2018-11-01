

#include "hwlib.hpp"
#include "keypad.hpp"
#include "keypadPlayer.hpp"
#include "rtos.hpp"


int main( void ){	
    // kill the watchdog
    WDT->WDT_MR = WDT_MR_WDDIS;
    hwlib::wait_ms(500);
    auto m = KeypadPlayer("keypadplayer");
    
   (void)m;
   rtos::run();
}
