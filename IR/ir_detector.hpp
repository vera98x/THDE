#include "hwlib.hpp"
#include "rtos.hpp"
#include "ir_decoder.hpp"

class ir_detector: public rtos::task<>
{
private:
	hwlib::target::pin_in & detector;
	ir_decoder & decoder;
	bool firstSet [16]={};
	bool secondSet [16]={};
	enum class STATE {WAITING, MESSAGING};
	enum STATE state;
	int counter = 0;
	int timerValue = 100;
	

	void main( void ) override
	{
		for(;;)
		{
			switch(state)
			{
				case STATE::WAITING:
					if(detector.get() == 0)
					{
						counter++;
						firstSet[0] = 1;
						timerValue = 1000;
						state = STATE::MESSAGING;
					}
					break;
				
				case STATE::MESSAGING:
					if(counter / 16 == 0)
					{
						firstSet[counter] = !detector.get();
					}
					else
					{
						secondSet[counter-16] = !detector.get();
					}
					if (counter == 15)
					{
						hwlib::wait_us(2980);
					}
					counter++;
					
					if(counter >= 31)
					{
						decoder.setChannel(firstSet, secondSet);
						for(int i = 0; i < 16; i++)
						{
							hwlib::cout<<firstSet[i]<< " /n";
						}
						hwlib::cout << " ================================";
						for(int i = 0; i < 16; i++)
						{
							hwlib::cout<<secondSet[i]<< " /n";
						}
						counter = 0;
						timerValue = 100;
						state = STATE::WAITING;
					}
					break;
			}
			hwlib::wait_us(timerValue);
		}
	}


public:
ir_detector(ir_decoder & decoder, hwlib::target::pin_in & detector):
	task(1, "detector_task"),
	decoder ( decoder ),
	detector(detector),
	state(STATE::WAITING)
	{}
	


};