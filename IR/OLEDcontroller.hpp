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
	
	char  * killerName [];

	void main( void ) override
	{
		for(;;)
		{
			auto done = wait(HPChangedflag + yourNameFlag + oneMinuteFlag + killerFlag + gameOverFlag);
			if (done == HPChangedflag)
			{
				flushHPchanged();
			}
			else if(done == yourNameFlag)
			{
				flushYourName();
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
				flushGameOver();
			}
			hwlib::wait_ms(6);
		}
	}
	
public:
OLEDcontroller(hwlib::font_default_8x8 & font, hwlib::window_ostream & display):
	task(6, "displaytask"),
	font(font),
	display(display),
	HPChangedflag(this, "HPChangedFlag"),
	yourNameFlag(this, "yourNameFlag"),
	killerFlag(this, "killerFlag"),
	oneMinuteFlag(this, "oneMinuteFlag"),
	gameOverFlag(this, "gameOverFlag"),
	HPInfoPool("HPinfoPool"),
	yourNamePool("yourNamePool"),
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
	
	void flushYourName()
	{
		const char * yourNewName = yourNamePool.read();
		display << "\f" << "   Welcome: "<< "\n" << "\n    " << yourNewName << '\n' << "\n"<< "  to the match!"<< hwlib::flush;
	}
	
	void showYourName(char c[15])
	{
		yourNamePool.write(c);
        yourNameFlag.set();
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
		display << "\f" << "  YOUR KILLER IS:  "<< "\n"  << "\n" << newName  << '\n' << "\n" << "!BACK TO SPAWN!"<< hwlib::flush;
	}
	
	void showKiller(const char * name)
	{
		killerInfoPool.write(name);
		killerFlag.set();
	}
	
	void flushGameOver()
	{
		display << "\f" << "    GAME "<< "\n"  << "\n" << "    OVER"  << '\n' << "\n" << "   !YOU SUCK!"<< hwlib::flush;
	}
	
	void showGameOver()
	{
		gameOverFlag.set();
	}
	
	
};
#endif //OLEDCONTROLLER_HPP