#include "hwlib.hpp"
#include "rtos.hpp"
#include "Button.hpp" 
#include "ButtonListener.hpp" 
#include "BuzzerControl.hpp"
#include "DisplayControl.hpp"
#include "GameLogs.hpp"
#include "IRControl.hpp"
#include "KeypadControl.hpp"
#include "MsgDecoder.hpp"
#include "MsgListener.hpp"
#include "PauseDetector.hpp"
#include "PauseListener.hpp"
#include "PlayerControl.hpp"
#include "PlayerData.hpp"
#include "ShootControl.hpp"
#include "Weapon.hpp"

int main( void ){
	WDT->WDT_MR = WDT_MR_WDDIS;
	hwlib::wait_ms(500);
	
	auto deadled = hwlib::target::pin_out(hwlib::target::pins::d11);
	auto reloadled = hwlib::target::pin_out(hwlib::target::pins::d12);

	auto triggerpin = hwlib::target::pin_in(hwlib::target::pins::d3);
	auto reloadpin = hwlib::target::pin_in(hwlib::target::pins::d4);
	auto IR_receive_pin = hwlib::target::pin_in(hwlib::target::pins::d7);
	auto buzzer = hwlib::target::pin_out(hwlib::target::pins::d6);
	
	auto scl = hwlib::target::pin_oc(hwlib::target::pins::scl);
	auto sda = hwlib::target::pin_oc(hwlib::target::pins::sda);
	auto i2c_bus = hwlib::i2c_bus_bit_banged_scl_sda(scl, sda);
	auto display = hwlib::glcd_oled(i2c_bus, 0x3c);
	
	auto display_control = DisplayControl("DisplayControl", 8, display);
	auto game_logs = GameLogs();
	auto weapon = Weapon();
	auto player_data = PlayerData();
	auto buzzer_control = BuzzerControl("Buzzer", 9, buzzer);
	auto IR_control = IRControl("IRControl", 2);
	auto shoot_control = ShootControl("ShootControl", 3, IR_control, player_data, weapon);
	auto player_control = PlayerControl("PlayerControl", 4, shoot_control, display_control, buzzer_control, player_data, weapon, game_logs, deadled, reloadled);
	auto keypad_control = KeypadControl("KeypadControl", 7, display_control, player_control, weapon, player_data);
	auto msg_decoder = MsgDecoder("MsgDecoder", 1, player_control);
	auto pause_detector = PauseDetector("PauseDetector", 0, IR_receive_pin, msg_decoder);
	auto trigger = Button("trigger", 5, triggerpin, player_control, 0);
	auto reload = Button("reload", 6, reloadpin, player_control, 1);
	
	(void) display_control;
	(void) keypad_control;
	(void) buzzer_control;
	(void) IR_control;
	(void) shoot_control;
	(void) player_control;
	(void) msg_decoder;
	(void) pause_detector;
	(void) trigger;
	(void) reload;
	rtos::run();
}