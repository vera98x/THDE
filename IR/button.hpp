#ifndef BUTTON_HPP
#define BUTTON_HPP

#include "hwlib.hpp"
#include "rtos.hpp"
#include "buttonListener.hpp"

class button : public rtos::task<>
{
private:
	hwlib::target::pin_in & gunTrigger;
	int previousShot = 0;
	
	rtos::flag gunShotFlag;
    buttonListener & transmitter;
	
	void main ( void );
public:
	button(hwlib::target::pin_in & gunTrigger, buttonListener & transmitter);

};


#endif