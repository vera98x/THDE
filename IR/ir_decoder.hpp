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
    
    
    bool verifyXOR(uint16_t d){
        for(unsigned int i=0; i<5; i++)
        {
            if( (((d << (15-(6+i))) & 1)^( (d << (15-(1+i))) & 1)) != ((d << (15-(11+i))) & 1) )
            {
                //hwlib::cout<< 1+i << " and " << 6+i << " are not XOR" << '\n';
                return false;
            }
        }
        return true;
    }
    
	
	void main( void ) override
	{
        uint16_t firstPattern;
		for(;;)
		{
			switch(state)
			{
				case STATE::IDLE:
                    firstPattern = irReceiveQueue.read().r;
                    state = STATE::DECODING;
                    break;
                
                case STATE::DECODING:
                                    
                    if (verifyXOR(firstPattern)){
                        auto done = wait(irReceiveQueue);
                        if (done == irReceiveQueue){
                            hwlib::cout << "cleaaaar \n";
                            auto r = irReceiveQueue.read();
                            (void) r;
                            timeout_timer.cancel();
                        } 
                        hwlib::cout << hwlib::_setbase(2);
                        hwlib::cout<< "bits: " << firstPattern << '\n';
                        hwlib::cout << hwlib::_setbase(10);
                        int playerNR = firstPattern >> 10;
                        int gunNR = (firstPattern >> 5) & 31; //11111
                        rGC.sendPlayerInfo(playerNR, gunNR);
                        state = STATE::IDLE;
                        break;
                    } else{
                        hwlib::cout << "false xor";
                        uint16_t secondPattern = irReceiveQueue.read().r;
                        if (verifyXOR(secondPattern)){
                            int playerNR = secondPattern >> 10;
                            int gunNR = (secondPattern >> 5) & 31; //11111
                            rGC.sendPlayerInfo(playerNR, gunNR);
                        }
                        
                    }            
                    state = STATE::IDLE;
                    break;
            }
            
            hwlib::wait_us(delay);
			
		}
	}
	
public:
	ir_decoder(runGameController & rGC, int delay):
	task(4, "ir_decoder"),
	irReceiveQueue(this, "irChannel"),
    state(STATE::IDLE),
    rGC (rGC),
    delay (delay),
    timeout_timer (this, "timeout_timer")
	{}
	
	void addPattern(const uint16_t & firstSet)
	{
		value f {firstSet};
		irReceiveQueue.write(f);
	}

};

#endif