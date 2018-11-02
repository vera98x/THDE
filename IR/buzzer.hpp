#ifndef _BUZZER_HPP
#define _BUZZER_HPP


#include "hwlib.hpp"
#include "rtos.hpp"
#include "note.hpp"


class buzzer : public rtos::task<>{
    
private:
    hwlib::target::pin_out & lsp;
    
    rtos::flag gotKilledFlag;
    rtos::flag gameOverFlag;
    rtos::flag lastMinuteFlag;
    rtos::flag hitFlag;
    rtos::flag killFlag;
    
    void play( const note & n ){
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
    
    void playGotKilledSound(){
        play( note{ 100,  300000 } );
        play( note{ 100,  300000 } );
        play( note{ 100,  300000 } );
        play( note{ 100,  300000 } );
    }
    
    void playLastMinuteSound(){
        play( note{ 100, 300000} ); 
        play( note{ 1000,  300000 } );
        play( note{ 2000, 300000 } );
        play( note{ 5500,  300000 } );
        play( note{ 2000,  300000 } );
        play( note{ 1000,  300000 } );
        play( note{ 100,  300000 } );
    }
    
    void playHitSound(){
        play( note{ 555,  300000 } );
        play( note{ 555,  300000 } );
    }
    
    void playGameOverSound(){
        play( note{ 7777, 300000 } );
        play( note{ 5555, 300000 } );
        play( note{ 3333, 300000 } );
        play( note{ 1111, 300000 } );
    }
    
    void playYouKilledSound(){
        play( note{ 5555, 300000 } );
        play( note{ 111, 300000 } );
        play( note{ 5555, 300000 } );
        play( note{ 111, 300000 } );
    }
    
    
    void main(){
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
			
            hwlib::wait_ms(6000);
        }
    }
    
public:
    buzzer(hwlib::target::pin_out & lsp):
    task(5, "buzzerTask"),
    lsp ( lsp ),
    gotKilledFlag(this, "gotKilledFlag"),
    gameOverFlag(this, "gameOverFlag"),
    lastMinuteFlag(this, "lastMinuteFlag"),
    hitFlag(this, "hitFlag"),
    killFlag(this, "killFlag")
    {}
    
    void gotKilledSound(){
        gotKilledFlag.set();
    }
    
    void lastMinuteSound(){
        lastMinuteFlag.set();
    }
    
    void hitSound(){
        hitFlag.set();
    }
    
    void gameOverSound(){
        gameOverFlag.set();
    }
    
    void youKilledSound(){
        killFlag.set();
    }
    
    
    
    
};


#endif