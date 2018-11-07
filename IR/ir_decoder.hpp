/**
 * @file
 * @brief     ir_decoder.
 *	An RTOS Task that handles incoming messages resulting in passing on only the correct ir-patterns
 *	This task periodically checks if there is a ir-pattern available, and if there is, it transmits a correct message to the runGameController.
 *	
 * @author    Vera Schoonderwoerd
 * @license   Boost
 */


#ifndef _IR_DECODER_HPP
#define _IR_DECODER_HPP


#include "hwlib.hpp"
#include "rtos.hpp"
#include "runGameController.hpp"


class ir_decoder: public rtos::task<>
{
private:
	
	struct value
	{
		uint16_t r;
	};
	
	rtos::channel<value, 1024> irReceiveQueue;
    
    /**
	 * @brief collection of the STATES we can be in.
	 */
    enum class STATE {IDLE, DECODING};
    /**
	 * @brief our current state
	 */
	enum STATE state;
    
    /**
	 * @brief the destination of our (correct) messages
	 */
	runGameController & rGC;
    /**
	 * @brief the polling rate
	 */
    int delay;
    
    /**
	 * @brief function to check if the player- and datapattern meets the XOR pattern
     * @details This function checks if the player- and datapattern meets the XOR pattern. If not, it wil return false. If the pattern is correct, the return is true
     * @param uint16_t d
	 */
    bool verifyXOR(uint16_t d);
    
    /**
	 * @brief function to send correct messages to runGameC
     * @details This function checks if the player- and datapattern meets the XOR pattern. If not, it wil retrieve the second (backup) pattern and will compare this one
     * if it is still not correct, the pattern is deleted and the return will be false.
     * @param uint16_t d
	 */
    void sendPattern(uint16_t pattern);
    
	void main( void );
	
public:
	ir_decoder(runGameController & rGC, int delay);
	void addPattern(const uint16_t & firstSet);

};

#endif