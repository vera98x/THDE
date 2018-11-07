/**
 * @file
 * @brief     ir_send task.
 *
 *	An RTOS Task that handles incoming flags and send an ir-pattern.
 *	This task periodically checks if there is a flag and if there is, transmits sends al ir-pattern.
 *	
 * @author    Vera Schoonderwoerd
 * @license   Boost
 */

#ifndef _IR_SEND_HPP
#define _IR_SEND_HPP


#include "hwlib.hpp"
#include "rtos.hpp"
#include "buttonListener.hpp"

class ir_send : public buttonListener,  public rtos::task<> {
private:
    
     /**
	 * @brief Pin_in of the ir-encoder. This is a 36kHz transmitter
	 */
    hwlib::target::d2_36kHz & encoder;
    /**
	 * @brief The ir-pattern that will be send
	 */
    bool values[16] = {};
    
    /**
	 * @brief Flag to alert that a ir-pattern schould be sent
	 */
    rtos::flag sendIRFlag;
    
    /**
	 * @brief Flag to alert that no shots may be made
	 */
    rtos::flag enableFlag;
    /**
	 * @brief Flag to alert that shots are allowed to made
	 */
    rtos::flag disableFlag;
    
    
    /**
	 * @brief collection of the STATES we can be in.
	 */
    enum class STATE {ENABLED, DISABLED};
    
    /**
	 * @brief our current state
	 */
	enum STATE state;
    
    /**
	 * @brief Body of the task. It checks if a flag is set. If sendIRFlag is set, a ir-pattern will be send twice.
	 */
    void main();
        
	
public:
	 
     /**
	 * @brief Constructor of the ir_send
	 * @param hwlib::target::d2_36kHz  encoder
	 */
    ir_send(hwlib::target::d2_36kHz & encoder);
    
    /**
	 * @brief Sets the ir-pattern
	 * @param uint8_t sp
     * @param uint8_t dmg
	 */
    void setIrpattern(uint8_t sp, uint8_t dmg);
    
    /**
	 * @brief Function to set a flag
     * @details This function sets the sendIRFlag. The taks can process this at a certain time
	 */
    void shootGun() override;
    
     /**
	 * @brief Function to set a flag
     * @details This function sets the enableFlag. The taks can process this at a certain time
	 */
    void enable();
    
     /**
	 * @brief Function to set a flag
     * @details This function sets the disableFlag. The taks can process this at a certain time
	 */
    void disable();
    
};

#endif