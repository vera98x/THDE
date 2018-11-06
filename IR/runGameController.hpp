#ifndef _RUNGAMECONTROLLER_HPP
#define _RUNGAMECONTROLLER_HPP


#include "hwlib.hpp"
#include "rtos.hpp"
#include "buzzer.hpp"
#include "ir_send.hpp"
#include "OLEDcontroller.hpp"
#include "msg.hpp"
#include "commandListener.hpp"

class runGameController : public rtos::task<>, public commandListener{
    
private:
    buzzer & bz;
    ir_send & encoder;
	OLEDcontroller & display;
    bool values[16] = {1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0};
 
    struct playerInfo{
        uint8_t playerNR;
        uint8_t dmg;
    };
    
    int HP_total;
    int HP;
    
    rtos::channel<playerInfo, 1024> playerInfoQueue;
    rtos::channel<msg, 1024> cmdChannelIn;
    
    enum class STATE {STARTUP, RUNNING, DEAD, GAMEOVER};
	enum STATE state;
    rtos::timer timeout_timer;
    int gameoverTime = 6000000;
    int killedTime = 6000000;
    commandListener * cl;
    
    
    void main();
    
public:
    runGameController(buzzer & bz, ir_send & encoder, OLEDcontroller & display, commandListener * cl = nullptr);
    
    void sendPlayerInfo(uint8_t playerNR, uint8_t dmg);
    
    void commandReceived(const msg & m);
    void setListener(commandListener * cl_def);

    
    
    
};


#endif