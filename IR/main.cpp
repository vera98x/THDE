#include "hwlib.hpp"
#include "ir_send.hpp"
#include "ir_detector.hpp"


int main( void ){	
     // kill the watchdog
   WDT->WDT_MR = WDT_MR_WDDIS;
   hwlib::wait_ms(1000);
   namespace target = hwlib::target;
   auto encoder = hwlib::target::d2_36kHz();
   auto listener = hwlib::target::pin_in(target::pins::d7);
   ir_decoder decoder = ir_decoder();
   ir_detector detector(decoder, listener);
   ir_send transmitter = ir_send(encoder);
   
   
   rtos::run();
   
   return 0;
}
