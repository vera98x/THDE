#ifndef _RUNGAMECONTROLLER_HPP
#define _RUNGAMECONTROLLER_HPP


#include "hwlib.hpp"
#include "rtos.hpp"
#include "buzzer.hpp"
#include "ir_send.hpp"


class runGameController : public rtos::task<>{
    
private:
    buzzer & bz;
    ir_send & encoder;
 
 
    struct playerInfo{
        int playerNR;
        int gunNR;
    };
    
    rtos::channel<playerInfo, 1024> playerInfoQueue;
    
    void main() override {
        while(1){
            bz.GameOverSound();
            hwlib::wait_ms(6000);
            bz.HitSound();
            hwlib::wait_ms(6000);
            bz.GotKilledSound();
            hwlib::wait_ms(6000);
            bz.LastMinuteSound();
            hwlib::wait_ms(6000);
            bz.YouKilledSound();
            hwlib::wait_ms(6000);
        }
    }
    
public:
    runGameController(buzzer & bz, ir_send & encoder):
    task(7, "runGameTask"),
    bz (bz),
    encoder ( encoder ),
    playerInfoQueue(this, "playerInfoQueue")
    {}
    
    void sendPlayerInfo(int playerNR, int gunNR){
        playerInfo pi{playerNR, gunNR};
        playerInfoQueue.write(pi);
    }

    
    
    
};


#endif