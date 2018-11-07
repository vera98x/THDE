/**
 * @file
 * @brief     button task.
 *
 *	An RTOS Task that handles incoming buttonpresses and gives a message to a buttonListener
 *	This task periodically checks if the button is pressed and if there is, it gives a message to the listener.
 * 
 * @author    Gianluca Piccardo
 * @license   Boost
 */


#ifndef BUTTON_HPP
#define BUTTON_HPP

#include "hwlib.hpp"
#include "rtos.hpp"
#include "buttonListener.hpp"

class button : public rtos::task<>
{
private:
	/**
	 * @brief Pin to check
	 */
    hwlib::target::pin_in & gunTrigger;
	/**
	 * @brief an int to check if the button was pressed before, so there will be one message per buttonpress
	 */
    int previousShot = 0;
	
    /**
	 * @brief our buttonListener, to give a signal that the button is pressed
	 */
    buttonListener & transmitter;
	
	void main ( void );
public:

	/**
	 * @brief Constructor of the button.
	 * @param hwlib::target::pin_in gunTrigger
	 * @param buttonListener transmitter
	 */
    button(hwlib::target::pin_in & gunTrigger, buttonListener & transmitter);

};


#endif