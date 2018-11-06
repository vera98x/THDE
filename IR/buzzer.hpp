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
    
    void play( const note & n );
    
    void playGotKilledSound();
    
    void playLastMinuteSound();
    
    void playHitSound();
    
    void playGameOverSound();
    
    void playYouKilledSound();
    
    
    void main();
    
public:
    buzzer(hwlib::target::pin_out & lsp);
    
    void gotKilledSound();
    void lastMinuteSound();
    
    void hitSound();
    void gameOverSound();
    
    void youKilledSound();
    
    
    
};


#endif