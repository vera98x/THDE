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
	uint16_t firstPattern;
	runGameController & rGC;
    int delay;
    
    
    bool verifyXOR(uint16_t d){
        for(unsigned int i=0; i<5; i++)
        {
            if( (((firstPattern << (15-(6+i))) & 1)^( (firstPattern << (15-(1+i))) & 1)) != ((firstPattern << (15-(11+i))) & 1) )
            {
                //hwlib::cout<< 1+i << " and " << 6+i << " are not XOR" << '\n';
                return false;
            }
        }
        return true;
    }
    
	
	void main( void ) override
	{
		for(;;)
		{
			switch(state)
			{
				case STATE::IDLE:
                    firstPattern = irReceiveQueue.read().r;
                    state = STATE::DECODING;
                    break;
                
                case STATE::DECODING:
                    hwlib::cout << hwlib::_setbase(2);
                    hwlib::cout<< "bits: " << firstPattern << '\n';
                    hwlib::cout << hwlib::_setbase(10);
                    
                    if (verifyXOR(firstPattern)){
                        int playerNR = firstPattern >> 10;
                        int gunNR = (firstPattern >> 5) & 31; //11111
                        
                        rGC.sendPlayerInfo(playerNR, gunNR);
                        
                    }
                    
                    state = STATE::IDLE;
                    break;
            }
            
            hwlib::wait_ms(delay);
			
		}
	}
	
public:
	ir_decoder(runGameController & rGC, int delay):
	task(4, "ir_decoder"),
	irReceiveQueue(this, "irChannel"),
    state(STATE::DECODING),
    rGC (rGC),
    delay (delay)
	{}
	
	void addPattern(const uint16_t & firstSet)
	{
		value f {firstSet};
		irReceiveQueue.write(f);
	}

};

#endif