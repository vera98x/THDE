/**
 * @file
 * @brief     ir_detector.
 *
 *	An RTOS Task that handles incoming ir-signals
 *	This task periodically checks if there is a ir-signal and if there is, transmits the message to the ir_decoder.
 *	
 * @author    Gianluca Piccardo
 * @license   Boost
 */


#ifndef _IR_DETECTOR_HPP
#define _IR_DETECTOR_HPP


#include "hwlib.hpp"
#include "rtos.hpp"
#include "ir_decoder.hpp"

class ir_detector: public rtos::task<>
{
private:
	
     /**
	 * @brief Pin_in of the ir-detector
	 */
    hwlib::target::pin_in & receivePin;
    
     /**
	 * @brief decoder to send the ir-message to
	 */
	ir_decoder & decoder;
    
     /**
	 * @brief our ir-pattern
	 */
	uint16_t firstSet = 0;
    
    /**
	 * @brief collection of the STATES we can be in.
	 */
	enum class STATE {WAITING, MESSAGING};
    
    /**
	 * @brief our current state
	 */
	enum STATE state;
    
    /**
	 * @brief maintains our bitnumber
	 */
	int counter = 0;
    /**
	 * @brief the polling rate
	 */
	int timerValue = 100;
    
	
    /**
	 * @brief Body of the function. Register all ir-patterns and send it to the ir_decoder class
     * @details This function checks if the startbit is according to our selfmade startbit. If it is, it retrieves the message
	 */
	void main( void ) override;

public:
    /**
	 * @brief Constructor of the ir_detector.
	 * @param ir_decoder decoder
	 * @param hwlib::target::pin_in receivePin
	 */
    ir_detector(ir_decoder & decoder, hwlib::target::pin_in & receivePin);
	


};

#endif