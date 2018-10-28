#include "hwlib.hpp"
#include "rtos.hpp"
#include "ir_decoder.hpp"

class ir_detector: public rtos::task<>
{
private:
	hwlib::target::pin_in & receivePin;
	ir_decoder & decoder;
	uint16_t firstSet = 0;
	//uint16_t secondSet = 0;
	enum class STATE {WAITING, MESSAGING};
	enum STATE state;
	int counter = 0;
	int timerValue = 100;
    int step = 0;
	

	void main( void ) override
	{
		for(;;)
		{
			switch(state)
			{
				case STATE::WAITING:
                    if(receivePin.get() == 0){
                        while(receivePin.get() == 0){
                            hwlib::wait_us(100);
                        }
                        while(receivePin.get() == 1){
                            hwlib::wait_us(100);
                        }
                            counter++;
                            firstSet= 1;
                            step = 0;
                            timerValue = 1200;
                            state = STATE::MESSAGING;
                    }
                    

					break;
				
				case STATE::MESSAGING:
                    firstSet = firstSet << 1;
                    firstSet |= !receivePin.get();
					counter++;
					
					if(counter >= 16)
					{
						hwlib::cout << "new bit \n";
                        decoder.setChannel(firstSet);
						counter = 0;
						timerValue = 100;
						state = STATE::WAITING;
					}
                    hwlib::wait_us(timerValue);
					break;
			}
			hwlib::wait_us(timerValue);
		}
	}


public:
ir_detector(ir_decoder & decoder, hwlib::target::pin_in & receivePin):
	task(1, "detector_task"),	
	receivePin(receivePin),
	decoder ( decoder ),
	state(STATE::WAITING)
	{}
	


};