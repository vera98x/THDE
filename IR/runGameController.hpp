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
    rtos::channel<playerInfo, 1024> cmdChannelIn;
    
    enum class STATE {IDLE, STARTUP, RUNNING, GAMEOVER};
	enum STATE state;
    
    
    void main() override {
        //const char* c = "hello"; 
		encoder.setIrpattern(22, 0);
        while(1){
            switch(state) {
				case STATE::IDLE:
                    //wifi.connect();
                    //if (wifi.connected){
                        //state = STATE::STARTUP()
                    //}
                    break;
                case STATE::STARTUP:
                    //if(wifi.spelernaam){
                        //window.showSpelernaam();
                    //}
                    //encoder.setIrpattern(wifi.spelerID, wifi.dmg)
                    
                    //if (wifi.startgame){
                        //state = STATE::RUNNING;
                    //}
                    break;
                    
                    
                case STATE::RUNNING:
                    encoder.enable();
                    //auto done = wait()
                    break;
                
                case STATE::GAMEOVER:
                    break;
            }
        }
    }
    
public:
    runGameController(buzzer & bz, ir_send & encoder, OLEDcontroller & window):
    task(7, "runGameTask"),
    bz (bz),
    encoder ( encoder ),
	window(window),
    playerInfoQueue(this, "playerInfoQueue"),
    cmdChannelIn(this, "cmdChannelIn"),
    state(STATE::IDLE)
    {}
    
    void sendPlayerInfo(int playerNR, int gunNR){
        playerInfo pi{playerNR, gunNR};
        playerInfoQueue.write(pi);
    }
    
    /*void sendCmd(){
        cmdChannelIn.write();
    }*/

    
    
    
};


#endif