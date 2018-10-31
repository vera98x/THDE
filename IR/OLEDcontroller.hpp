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
	rtos::flag killerFlag;
	rtos::flag oneMinuteFlag;
	rtos::flag gameOverFlag;
	rtos::pool <int> HPInfoPool;
	rtos::pool <const char *> killerInfoPool;
	
	char  * killerName [];

	void main( void ) override
	{
		for(;;)
		{
			auto done = wait(HPChangedflag + oneMinuteFlag + killerFlag + gameOverFlag);
			if (done == HPChangedflag)
			{
				flushHPchanged();
			} 
			else if(done == oneMinuteFlag)
			{
				flushOneMinute();
			} 
			else if(done == killerFlag)
			{
				flushKiller();
			}
			else if(done == gameOverFlag)
			{
				//flushGameOver();
			}
			hwlib::wait_ms(6000);
		}
	}
	
public:
OLEDcontroller(hwlib::font_default_8x8 & font, hwlib::window_ostream & display):
	task(6, "displaytask"),
	font(font),
	display(display),
	HPChangedflag(this, "HPChangedFlag"),
	killerFlag(this, "killerFlag"),
	oneMinuteFlag(this, "oneMinuteFlag"),
	gameOverFlag(this, "gameOverFlag"),
	HPInfoPool("HPinfoPool"),
	killerInfoPool("killerInfoPool")
	{}
	
	void flushHPchanged()
	{
		int newHP = HPInfoPool.read();
		display << "\f" << "    Je hebt: "<< "\n" << "\n" << "     " <<  newHP << "  HP" << '\n'<< "\n" << "     Over"<< hwlib::flush;
		
	}
	
	void showHPchanged(const int & HP)
	{
		HPInfoPool.write(HP);
		HPChangedflag.set();
	}
	
	void flushOneMinute()
	{
		display << "\f" << "   !OPGELET!"<< "\n" << "\n" << "   1 MINUUT" << '\n' << "\n"<< "  SPEELTIJD OVER"<< hwlib::flush;
	}
	
	void showOneMinute()
	{
		oneMinuteFlag.set();
	}
	
	void flushKiller()
	{
		const char * newName = killerInfoPool.read();
		display << "\f" << "Je moordenaar is "<< "\n"  << "\n" << newName  << '\n' << "\n" << "TERUG NAAR SPAWN!"<< hwlib::flush;
	}
	
	void showKiller(const char * name)
	{
		killerInfoPool.write(name);
		killerFlag.set();
	}
	
	void showGameOver()
	{
		gameOverFlag.set();
	}
	
};
#endif //OLEDCONTROLLER_HPP