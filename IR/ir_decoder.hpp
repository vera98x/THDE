#include "hwlib.hpp"
#include "rtos.hpp"


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
                    
                    for(unsigned int i=0; i<5; i++)
                    {
                        if( (((firstPattern << (15-(6+i))) & 1)^( (firstPattern << (15-(1+i))) & 1)) != ((firstPattern << (15-(11+i))) & 1) )
                        {
                            //hwlib::cout<< 1+i << " and " << 6+i << " are not XOR" << '\n';
                        }
                    }
                    //hwlib::cout << " xor done, its okay \n";
                    state = STATE::IDLE;
                    break;
            }
			
		}
	}
	
public:
	ir_decoder():
	task(4, "ir_decoder"),
	irReceiveQueue(this, "irChannel"),
    state(STATE::DECODING)
	{}
	
	void setChannel(const uint16_t & firstSet)
	{
		value f {firstSet};
		irReceiveQueue.write(f);
	}

};
