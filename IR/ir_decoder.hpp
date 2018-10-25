#include "hwlib.hpp"
#include "rtos.hpp"

class ir_decoder: public rtos::task<>
{
	
private:
	hwlib::target::pin_in & decoder;
	int firstSet [16]={};
	int secondSet [16]={};

public:
ir_decoder(hwlib::target::pin_in & decoder, int firstSet, int secondSet):
	decoder(decoder),
	firstSet(firstSet),
	secondSet(secondSet)
	{}
	
	void main( void )
	{
			while(1)
	{
		for(unsigned int j=0; j<=1; j++)
		{
			//hwlib::wait_ms(3);
			int firstpin = !decoder.get();
			if(firstpin == 1)
			{
				firstSet [0] = !decoder.get();
				secondSet [0] = !decoder.get();
				while(!decoder.get() == 1){
					
				}
				while(!decoder.get() == 0){
					
				}
				for(unsigned int i=1; i <= 15; i++)
				{
					while(!decoder.get() == 0){
					
					}
					hwlib::wait_us(1000);
					if (j == 0){
						firstSet [i] = !decoder.get();
					}
					else
					{
						secondSet [i] =!decoder.get();
					}
					hwlib::wait_us(1000);
				}
				
			}
			
			if (firstpin && j ==1)
			{
				if(firstSet[11] != (firstSet[1] ^firstSet[6]))
				{
					hwlib::cout << " one or six are false! \n" ;
				}
				else
				{
					hwlib::cout << " one and six are true! \n";
				}
				if(firstSet[12] != (firstSet[2] ^firstSet[7]))
				{
					hwlib::cout << "two and seven are false! \n";
				}
				else
				{
					hwlib::cout << " two and seven are true! \n";
				}
				if(firstSet[13] != (firstSet[3] ^firstSet[8]))
				{
					hwlib::cout << " three or eight are false! \n" ;
				}
				else
				{
					hwlib::cout << " three and eight are true! \n";
				}
				if(firstSet[14] != (firstSet[4] ^firstSet[9]))
				{
					hwlib::cout << " four or nine are false! \n" ;
				}
				else
				{
					hwlib::cout << " four and nine are true! \n";
				}
				
				
				
				for(unsigned int i=0; i <=15; i++)
				{
					hwlib::cout<< "bit:	" << i <<" 	=	"<< firstSet[i] << '\n';
				}
				hwlib::cout<< "----------EERSTE-----------" << '\n';
				for(unsigned int i=0; i<=15; i++)
				{
					hwlib::cout<< "bit:	" << i <<" 	=	"<< secondSet[i] << '\n';
				}
				hwlib::cout<< "----------TWEEDE-----------" << '\n';
			}
			}
		}
	}


};