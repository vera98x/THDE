
#include "hwlib.hpp"
#include "rtos.hpp"
#include "ir_send.hpp"


    void ir_send::main(){
        
       while (1){
		   //hwlib::cout << "start" << "\n";
           auto done = wait(sendIRFlag + enableFlag + disableFlag);
           if(done == sendIRFlag && state == STATE::ENABLED){
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
                   hwlib::wait_ms(3);
               }
           } else if (done == enableFlag){
               state = STATE::ENABLED;
           } else if (done == disableFlag){
               state = STATE::DISABLED;
           }
		   hwlib::wait_us(30);
		}
        
    }
	
    ir_send::ir_send(hwlib::target::d2_36kHz & encoder):
        task(2, "msg_send" ),
        encoder  ( encoder ),
        sendIRFlag (this, "sendIRFlag"),
        enableFlag (this, "enableFlag"),
        disableFlag (this, "disableFlag"),
        state(STATE::DISABLED)
        {}
    
    void ir_send::setIrpattern(uint8_t sp, uint8_t dmg){
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
    
    void ir_send::shootGun(){
        sendIRFlag.set();
    }
    
    void ir_send::enable(){
        enableFlag.set();
    }
    void ir_send::disable(){
        disableFlag.set();
    }
    
