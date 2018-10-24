#include "hwlib.hpp"
#include "rtos.hpp"

#include "pause_detector.hpp"
#include "msg_decoder.hpp"
#include "msg_logger.hpp"

int main( void ){	
  
	WDT->WDT_MR = WDT_MR_WDDIS;
	hwlib::wait_ms(500);

	auto ir_receiver_pin = hwlib::target::pin_in(hwlib::target::pins::d8);

	auto ir_msg_logger = msg_logger("pause_detector");
	auto ir_msg_decoder = msg_decoder("msg_decoder", ir_msg_logger);	
	auto ir_pause_detector = pause_detector("pause_detector", ir_receiver_pin, ir_msg_decoder);
  
	(void) ir_msg_logger;  
	(void) ir_msg_decoder;
	(void) ir_pause_detector;

	rtos::run();
}