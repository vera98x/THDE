#ifndef _RUNGAMECONTROLLER_HPP
#define _RUNGAMECONTROLLER_HPP


#include "hwlib.hpp"
#include "rtos.hpp"
#include "buzzer.hpp"
#include "ir_send.hpp"
#include "OLEDcontroller.hpp"


class runGameController : public rtos::task<>{
    
private:
    buzzer & bz;
    ir_send & encoder;
	OLEDcontroller & window;
    bool values[16] = {1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0};
 
    struct playerInfo{
        int playerNR;
        int gunNR;
    };
    
    rtos::channel<playerInfo, 1024> playerInfoQueue;
    
    void main() override {
        const char* c = "hello"; 
		encoder.setIrpattern(22, 0);
        while(1){
            //bz.GameOverSound();
            //hwlib::wait_ms(6000);
            //bz.HitSound();
            //hwlib::wait_ms(6000);
            //bz.GotKilledSound();
            //hwlib::wait_ms(6000);
            //bz.LastMinuteSound();
            //hwlib::wait_ms(6000);
            //bz.YouKilledSound();
            
            hwlib::wait_ms(6000);
			window.showHPchanged(60);
			hwlib::wait_ms(11111);
			window.showKiller(c);
			hwlib::wait_ms(11111);
			window.showOneMinute();
			hwlib::wait_ms(11111);
        }
    }
    
public:
    runGameController(buzzer & bz, ir_send & encoder, OLEDcontroller & window):
    task(7, "runGameTask"),
    bz (bz),
    encoder ( encoder ),
	window(window),
    playerInfoQueue(this, "playerInfoQueue")
    {}
    
    void sendPlayerInfo(int playerNR, int gunNR){
        playerInfo pi{playerNR, gunNR};
        playerInfoQueue.write(pi);
    }

    
    
    
};


#endif