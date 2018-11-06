#ifndef KEYPAD_HPP
#define KEYPAD_HPP

#include "hwlib.hpp"
#include "rtos.hpp"

class Keypad{
private:
    hwlib::target::pin_oc x0 = hwlib::target::pin_oc( hwlib::target::pins::d24 );
    hwlib::target::pin_oc x1 = hwlib::target::pin_oc( hwlib::target::pins::d26 );
    hwlib::target::pin_oc x2 = hwlib::target::pin_oc( hwlib::target::pins::d28 );
    hwlib::target::pin_oc x3 = hwlib::target::pin_oc( hwlib::target::pins::d30 );
    hwlib::port_oc_from_pins x = hwlib::port_oc_from_pins( x0, x1, x2, x3 );
    hwlib::target::pin_in y0 = hwlib::target::pin_in( hwlib::target::pins::d32 );
    hwlib::target::pin_in y1 = hwlib::target::pin_in( hwlib::target::pins::d34 );
    hwlib::target::pin_in y2 = hwlib::target::pin_in( hwlib::target::pins::d36 );
    hwlib::target::pin_in y3 = hwlib::target::pin_in( hwlib::target::pins::d38 );
    hwlib::port_in_from_pins y = hwlib::port_in_from_pins( y0, y1, y2, y3 );
    hwlib::matrix_of_switches ms = hwlib::matrix_of_switches(x,y);
    hwlib::keypad<16> m = hwlib::keypad<16>(ms, "123A456B789C*0#D");
public:
    char getc(){
        return m.getc();
    }
};

#endif //KEYBOARD_HPP