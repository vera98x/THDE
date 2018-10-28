#include "hwlib.hpp"
#include "ir_send.hpp"
#include "ir_detector.hpp"
#include "buzzer.hpp"
#include "runGameController.hpp"




int main( void )
{
	// kill the watchdog
	WDT->WDT_MR = WDT_MR_WDDIS;
	hwlib::wait_ms(1000);
	namespace target = hwlib::target;
	
	// IR-LED & IR-RECEIVER
	auto encoder = hwlib::target::d2_36kHz();
	auto listener = hwlib::target::pin_in(target::pins::d7);
	
	// BUZZER
	auto lsp = hwlib::target::pin_out( target::pins::d8 );
	
	//display 
	auto scl = target::pin_oc( target::pins::scl );
	auto sda = target::pin_oc( target::pins::sda );   
	auto i2c_bus = hwlib::i2c_bus_bit_banged_scl_sda( scl,sda );   
	auto display = hwlib::glcd_oled_buffered( i2c_bus, 0x3C);
	auto font = hwlib::font_default_8x8();
   
   
   ir_send transmitter = ir_send(encoder);
   buzzer buzzertask = buzzer(lsp);
   
   runGameController rGC(buzzertask, transmitter);
   
   ir_decoder decoder = ir_decoder(rGC, 300);
   ir_detector detector(decoder, listener);
   
   rtos::run();
   
   
   return 0;
}