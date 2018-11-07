
#include "button.hpp"
#include "hwlib.hpp"
#include "rtos.hpp"
#include "ir_send.hpp"

void button::main ( void )
{
    for(;;)
    {
        if(gunTrigger.get()==0 && previousShot == 0)
        {
            previousShot = 1;
            //hwlib::cout<< "B" << previousShot << '\n';
            transmitter.shootGun();
        }
        if(gunTrigger.get()==1 && previousShot == 1)
        {
            previousShot = 0;
        }
        hwlib::wait_ms(5);
    }
}

button::button(hwlib::target::pin_in & gunTrigger, buttonListener & transmitter):
    task(3, "buttonTask"),
    gunTrigger(gunTrigger),
    transmitter (transmitter)
    {}

