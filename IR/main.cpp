#include "hwlib.hpp"
#include "ir_send.hpp"
#include "ir_detector.hpp"
#include "buzzer.hpp"
#include "runGameController.hpp"




int main( void ){	
     // kill the watchdog
   WDT->WDT_MR = WDT_MR_WDDIS;
   hwlib::wait_ms(1000);
   namespace target = hwlib::target;
   auto encoder = hwlib::target::d2_36kHz();
   auto listener = hwlib::target::pin_in(target::pins::d7);
   // buzzer 
   auto lsp = hwlib::target::pin_out( target::pins::d8 );
   
   
   ir_send transmitter = ir_send(encoder);
   buzzer buzzertask = buzzer(lsp);
   
   runGameController rGC(buzzertask, transmitter);
   
   ir_decoder decoder = ir_decoder(rGC, 300);
   ir_detector detector(decoder, listener);
   
   rtos::run();
   
   
   return 0;
}
