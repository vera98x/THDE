
#include "hwlib.hpp"
#include "rtos.hpp"


class ir_send : public rtos::task<>{
    
    hwlib::target::d2_36kHz & encoder;
    bool values[16] = {1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0};
    
    
    ir_send(hwlib::target::d2_36kHz & encoder, bool values[16]):
    encoder  ( encoder ),
    values ( values )
    {}
    
    
    void main() override{
        
       while (1){
       encoder.set(1);
       hwlib::cout << "start" << "\n";
       for (int i = 0; i < 2; i++){
           for(int i = 0; i < 16; i++){
               if (values[i]){
                   encoder.set(1);
                   hwlib::wait_us(1600);
                   encoder.set(0);
                   hwlib::wait_us(800);
               } else {
                   encoder.set(1);
                   hwlib::wait_us(800);
                   encoder.set(0);
                   hwlib::wait_us(1600);
               }
               
               hwlib::cout << values[i] << "\n";
           }
           hwlib::cout << "end  " << i << "\n";
           encoder.set(0);
           hwlib::wait_ms(3000);
       }
       hwlib::wait_ms(4);
   }
        
    }
};