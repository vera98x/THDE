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
	OLEDcontroller & display;
    bool values[16] = {1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0};
 
    struct playerInfo{
        int playerNR;
        int dmg;
    };
    
    int HP_total;
    int HP;
    
    rtos::channel<playerInfo, 1024> playerInfoQueue;
    rtos::channel<msg, 1024> cmdChannelIn;
    
    enum class STATE {STARTUP, RUNNING, DEAD, GAMEOVER};
	enum STATE state;
    rtos::timer timeout_timer;
    int gameoverTime = 60000;
    int killedTime = 6000;
    
    
    void main() override {
        //const char* c = "hello"; 
        uint8_t spelerID = 0;
        uint8_t dmg = 30;
        bool readyToStart = 0;
		encoder.setIrpattern(22, 0);
        while(1){
            switch(state) {
                case STATE::STARTUP:
                    {
                        msg message = cmdChannelIn.read();
                        if(message.command == message.CMD::R_PLAYER_NAME){
                            //display.showSpelernaam(message.naam);
                        } else if (message.command == message.CMD::R_PLAYER_ID){
                            spelerID = message.waarde;
                        } else if (message.command == message.CMD::R_SELECTED_DMG){
                            dmg = message.waarde/10;
                        } else if (message.command == message.CMD::R_START_GAME){
                            readyToStart = 1;
                            if (spelerID == 0){
                                //msg m = {CMD::T_REQ_PLAYERID, "", 0};
                                //wifi.send(m);
                            }
                        } else if (message.command == message.CMD::R_HP){
                            HP_total = message.waarde;
                            HP = HP_total;
                        } else if (spelerID > 0 && readyToStart){
                            encoder.setIrpattern(spelerID, dmg);
                            encoder.enable();
                            state = STATE::RUNNING;
                        } 
                    }
                    
                    break;
                    
                case STATE::RUNNING:
                    {
                        auto done = wait(playerInfoQueue + cmdChannelIn);
                        if (done == playerInfoQueue){
                            auto pi = playerInfoQueue.read();
                            auto dmg_enemy = pi.dmg;
                            HP -= dmg_enemy*10;
                            
                            if (HP <= 0){
                                //msg m = {CMD::T_KILLED_BY, "", pi.playerNR};
                                //wifi.send(m);
                                encoder.disable();
                                HP = HP_total;                               
                                timeout_timer.set( killedTime );
                                state = STATE::DEAD;
                            } else {
                                bz.hitSound();
                                display.showHPchanged(HP);
                                
                            }
                        } else if (done == cmdChannelIn){
                            auto cmd_msg = cmdChannelIn.read();
                            if (cmd_msg.command == cmd_msg.CMD::R_GAME_OVER){
                                timeout_timer.set( gameoverTime );
                                state = STATE::GAMEOVER;
                            } else if (cmd_msg.command == cmd_msg.CMD::R_LAST_MINUTE){
                                bz.lastMinuteSound();
                                display.showOneMinute();
                            } else if (cmd_msg.command == cmd_msg.CMD::R_KILL_CONFIRM){
                                bz.youKilledSound();
                            }
                        }
                    }
                    
                    break;
                    
                case STATE::DEAD:
                    {
                        bz.gotKilledSound();
                        auto done = wait(timeout_timer + playerInfoQueue + cmdChannelIn);
                        if (done == playerInfoQueue){
                            playerInfoQueue.clear();
                        } else if (done == cmdChannelIn) {
                            msg cmd_msg = cmdChannelIn.read();
                            if (cmd_msg.command == cmd_msg.CMD::R_GAME_OVER){
                                timeout_timer.set( gameoverTime );
                                encoder.disable();
                                display.showGameOver();
                                bz.gameOverSound();
                                state = STATE::GAMEOVER;
                            } else if (cmd_msg.command == cmd_msg.CMD::R_LAST_MINUTE){
                                bz.lastMinuteSound();
                                display.showOneMinute();
                            } else if (cmd_msg.command == cmd_msg.CMD::R_KILLED_BY){
                                display.showKiller(cmd_msg.naam);
                            }
                        } else {
                            encoder.enable();
                            state = STATE::RUNNING;
                        }
                    }
                    break;
                
                case STATE::GAMEOVER:
                    auto done = wait(timeout_timer);
                    if (done == timeout_timer){
                        state = STATE::STARTUP;
                    }
                    break;
            }
        }
    }
    
public:
    runGameController(buzzer & bz, ir_send & encoder, OLEDcontroller & display):
    task(7, "runGameTask"),
    bz (bz),
    encoder ( encoder ),
	display(display),
    HP (100),
    playerInfoQueue(this, "playerInfoQueue"),
    cmdChannelIn(this, "cmdChannelIn"),
    state(STATE::STARTUP),
    timeout_timer (this, "timeout_timer")
    {}
    
    void sendPlayerInfo(int playerNR, int dmg){
        playerInfo pi{playerNR, dmg};
        playerInfoQueue.write(pi);
    }
    
    void receiveCmd(msg m){
        cmdChannelIn.write(m);
    }

    
    
    
};


#endif