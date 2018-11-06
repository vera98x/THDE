#ifndef OLEDCONTROLLER_HPP
#define OLEDCONTROLLER_HPP

#include "hwlib.hpp"
#include "rtos.hpp"
#include "hwlib-graphics.hpp"

class OLEDcontroller: public rtos::task<>
{
private:
	hwlib::font_default_8x8 & font;
	hwlib::window_ostream & display;
	
	rtos::flag HPChangedflag;
	rtos::flag yourNameFlag;
	rtos::flag killerFlag;
	rtos::flag oneMinuteFlag;
	rtos::flag gameOverFlag;
	rtos::pool <int> HPInfoPool;
	rtos::pool <const char *> yourNamePool;
	rtos::pool <const char *> killerInfoPool;
	
	void main( void );
	
public:
    OLEDcontroller(hwlib::font_default_8x8 & font, hwlib::window_ostream & display);
	
	void flushHPchanged();
	
	void showHPchanged(const int & HP);
	
	void flushYourName();
	
	void showYourName(char c[15]);
	
	void flushOneMinute();
	
	void showOneMinute();
	
	void flushKiller();
	void showKiller(const char * name);
	void flushGameOver();
	
	void showGameOver();
	
};
#endif //OLEDCONTROLLER_HPP