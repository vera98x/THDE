#include "hwlib.hpp"
#include "rtos.hpp"
#include <bitset>


class ir_decoder: public rtos::task<>
{
private:
	rtos::channel<uint16_t, 1024> irReceiveQueue;
	
	void main( void ) override
	{
		for(;;)
		{
			uint16_t firstPattern = irReceiveQueue.read();
			for(unsigned int i=0; i<5 i++)
			{
				if( (((firstPattern << 15-6+i) & 1)^( (firstPattern << 15-1+i) & 1)) != ((firstPattern << 15-11+i) & 1) )
				{
					hwlib::cout<< 1+i << " and " << 6+i << " are not XOR" << '\n';
				}
			}
		}
	}
	
public:
	ir_detector():
	task(4, "ir_decoderTask")
	{}
	
	void setChannel(const uint16_t & firstSet, const uint16_t  & secondSet)
	{
		irPattern.write(firstSet);
		irPattern.write(secondSet);
	}

};
