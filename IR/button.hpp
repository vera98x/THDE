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
	
	void main ( void );
public:
	button(hwlib::target::pin_in & gunTrigger, ir_send & transmitter);

};


#endif