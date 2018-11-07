/**
 * @file
 * @brief     Buzzer task.
 *
 *	An RTOS Task that handles incoming messages resulting in making a sound
 *	This task periodically checks if there is a flag available and if there is, it wil produce a sound.

 * @author    Vera Schoonderwoerd and Gianluca Piccardo
 * @license   Boost
 */


#ifndef _BUZZER_HPP
#define _BUZZER_HPP


#include "hwlib.hpp"
#include "rtos.hpp"
#include "note.hpp"


class buzzer : public rtos::task<>{
    
private:
    /**
	 * @brief Pin_out of the buzzer
	 */
    hwlib::target::pin_out & lsp;
    
    /**
	 * @brief Flag to make a sound when you are killed
	 */
    rtos::flag gotKilledFlag;
    /**
	 * @brief Flag to make a sound when you are gameover
	 */
    rtos::flag gameOverFlag;
    
    /**
	 * @brief Flag to make a sound when you have one minute left in the game
	 */
    rtos::flag lastMinuteFlag;
    
    /**
	 * @brief Flag to make a sound when you are hit
	 */
    rtos::flag hitFlag;
    
    /**
	 * @brief Flag to make a sound when you have killed someone
	 */
    rtos::flag killFlag;
    
    /**
	 * @brief Function to create the sounds
     * @details This function reads from note the duration and the frequency and produces on the pin_out high and low signals
     * @param note n
	 */
    
    void play( const note & n );
    
    /**
	 * @brief Function to play a sound when you are killed
	 */
    void playGotKilledSound();
    
     /**
	 * @brief Function to play a sound when you have one minute left 
	 */
    void playLastMinuteSound();
    
     /**
	 * @brief Function to play a sound when you got hit
	 */
    void playHitSound();
    
     /**
	 * @brief Function to play a sound when you are gameover
	 */
    void playGameOverSound();
    
     /**
	 * @brief Function to play a sound when you have killed someone
	 */
    void playYouKilledSound();
    
     /**
	 * @brief The body of the function. It checks which flag is set, then will play the corresponding song
	 */
    void main();
    
public:
    
    /**
	 * @brief Constructor of the buzzer.
	 * @param hwlib::target::pin_out lsp
	 */
    buzzer(hwlib::target::pin_out & lsp);
    
    /**
	 * @brief Function to set a flag
     * @details This function sets the gotKilledFlag, later the task can play the sound due to the flag
	 */
    void gotKilledSound();
    
    /**
	 * @brief Function to set a flag
     * @details This function sets the lastMinuteFlag, later the task can play the sound due to the flag
	 */
    void lastMinuteSound();
    
    /**
	 * @brief Function to set a flag
     * @details This function sets the hitFlag, later the task can play the sound due to the flag
	 */
    void hitSound();
    
    /**
	 * @brief Function to set a flag
     * @details This function sets the gameOverFlag, later the task can play the sound due to the flag
	 */
    void gameOverSound();
    
    /**
	 * @brief Function to set a flag
     * @details This function sets the killFlag, later the task can play the sound due to the flag
	 */
    void youKilledSound();
    
    
    
};


#endif