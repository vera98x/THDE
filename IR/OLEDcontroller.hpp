/**
 * @file
 * @brief     OLEDcontroller task.
 *
 *	An RTOS Task that handles incoming messages resulting in making a visualization on the OLED screen.
 *	This task periodically checks if there is a flag available or a pool filled and if there is, it wil produce a sound.

 * @author    Vera Schoonderwoerd and Gianluca Piccardo
 * @license   Boost
 */

#ifndef OLEDCONTROLLER_HPP
#define OLEDCONTROLLER_HPP

#include "hwlib.hpp"
#include "rtos.hpp"
#include "hwlib-graphics.hpp"

class OLEDcontroller: public rtos::task<>
{
private:
/**
	 * @brief enables fonts to display
	 */
	hwlib::font_default_8x8 & font;
/**
	 * @brief out stream of the display
	 */
	hwlib::window_ostream & display;
/**
	 * @brief Flag to display the correct HP value on the OLED screen
	 */
	rtos::flag HPChangedflag;
/**
	 * @brief Flag to display your name to welcome the person to the match
	 */
	rtos::flag yourNameFlag;
/**
	 * @brief Flag to display the name of the person that killed you during a game
	 */
	rtos::flag killerFlag;
/**
	 * @brief Flag to display the notification that one minute is left in the game
	 */
	rtos::flag oneMinuteFlag;
/**
	 * @brief Flag to display the ending of the match
	 */
	rtos::flag gameOverFlag;
/**
	 * @brief pool to pass along the value of the current player HP
	 */
	rtos::pool <int> HPInfoPool;
/**
	 * @brief pool to pass along the name that you registered with
	 */
	rtos::pool <const char *> yourNamePool;
/**
	 * @brief pool to pass alon the name of the person that killed the player during a game
	 */
	rtos::pool <const char *> killerInfoPool;
/**
	 * @brief main body of the function, checks for flags, reads pools and displays the corresponding information on the OLED.
	 */
	void main( void );
	
public:
    /**
	 * @brief Constructor of the OLEDcontroller.
	 * @param hwlib::font_default_8x8 & font, hwlib::window_ostream & display
	 */
    OLEDcontroller(hwlib::font_default_8x8 & font, hwlib::window_ostream & display);
    /**
	 * @brief Function reads pool and flushes to OLED
     * @details This function reads the HPinfopool and out-streams it to the OLED screen
	 */
	void flushHPchanged();
    /**
	 * @brief Function to set flag and write pool
     * @details This function sets the flag and writes to the pool when the HP changes
	 * @param const int & HP
	 */
	void showHPchanged(const int & HP);
    /**
	 * @brief Function reads pool and flushes to OLED
     * @details This function reads the yourNamePool and out-streams it to the OLED screen
	 */
	void flushYourName();
    /**
	 * @brief Function to set flag and write pool
     * @details This funtion sets the flag and writes to the pool when a player registered to the game
	 * @param char array [15]
	 */
	void showYourName(char c[15]);
    /**
	 * @brief Function to display last minute
     * @details This function notifies the player that there is only one minute left of the current game
	 */
	void flushOneMinute();
    /**
	 * @brief Function to set flag
     * @details This funtion sets the flag when there is only one minute left in the current game

	 */
	void showOneMinute();
    /**
	 * @brief Function reads pool and flushes to OLED
     * @details This funtion read the pool to notify who killed another player and flushes that information to the OLED of the killed player
	 */
	void flushKiller();
    /**
	 * @brief Function to set flag and write pool
     * @details This funtion sets the flag and writes to the pool when a player got killed during the game
	 * @param const char * name
	 */
	void showKiller(const char * name);
    /**
	 * @brief Function displays information on OLED
     * @details This funtion notifies the player when the current game has ended
	 */
	void flushGameOver();
    /**
	 * @brief Function sets a flag
     * @details This funtion sets a flag when the current game has ended
	 */
	void showGameOver();
	
};
#endif //OLEDCONTROLLER_HPP