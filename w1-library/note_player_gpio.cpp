#include "hwlib.hpp"
#include "note_player_gpio.hpp"

  
void note_player_gpio::play( const note & n ){
   if( n.frequency == 0 ){
      hwlib::wait_us( n.duration );
   } else {
      auto end = hwlib::now_us() + n.duration;
      auto half_period = 1'000'000 / ( 2 * n.frequency );    
      while( end > hwlib::now_us() ){
         lsp.set( 1 );
         hwlib::wait_us( half_period );
         lsp.set( 0 );
         hwlib::wait_us( half_period );
      }
   }     
}
