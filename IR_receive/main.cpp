#include "hwlib.hpp"
#include "rtos.hpp"

#include "pause_detector.hpp"
#include "msg_decoder.hpp"
#include "msg_logger.hpp"

int main( void ){	
  
  WDT->WDT_MR = WDT_MR_WDDIS;
  hwlib::wait_ms(500);

  auto ir_receiver_pin = hwlib::target::pin_in(hwlib::target::pins::d8);

  auto scl = hwlib::target::pin_oc(hwlib::target::pins::scl1);
  auto sda = hwlib::target::pin_oc(hwlib::target::pins::sda1);
  auto i2c_bus = hwlib::i2c_bus_bit_banged_scl_sda( scl,sda );
  auto display = hwlib::glcd_oled(i2c_bus, 0x3C);
  display.clear();
  display.flush();

  hwlib::font_default_8x8 f1 = hwlib::font_default_8x8();

  hwlib::window_part  static_part = hwlib::window_part(display, hwlib::location(0, 0), hwlib::location(display.size.x/2, display.size.y));
  hwlib::window_part  dynamic_part = hwlib::window_part(display, hwlib::location(display.size.x/2, 0), hwlib::location(display.size.x, display.size.y));
  hwlib::window_ostream static_window = hwlib::window_ostream(static_part, f1);
  hwlib::window_ostream dynamic_window = hwlib::window_ostream(dynamic_part, f1);

  auto ir_msg_logger = msg_logger("pause_detector", static_window, dynamic_window);
  auto ir_msg_decoder = msg_decoder("msg_decoder", ir_msg_logger);
  auto ir_pause_detector = pause_detector("pause_detector", ir_receiver_pin, ir_msg_decoder);
  
  (void) ir_msg_logger;  
  (void) ir_msg_decoder;
  (void) ir_pause_detector;

  rtos::run();
}