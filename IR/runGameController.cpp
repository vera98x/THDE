
#include "hwlib.hpp"
#include "rtos.hpp"
#include "buzzer.hpp"
#include "ir_send.hpp"
#include "OLEDcontroller.hpp"
#include "msg.hpp"
#include "commandListener.hpp"
#include "runGameController.hpp"

    
    void runGameController::main() {
        uint8_t spelerID = 0;
        uint8_t dmg = 3;
        bool readyToStart = 0;
        while(1){
            switch(state) {
                case STATE::STARTUP:
                    {
                        msg message = cmdChannelIn.read();
                        if(message.command == message.CMD::R_PLAYER_NAME){
                            hwlib::cout<< "R_PLAYER_NAME \n";
                            display.showYourName(message.naam);
                        } else if (message.command == message.CMD::R_PLAYER_ID){
                            spelerID = message.waarde;
                            hwlib::cout<< "R_PLAYER_ID \n";
                        } else if (message.command == message.CMD::R_SELECTED_DMG){
                            dmg = message.waarde/10;
                            hwlib::cout<< dmg << "R_SELECTED_DMG \n";
                        } else if (message.command == message.CMD::R_START_GAME){
                            hwlib::cout<< "R_START_GAME \n";
                            readyToStart = 1;
                            if (spelerID == 0){
                                msg m = {};
                                m.command = msg::CMD::T_REQ_PLAYERID;
                                cl -> commandReceived(m);
                            } else {
                                encoder.setIrpattern(spelerID, dmg);
                                encoder.enable();
								display.showHPchanged(HP);
                                hwlib::cout << "running!!";
                                state = STATE::RUNNING;
                            }
                        } else if (message.command == message.CMD::R_HP){
                            hwlib::cout<< "R_HP \n";
                            HP_total = message.waarde;
                            HP = HP_total;
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
                                hwlib::cout<< "killed!";
                                msg m = {};
                                m.command = msg::CMD::T_KILLED_BY;
                                m.waarde = pi.playerNR;
                                cl -> commandReceived(m);
                                encoder.disable();
                                HP = HP_total;                               
                                timeout_timer.set( killedTime );
                                state = STATE::DEAD;
                            } else if (dmg > 0) {
                                bz.hitSound();
                                display.showHPchanged(HP);
                                
                            }
                        } else if (done == cmdChannelIn){
                            auto cmd_msg = cmdChannelIn.read();
                            if (cmd_msg.command == cmd_msg.CMD::R_GAME_OVER){
                                hwlib::cout<< "R_GAME_OVER \n";
                                timeout_timer.set( gameoverTime );
                                encoder.disable();
                                display.showGameOver();
                                state = STATE::GAMEOVER;
                            } else if (cmd_msg.command == cmd_msg.CMD::R_LAST_MINUTE){
                                hwlib::cout<< "R_LAST_MINUTE \n";
                                bz.lastMinuteSound();
                                display.showOneMinute();
                            } else if (cmd_msg.command == cmd_msg.CMD::R_KILL_CONFIRM){
                                bz.youKilledSound();
                            }
                        }
                    }
                    hwlib::wait_us(100);
                    break;
                    
                case STATE::DEAD:
                    {
                        bz.gotKilledSound();
                        auto done = wait(timeout_timer + cmdChannelIn);
                        if (done == cmdChannelIn) {
                            msg cmd_msg = cmdChannelIn.read();
                            if (cmd_msg.command == cmd_msg.CMD::R_GAME_OVER){
                                hwlib::cout<< "R_GAME_OVER \n";
                                timeout_timer.set( gameoverTime );
                                encoder.disable();
                                display.showGameOver();
                                bz.gameOverSound();
                                state = STATE::GAMEOVER;
                            } else if (cmd_msg.command == cmd_msg.CMD::R_LAST_MINUTE){
                                hwlib::cout<< "R_LAST_MINUTE \n";
                                bz.lastMinuteSound();
                                display.showOneMinute();
                            } else if (cmd_msg.command == cmd_msg.CMD::R_KILLED_BY){
                                display.showKiller(cmd_msg.naam);
                            }
                        } else {
                            hwlib::cout << "alive!!";
                            encoder.enable();
                            playerInfoQueue.clear();
                            display.showHPchanged(HP);
                            state = STATE::RUNNING;
                        }
                    }
                    break;
                
                case STATE::GAMEOVER:
                    auto done = wait(timeout_timer);
                    if (done == timeout_timer){
                        hwlib::cout << "startUP";
                        state = STATE::STARTUP;
                        encoder.enable();
                    }
                    break;
                    
            }
        }
    }
    

    runGameController::runGameController(buzzer & bz, ir_send & encoder, OLEDcontroller & display, commandListener * cl):
        task(7, "runGameTask"),
        bz (bz),
        encoder ( encoder ),
        display(display),
        HP_total (100),
        HP (100),
        playerInfoQueue(this, "playerInfoQueue"),
        cmdChannelIn(this, "cmdChannelIn"),
        state(STATE::STARTUP),
        timeout_timer (this, "timeout_timer"),
        cl (cl)
        {}
    
    void runGameController::sendPlayerInfo(uint8_t playerNR, uint8_t dmg){
        playerInfo pi{playerNR, dmg};
        playerInfoQueue.write(pi);
    }
    
    void runGameController::commandReceived(const msg & m){
        cmdChannelIn.write(m);
    }
    
    void runGameController::setListener(commandListener * cl_def){
        cl = cl_def;
    }

