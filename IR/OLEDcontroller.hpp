#include "hwlib.hpp"
#include "rtos.hpp"

class OLEDcontroller: public rtos::task<>
{
private:
	
	hwlib::glcd_oled_buffered & display;
	
	rtos::flag HPChangedflag;
	rtos::flag killerFlag;
	rtos::flag oneMinuteFlag;
	rtos::flag gameOverFlag;
	
	void main( void ) override
	{
		
	}
	
public:
OLEDcontroller(hwlib::glcd_oled_buffered & display):
	task(6, "displaytask")
	display(display)
	{}
	
	void showHPchanged()
	{
		
	}
}
