#ifndef DISPLAY_CONTROL_HPP
#define DISPLAY_CONTROL_HPP

#include "hwlib.hpp"
#include "rtos.hpp"

class DisplayControl : public rtos::task<>{
private:
    	rtos::flag 
        display_command_flag,  
        clear_flag;
        
    rtos::pool<uint8_t>
        command_pool;
    
    const uint8_t display_size_y = 64;
    const uint8_t display_size_x = 128;
    const uint8_t devider = 80;
    hwlib::font_default_8x8 font = hwlib::font_default_8x8();
	const uint8_t font_height = 8;

    hwlib::glcd_oled & display;

    
    hwlib::window_part value_names = hwlib::window_part( display, hwlib::location (0, 0), hwlib::location (devider, font_height) );	
    hwlib::window_ostream value_names_ostream = hwlib::window_ostream(value_names, font);
    
    hwlib::window_part command_value = hwlib::window_part( display, hwlib::location (0, font_height * 3), hwlib::location (display_size_x, font_height * 4) );
    hwlib::window_ostream command_value_ostream = hwlib::window_ostream(command_value, font);

public:

    DisplayControl( const char * name, hwlib::glcd_oled & display):
		task( 1, name ),
        display_command_flag(this, "display command flag"),
        clear_flag(this, "display clear flag"),
        
        command_pool("display command pool"),
        
        display(display)
	{
        display.clear();
        value_names_ostream 
        << "\t0000Command:"
        << hwlib::flush;

    }
//////////////////////////////////// interface /////////////////////////////////////////////////////////    
    void showCommand(uint8_t value){
        display_command_flag.set();
        command_pool.write(value);
    }
    
    void clear(){
        display_command_flag.set();
    }
////////////////////////////////////////////end interface///////////////////////////////////////////////////

    void updateCommandValue(uint8_t value){
        command_value_ostream 
            << "\t0300"
            << (int)value
            << hwlib::flush;
    }
    
    void clearCommandValue(){
        command_value.clear();
    }

    void main() override {
		enum states { WAIT_FOR_MESSAGE};
		states state = states::WAIT_FOR_MESSAGE;
        for(;;){
			switch( state ) {
                case states::WAIT_FOR_MESSAGE: {
                    auto event = wait(
                        display_command_flag + 
                        clear_flag
                    );
                    
                    if(event == display_command_flag){
                        updateCommandValue(command_pool.read());
                    } else if(event == clear_flag){
                        clearCommandValue();
                    } 
                    break;
                }
            }
        }
    }
};
#endif // DISPLAY_CONTROL_HPP