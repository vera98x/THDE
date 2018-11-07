
#include "hwlib.hpp"
#include "rtos.hpp"
#include "runGameController.hpp"
#include "ir_decoder.hpp"

bool ir_decoder::verifyXOR(uint16_t d){
    for(unsigned int i=0; i<5; i++)
    {
        if( (((d >> (15-(6+i))) & 1)^( (d>>(15-(1+i))) & 1)) != ((d>>(15-(11+i))) & 1) )
        {
            //hwlib::cout<< 1+i << " and " << 6+i << " are not XOR" << '\n';
            return false;
        }
    }
    return true;
}
    
void ir_decoder::sendPattern(uint16_t pattern){
    uint8_t playerNR = (pattern >> 10) & 31;
    uint8_t gunNR = (pattern >> 5) & 31; //11111
    rGC.sendPlayerInfo(playerNR, gunNR);
}
	
void ir_decoder::main( void )
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
                    } 
                    hwlib::cout << hwlib::_setbase(2);
                    hwlib::cout<< "bits: " << firstPattern << '\n';
                    hwlib::cout << hwlib::_setbase(10);
                    sendPattern(firstPattern);
                    state = STATE::IDLE;
                    break;
                } else{
                    uint16_t secondPattern = irReceiveQueue.read().r;
                    if (verifyXOR(secondPattern)){
                        sendPattern(secondPattern);
                        hwlib::cout << "second worked ! \n";
                    }
                        
                }            
                state = STATE::IDLE;
                break;
        }
            
        hwlib::wait_us(delay);
	
    }
}
	

ir_decoder::ir_decoder(runGameController & rGC, int delay):
	task(4, "ir_decoder"),
	irReceiveQueue(this, "irChannel"),
    state(STATE::IDLE),
    rGC (rGC),
    delay (delay)
	{}
	
void ir_decoder::addPattern(const uint16_t & firstSet)
{
    value f {firstSet};
    irReceiveQueue.write(f);
}

