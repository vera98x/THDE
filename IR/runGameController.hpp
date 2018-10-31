#ifndef _RUNGAMECONTROLLER_HPP
#define _RUNGAMECONTROLLER_HPP


#include "hwlib.hpp"
#include "rtos.hpp"
#include "buzzer.hpp"
#include "ir_send.hpp"
#include "OLEDcontroller.hpp"
#include "msg.hpp"


class runGameController : public rtos::task<>{
    
private:
    buzzer & bz;
    ir_send & encoder;
	OLEDcontroller & window;
    bool values[16] = {1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0};
 
    struct playerInfo{
        int playerNR;
        int dmg;
    };
    
    int HP_total;
    int HP;
    
    rtos::channel<playerInfo, 1024> playerInfoQueue;
    rtos::channel<msg, 1024> cmdChannelIn;
    
    enum class STATE {IDLE, STARTUP, RUNNING, HIT, GAMEOVER};
	enum STATE state;
    
    
    void main() override {
        //const char* c = "hello"; 
        int spelerID = -1;
        int dmg = -1;
        bool readyToStart = 0;
		encoder.setIrpattern(22, 0);
        while(1){
            switch(state) {
				case STATE::IDLE:
                    /*auto wificonfirm = cmdChannelIn.read();
                    (void) wificonfirm;
                    state = STATE::STARTUP;*/
                    break;
                case STATE::STARTUP:
                    {
                        msg message = cmdChannelIn.read();
                        if(message.command == message.CMD::R_PLAYER_NAME){
                            //window.showSpelernaam(message.naam);
                        } else if (message.command == message.CMD::R_PLAYER_ID){
                            spelerID = message.waarde;
                        } else if (message.command == message.CMD::R_SELECTED_DMG){
                            dmg = message.waarde;
                        } else if (spelerID > 0 && dmg > 0){
                            encoder.setIrpattern(spelerID, dmg);
                        } else if (message.command == message.CMD::R_START_GAME){
                            readyToStart = 1;
                        } else if (spelerID > 0 && dmg > 0 && readyToStart){
                            state = STATE::RUNNING;
                        } else if (message.command == message.CMD::R_HP){
                            HP_total = message.waarde;
                            HP = HP_total;
                        }
                    }
                    
                    break;
                    
                case STATE::RUNNING:
                    {
                        encoder.enable();
                        auto done = wait(playerInfoQueue + cmdChannelIn);
                        if (done == playerInfoQueue){
                            auto pi = playerInfoQueue.read();
                            //auto p = pi.playerNR;
                            auto dmg = pi.dmg;
                            HP -= dmg;
                            if (HP <= 0){
                                //wifi.send(p);
                                HP = HP_total;
                                state = STATE::HIT;
                            }
                        } else if (done == cmdChannelIn){
                            auto cmd_msg = cmdChannelIn.read();
                            if (cmd_msg.command == cmd_msg.CMD::R_GAME_OVER){
                                state = STATE::GAMEOVER;
                            } else if (cmd_msg.command == cmd_msg.CMD::R_KILLED_BY){
                                bz.gotKilledSound();
                                window.showKiller(cmd_msg.naam);
                            } else if (cmd_msg.command == cmd_msg.CMD::R_LAST_MINUTE){
                                bz.lastMinuteSound();
                                window.showOneMinute();
                            } else if (cmd_msg.command == cmd_msg.CMD::R_KILL_CONFIRM){
                                bz.gotKilledSound();
                            }
                        }
                    }
                    
                    break;
                    
                case STATE::HIT:
                    encoder.disable();
                    hwlib::wait_ms(6000);
                    playerInfoQueue.clear();
                    //auto done = wait()
                    state = STATE::RUNNING;
                    break;
                
                case STATE::GAMEOVER:
                    encoder.disable();
                    window.showGameOver();
                    bz.gameOverSound();
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
    HP (100),
    playerInfoQueue(this, "playerInfoQueue"),
    cmdChannelIn(this, "cmdChannelIn"),
    state(STATE::IDLE)
    {}
    
    void sendPlayerInfo(int playerNR, int dmg){
        playerInfo pi{playerNR, dmg};
        playerInfoQueue.write(pi);
    }
    
    /*void sendCmd(){
        cmdChannelIn.write();
    }*/

    
    
    
};


#endif