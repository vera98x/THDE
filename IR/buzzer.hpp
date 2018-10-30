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
        play( note{ note::D5,  note::dQ } );
        play( note{ note::C5,  note::dQ } );
        play( note{ note::A4,  note::dH } );
    }
    
    void playLastMinuteSound(){
        play( note{ note::D5s, note::dQ } ); 
        play( note{ note::A4,  note::dH } );
    }
    
    void playHitSound(){
        play( note{ note::C5,  note::dQ } );
        play( note{ note::C5,  note::dQ } );
    }
    
    void playGameOverSound(){
        play( note{ 1318, 125000 } );
        play( note{ 1174, 250000 } );
        play( note{ 1046, 500000 } );
        play( note{ 1318, 125000 } );
    }
    
    void playYouKilledSound(){
        play( note{ 1046, 250000 } );
        play( note{ 440, 125000 } );
        play( note{ 784, 250000 } );
        play( note{ 1318, 750000 } );
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
    
    void GotKilledSound(){
        gotKilledFlag.set();
    }
    
    void LastMinuteSound(){
        lastMinuteFlag.set();
    }
    
    void HitSound(){
        hitFlag.set();
    }
    
    void GameOverSound(){
        gameOverFlag.set();
    }
    
    void YouKilledSound(){
        killFlag.set();
    }
    
    
    
    
};


#endif