#include "hwlib.hpp"


int main( void ){	
    
   // kill the watchdog, wait for the PC console to start
   WDT->WDT_MR = WDT_MR_WDDIS;
   hwlib::wait_ms( 1000 );   
   namespace target = hwlib::target; 
   
   auto pinnetje = target::pin_in( target::pins::d7 );
   int message [16]={};
   int messageTwotje [16]={};
	while(1)
	{
		for(unsigned int j=0; j<=1; j++)
		{
			//hwlib::wait_ms(3);
			int firstpin = !pinnetje.get();
			if(firstpin == 1)
			{
				message [0] = !pinnetje.get();
				messageTwotje [0] = !pinnetje.get();
				while(!pinnetje.get() == 1){
					
				}
				while(!pinnetje.get() == 0){
					
				}
				for(unsigned int i=1; i <= 15; i++)
				{
					while(!pinnetje.get() == 0){
					
					}
					hwlib::wait_us(1000);
					if (j == 0){
						message [i] = !pinnetje.get();
					}
					else
					{
						messageTwotje [i] =!pinnetje.get();
					}
					hwlib::wait_us(1000);
				}
				
			}
			
			
			
			if (firstpin && j ==1)
			{
				if(message[11] != (message[1] ^message[6]))
				{
					hwlib::cout << " one or six are false! \n" ;
				}
				else
				{
					hwlib::cout << " one and six are true! \n";
				}
				if(message[12] != (message[2] ^message[7]))
				{
					hwlib::cout << "two and seven are false! \n";
				}
				else
				{
					hwlib::cout << " two and seven are true! \n";
				}
				if(message[13] != (message[3] ^message[8]))
				{
					hwlib::cout << " three or eight are false! \n" ;
				}
				else
				{
					hwlib::cout << " three and eight are true! \n";
				}
				if(message[14] != (message[4] ^message[9]))
				{
					hwlib::cout << " four or nine are false! \n" ;
				}
				else
				{
					hwlib::cout << " four and nine are true! \n";
				}
				
				
				
				for(unsigned int i=0; i <=15; i++)
				{
					hwlib::cout<< "bit:	" << i <<" 	=	"<< message[i] << '\n';
				}
				hwlib::cout<< "----------EERSTE-----------" << '\n';
				for(unsigned int i=0; i<=15; i++)
				{
					hwlib::cout<< "bit:	" << i <<" 	=	"<< messageTwotje[i] << '\n';
				}
				hwlib::cout<< "----------TWEEDE-----------" << '\n';
			}
			
		}
		
		
	}
}