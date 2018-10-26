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
	
	
	
	void main( void ) override
	{
		for(;;)
		{
			uint16_t firstPattern = irReceiveQueue.read().r;
			for(unsigned int i=0; i<5; i++)
			{
				if( (((firstPattern << (15-(6+i))) & 1)^( (firstPattern << (15-(1+i))) & 1)) != ((firstPattern << (15-(11+i))) & 1) )
				{
					hwlib::cout<< 1+i << " and " << 6+i << " are not XOR" << '\n';
				}
			}
			hwlib::cout << " xor done, its okay \n";
		}
	}
	
public:
	ir_decoder():
	task(4, "ir_decoder"),
	irReceiveQueue(this, "irChannel")
	{}
	
	void setChannel(const uint16_t & firstSet, const uint16_t  & secondSet)
	{
		value f {firstSet};
		value s {secondSet};
		irReceiveQueue.write(f);
		irReceiveQueue.write(s);
	}

};
