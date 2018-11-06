
#include "hwlib.hpp"
#include "rtos.hpp"
#include "hwlib-graphics.hpp"
#include "OLEDcontroller.hpp"


	void OLEDcontroller::main( void )
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
	

    OLEDcontroller::OLEDcontroller(hwlib::font_default_8x8 & font, hwlib::window_ostream & display):
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
	
	void OLEDcontroller::flushHPchanged()
	{
		int newHP = HPInfoPool.read();
		display << "\f" << "    Je hebt: "<< "\n" << "\n" << "     " <<  newHP << "  HP" << '\n'<< "\n" << "      over"<< hwlib::flush;
		
	}
	
	void OLEDcontroller::showHPchanged(const int & HP)
	{
		HPInfoPool.write(HP);
		HPChangedflag.set();
	}
	
	void OLEDcontroller::flushYourName()
	{
		const char * yourNewName = yourNamePool.read();
		display << "\f" << "   Welcome: "<< "\n" << "\n    " << yourNewName << '\n' << "\n"<< "  to the match!"<< hwlib::flush;
	}
	
	void OLEDcontroller::showYourName(char c[15])
	{
		yourNamePool.write(c);
        yourNameFlag.set();
	}
	
	void OLEDcontroller::flushOneMinute()
	{
		display << "\f" << "   !OPGELET!"<< "\n" << "\n" << "   1 MINUUT" << '\n' << "\n"<< "  SPEELTIJD OVER"<< hwlib::flush;
	}
	
	void OLEDcontroller::showOneMinute()
	{
		oneMinuteFlag.set();
	}
	
	void OLEDcontroller::flushKiller()
	{
		const char * newName = killerInfoPool.read();
		display << "\f" << "  YOUR KILLER IS:  "<< "\n"  << "\n" << newName  << '\n' << "\n" << "!BACK TO SPAWN!"<< hwlib::flush;
	}
	
	void OLEDcontroller::showKiller(const char * name)
	{
		killerInfoPool.write(name);
		killerFlag.set();
	}
	
	void OLEDcontroller::flushGameOver()
	{
		display << "\f" << "    GAME "<< "\n"  << "\n" << "    OVER"  << '\n' << "\n" << "   !YOU SUCK!"<< hwlib::flush;
	}
	
	void OLEDcontroller::showGameOver()
	{
		gameOverFlag.set();
	}
