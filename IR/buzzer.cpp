
#include "hwlib.hpp"
#include "rtos.hpp"
#include "note.hpp"
#include "buzzer.hpp"


    
void buzzer::play( const note & n ){
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
    
void buzzer::playGotKilledSound(){
    play( note{ 100,  300000 } );
    play( note{ 100,  300000 } );
    play( note{ 100,  300000 } );
    play( note{ 100,  300000 } );
}
    
void buzzer::playLastMinuteSound(){
    play( note{ 100, 300000} ); 
    play( note{ 1000,  300000 } );
    play( note{ 2000, 300000 } );
    play( note{ 5500,  300000 } );
    play( note{ 2000,  300000 } );
    play( note{ 1000,  300000 } );
    play( note{ 100,  300000 } );
}
    
void buzzer::playHitSound(){
    play( note{ 555,  300000 } );
    play( note{ 555,  300000 } );
}
    
void buzzer::playGameOverSound(){
    play( note{ 7777, 300000 } );
    play( note{ 5555, 300000 } );
    play( note{ 3333, 300000 } );
    play( note{ 1111, 300000 } );
}
    
void buzzer::playYouKilledSound(){
    play( note{ 5555, 300000 } );
    play( note{ 111, 300000 } );
    play( note{ 5555, 300000 } );
    play( note{ 111, 300000 } );
}
    
    
void buzzer::main(){
    while(1){
        auto done = wait(gotKilledFlag + gameOverFlag + lastMinuteFlag + hitFlag + killFlag);
        if (done == gotKilledFlag){
            playGotKilledSound();
        } else if(done == gameOverFlag){
            playGameOverSound();
        } else if(done == lastMinuteFlag){
            playLastMinuteSound();
        } else if(done == hitFlag){
            playHitSound();
        } else if(done == killFlag){
            playYouKilledSound();
        }
			
        hwlib::wait_ms(1000);
    }
}
    
    
buzzer::buzzer(hwlib::target::pin_out & lsp):
    task(5, "buzzerTask"),
    lsp ( lsp ),
    gotKilledFlag(this, "gotKilledFlag"),
    gameOverFlag(this, "gameOverFlag"),
    lastMinuteFlag(this, "lastMinuteFlag"),
    hitFlag(this, "hitFlag"),
    killFlag(this, "killFlag")
    {}
    
void buzzer::gotKilledSound(){
    gotKilledFlag.set();
}
    
void buzzer::lastMinuteSound(){
    lastMinuteFlag.set();
}
    
void buzzer::hitSound(){
    hitFlag.set();
}
    
void buzzer::gameOverSound(){
    gameOverFlag.set();
}
    
void buzzer::youKilledSound(){
    killFlag.set();
}
