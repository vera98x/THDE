
#include "hwlib.hpp"
#include "rtos.hpp"
#include "ir_decoder.hpp"
#include "ir_detector.hpp"

	void ir_detector::main( void ) 
	{
        int n = 0;
		for(;;)
		{
			switch(state)
			{
				case STATE::WAITING:
                    if(receivePin.get() == 0){
                        while(receivePin.get() == 0){
                            hwlib::wait_us(100);
                            n += 100;
                        }
                        while(receivePin.get() == 1){
                            hwlib::wait_us(100);
                            n += 100;
                        }
                        if (n > 1500 && n < 2500)
                        {
                            counter++;
                            firstSet= 1;
                            timerValue = 1200;
                            state = STATE::MESSAGING;
                        }
                        n = 0;
                    }
                    

					break;
				
				case STATE::MESSAGING:
                    firstSet = firstSet << 1;
                    firstSet |= !receivePin.get();
					counter++;
					
					if(counter >= 16)
					{
                        decoder.addPattern(firstSet);
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


ir_detector::ir_detector(ir_decoder & decoder, hwlib::target::pin_in & receivePin):
	task(1, "detector_task"),	
	receivePin(receivePin),
	decoder ( decoder ),
	state(STATE::WAITING)
	{}
	
