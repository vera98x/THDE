#ifndef BUTTON_HPP
#define BUTTON_HPP

#include "hwlib.hpp"
#include "rtos.hpp"
#include "ir_send.hpp"

class button : public rtos::task<>
{
private:
	hwlib::target::pin_in & gunTrigger;
	int previousShot = 0;
	
	rtos::flag gunShotFlag;
    ir_send & transmitter;
	
	void main ( void )
	{
		for(;;)
		{
			if(gunTrigger.get()==0 && previousShot == 0)
			{
				previousShot = 1;
				hwlib::cout<< "B" << previousShot << '\n';
				transmitter.shootGun();
			}
			if(gunTrigger.get()==1 && previousShot == 1)
			{
				previousShot = 0;
			}
			hwlib::wait_ms(5);
		}
	}
	
public:
	button(hwlib::target::pin_in & gunTrigger, ir_send & transmitter):
		task(3, "buttonTask"),
		gunTrigger(gunTrigger),
		gunShotFlag(this, "gunShotFlag"),
        transmitter (transmitter)
		{}

};


#endif