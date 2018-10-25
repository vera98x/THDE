#include "hwlib.hpp"


int main( void ){	
     // kill the watchdog
   WDT->WDT_MR = WDT_MR_WDDIS;
   hwlib::wait_ms(1000);
   namespace target = hwlib::target;
   auto encoder = hwlib::target::d2_36kHz();
   
   bool values[16] = {1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0};
   
   while (1){
       encoder.set(1);
       hwlib::cout << "start" << "\n";
       for (int i = 0; i < 2; i++){
           for(int i = 0; i < 16; i++){
               if (values[i]){
                   encoder.set(1);
                   hwlib::wait_us(1600);
                   encoder.set(0);
                   hwlib::wait_us(800);
               } else {
                   encoder.set(1);
                   hwlib::wait_us(800);
                   encoder.set(0);
                   hwlib::wait_us(1600);
               }
               
               hwlib::cout << values[i] << "\n";
           }
           hwlib::cout << "end  " << i << "\n";
           encoder.set(0);
           hwlib::wait_ms(3000);
       }
       hwlib::wait_ms(4);
   }
   return 0;
}
