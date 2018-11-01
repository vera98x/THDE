#include "hwlib.hpp"
#include "ir_send.hpp"
#include "ir_detector.hpp"
#include "buzzer.hpp"
#include "runGameController.hpp"
#include "button.hpp"
#include "WifiTaak.hpp"
#include "hardware_usart.hpp"





int main( void )
{
	// kill the watchdog
	WDT->WDT_MR = WDT_MR_WDDIS;
	hwlib::wait_ms(1000);
	namespace target = hwlib::target;
	auto encoder = hwlib::target::d2_36kHz();
	auto listener = hwlib::target::pin_in(target::pins::d7);
	// buzzer 
	auto lsp = hwlib::target::pin_out( target::pins::d8 );
	// display
	auto scl = target::pin_oc( target::pins::scl );
	auto sda = target::pin_oc( target::pins::sda );   
	auto i2c_bus = hwlib::i2c_bus_bit_banged_scl_sda( scl,sda );   
	auto oled    = hwlib::glcd_oled( i2c_bus, 0x3c );
	auto font = hwlib::font_default_8x8();
	auto display = hwlib::window_ostream( oled, font );

	//button
	auto gunTrigger = target::pin_in(target::pins::d5);
	UARTLib::HardwareUART wifi_chip = UARTLib::HardwareUART(2400, UARTLib::HardwareUART::UARTController::THREE);
	wifi_chip.begin();

	WifiTaak WT(wifi_chip, nullptr);

	OLEDcontroller window(font, display);
	ir_send transmitter = ir_send(encoder);
	buzzer buzzertask = buzzer(lsp);

	runGameController rGC(buzzertask, transmitter, window, &WT);
	WT.setListener(& rGC);
	ir_decoder decoder = ir_decoder(rGC, 300);
	ir_detector detector(decoder, listener);

	button b(gunTrigger, transmitter);

	rtos::run();


	return 0;
}
