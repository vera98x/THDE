/**
 * @file
 * @brief     runGameController.
 *
 *	An RTOS Task that handles incoming flags and send an ir-pattern.
 *	This task periodically checks if there is a flag and if there is, transmits sends al ir-pattern.
 *	
 * @author    Gianluca Piccardo and Vera Schoonderwoerd
 * @license   Boost
 */

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
    
    /**
	 * @brief buzzer to manage the sounds
	 */
    buzzer & bz;
    
    /**
	 * @brief encoder to set the ir-pattern
	 */
    ir_send & encoder;
    
    /**
	 * @brief buzzer to manage the visuals
	 */
	OLEDcontroller & display;
    
    /**
	 * @brief struct of the info from our ir-data
	 */
    struct playerInfo{
        uint8_t playerNR;
        uint8_t dmg;
    };
    
    /**
	 * @brief contains the value of our max hp
	 */
    int HP_total;
    
    /**
	 * @brief contains our current HP, if HP is 0, the HP will be HP_total again
	 */
    int HP;
    
    /**
	 * @brief Channel to hold the playerInfo we will be processing
	 */
    rtos::channel<playerInfo, 1024> playerInfoQueue;
   
    /**
	 * @brief Channel to hold the commands from the wifiControl
	 */
    rtos::channel<msg, 1024> cmdChannelIn;
    
    /**
	 * @brief collection of the STATES we can be in.
	 */
    enum class STATE {STARTUP, RUNNING, DEAD, GAMEOVER};
	
    /**
	 * @brief our current state
	 */
    enum STATE state;
    
    /**
	 * @brief timer to check if the timeout (e.g. timeout for killed and gameover)
	 */
    rtos::timer timeout_timer;
    
    /**
	 * @brief duration of our gameoverTime
	 */
    int gameoverTime = 6000000;
    
    /**
	 * @brief duration of our killedTime
	 */
    int killedTime = 6000000;
    
    /**
	 * @brief commandListener, to give a signal about the game to the server
	 */
    commandListener * cl;
    
    /**
	 * @brief body of our task. Initializes the game, checks if you are dead, and stops the game
     * @details
	 */
    void main();
    
public:
    
    /**
	 * @brief Constructor of the runGameController.
	 * @param buzzer bz
	 * @param ir_send encoder
     * @param OLEDcontroller display
	 * @param commandListener * cl
	 */
    runGameController(buzzer & bz, ir_send & encoder, OLEDcontroller & display, commandListener * cl = nullptr);
    
    /**
	 * @brief Writes the received message to the to playerInfoQueue to be processed whenever the task gets to it, at a certain time
	 * @param uint8_t playerNR
     * @param uint8_t dmg
	 */
    void sendPlayerInfo(uint8_t playerNR, uint8_t dmg);
    
    /**
	 * @brief Writes the received message to the to cmdChannelIn to be processed whenever the task gets to it, at a certain time
	 * @param msg m
	 */
    void commandReceived(const msg & m);
    
    /**
	 * @brief setter for a commandListener
	 * This is to resolve the circular dependency between the WifiControl and the runGameControl, where they both listen to each other
	 * By having a setter, we can instantiate this class before the runGameController and set a pointer to it later.
	 * @param cl_def
	 */
    void setListener(commandListener * cl_def);

    
    
    
};


#endif