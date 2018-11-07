/**
 * @file
 * @brief     Wifi connection task.
 *
 *	An RTOS Task that handles incoming and outgoing communication with an ESP12-E through a UART interface
 *	This task periodically checks if there is a character available on the given UART interface and if there is, transmits the message to a given listener.
 *	This task will also periodically check if there is a message waiting to be sent in the Queue, if there is, it gets sent through UART to the ESP12-E to be transmitted to the game manager
 *
 * @author    Florian Humblot
 * @license   Boost
 */

#ifndef UART_WIFITAAK_HPP
#define UART_WIFITAAK_HPP

#include "hwlib.hpp"
#include "rtos.hpp"
#include "msg.hpp"
#include "hardware_usart.hpp"
#include "commandListener.hpp"

class WifiControl : rtos::task<>, public commandListener {
private:
	/**
	 * @brief Channel to hold the commmands we will be sending to the ESP
	 */
	rtos::channel<msg, 10> cmdChannelOut;
	/**
	 * @brief Uart connection with the ESP
	 */
	UARTLib::HardwareUART &wifi_chip;
	/**
	 * @brief Timer for escaping a wait on the message queue in case it's empty.
	 */
	rtos::timer waiting_timeout;
	/**
	 * @brief the listener that will get notified whenever we have a new message.
	 */
	commandListener *cl;

	/**
	 * @brief collection of the STATES we can be in.
	 */
	enum class STATE {
		LISTENING,
		SENDING,
		WAITING

	};

	/**
	 * @brief our current state
	 */
	STATE state;

	/**
	 * @brief actual body of the task, checks if there are UART chars available or commands.
	 * If there are characters available on UART, it will put them in a msg object and send them to the commandListener.
	 * If there is a command in the cmdChannelOut, it will convert the message to a string and send it over uart.
	 */
	void main() override ;

public:

	/**
	 * @brief Constructor of the WifiControl.
	 * Marked explicit to avoid conversions done by the compiler when only one argument is supplied.
	 * @param ESP
	 * @param cl*
	 */
	explicit WifiControl(UARTLib::HardwareUART &ESP, commandListener *cl = nullptr);

	/**
	 * @brief Implementation of the commandListener interface
	 * Writes the received message to the to cmdChannelOut to be processed whenever the task gets to that point.
	 * @param m msg
	 */
	void commandReceived(const msg &m) override;

	/**
	 * @brief setter for a commandListener
	 * This is to resolve the circular dependency between the WifiControl and the runGameControl, where they both listen to each other
	 * By having a setter, we can instantiate this class before the runGameController and set a pointer to it later.
	 * @param cl_def
	 */
	void setListener(commandListener *cl_def);

};


#endif //UART_WIFITAAK_HPP
