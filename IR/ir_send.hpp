#ifndef _IR_SEND_HPP
#define _IR_SEND_HPP


#include "hwlib.hpp"
#include "rtos.hpp"


class ir_send : public rtos::task<>{
private:
    hwlib::target::d2_36kHz & encoder;
    bool values[16] = {};
    rtos::flag sendIRFlag;
    rtos::flag enableFlag;
    rtos::flag disableFlag;
    
    enum class STATE {ENABLED, DISABLED};
	enum STATE state;
    
    void main() override{
        
       while (1){
		   //hwlib::cout << "start" << "\n";
           auto done = wait(sendIRFlag + enableFlag + disableFlag);
           if(1){
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
                       
                      //hwlib::cout << values[i] << "\n";
                   }
                  // hwlib::cout << "end  " << i << "\n";
                   encoder.set(0);
                   hwlib::wait_ms(3000);
               }
           } else if (done == enableFlag){
               state = STATE::ENABLED;
           } else if (done == disableFlag){
               state = STATE::DISABLED;
           }
		   hwlib::wait_ms(4);
		}
        
    }
	
public:
	ir_send(hwlib::target::d2_36kHz & encoder):
    task(2, "msg_send" ),
	encoder  ( encoder ),
    sendIRFlag (this, "sendIRFlag"),
    enableFlag (this, "enableFlag"),
    disableFlag (this, "disableFlag"),
    state(STATE::DISABLED)
    {}
    
    void setIrpattern(uint8_t sp, uint8_t dmg){
        values[0] = 1;
        int j = 1;
        for(int i = 4; i >= 0; i--){
            values[j] = ((sp >> (i)) & 1);
            j++;
        }
        for(int i = 4; i >=0 ; i--){
            values[j] = (dmg >> (i)) & 1;
            j++;
        }
        for(int i = 4; i >=0 ; i--){
            values[j] = ((sp >> (i)) & 1 ) ^ ((dmg >> (i)) & 1);
            j++;
        }
    }
    
    void shootGun(){
        sendIRFlag.set();
    }
    
    void enable(){
        enableFlag.set();
    }
    void disable(){
        disableFlag.set();
    }
    
};

#endif