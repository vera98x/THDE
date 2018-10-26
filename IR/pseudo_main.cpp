#include "hwlib.hpp"
#include "ir_decoder.hpp"

int main( void )
{	
    
   // kill the watchdog, wait for the PC console to start
   WDT->WDT_MR = WDT_MR_WDDIS;
   hwlib::wait_ms( 1000 );   
   namespace target = hwlib::target; 
   
   auto decoder = target::pin_in( target::pins::d7 );
   
}
