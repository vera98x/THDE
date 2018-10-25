#include "hwlib.hpp"
#include "ir_send.hpp"


int main( void ){	
     // kill the watchdog
   WDT->WDT_MR = WDT_MR_WDDIS;
   hwlib::wait_ms(1000);
   namespace target = hwlib::target;
   auto encoder = hwlib::target::d2_36kHz();
   
   bool values[16] = {1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0};
   
   ir_send transmitter = ir_send(encoder, values);
   
   rtos::run;
   
   return 0;
}
