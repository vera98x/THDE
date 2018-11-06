#ifndef _IR_SEND_HPP
#define _IR_SEND_HPP


#include "hwlib.hpp"
#include "rtos.hpp"


class ir_send : public rtos::task<>{
private:
    hwlib::target::d2_36kHz & encoder;
    bool values[16] = {};
    rtos::flag sendIRFlag;
    rtos::flag enableFlag;
    rtos::flag disableFlag;
    
    enum class STATE {ENABLED, DISABLED};
	enum STATE state;
    
    void main();
        
	
public:
	ir_send(hwlib::target::d2_36kHz & encoder);
    
    void setIrpattern(uint8_t sp, uint8_t dmg);
    
    void shootGun();
    
    void enable();
    void disable();
    
};

#endif