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
    enum class STATE {IDLE, DECODING};
	enum STATE state;
	runGameController & rGC;
    int delay;
    rtos::timer timeout_timer;
    
    
    bool verifyXOR(uint16_t d);
    
    void sendPattern(uint16_t pattern);
	void main( void );
	
public:
	ir_decoder(runGameController & rGC, int delay);
	void addPattern(const uint16_t & firstSet);

};

#endif