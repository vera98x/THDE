#include "hwlib.hpp"


int main( void ){	
     // kill the watchdog
   WDT->WDT_MR = WDT_MR_WDDIS;
   hwlib::wait_ms(1000);
   namespace target = hwlib::target;
   
   auto decoder = target::pin_in(target::pins::d7);
   
   
   while (1){
       hwlib::cout << "test: " << !decoder.get() << "\n";
       hwlib::wait_us(26);
   }
}
z