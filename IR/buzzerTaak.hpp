#include "hwlib.hpp"
#inlcude "rtos.hpp"
#include "note_player_gpio.hpp"
#include "melody.hpp"
class letitbe : public melody {
public:

class buzzer : public rtos::task<>{
    
private:
    hwlib::pin_out b;
    
    rtos::flag gotKilledFlag;
    rtos::flag gameOverFlag;
    rtos::flag lastMinuteFlag;
    rtos::flag hitFlag;
    rtos::flag killFlag;
    
    
    
    void main(){
        
    }
    
public:
    buzzer(hwlib::pin_out b):
    task("buzzerTask"),
    b ( b )
    {}
    
    void playGotKilledSound(){
        gotKilledFlag.set()
    }
    
    void playLastMinuteSound(){
        lastMinuteFlag.set()
    }
    
    void playHitSound(){
        hitFlag.set()
    }
    
    void playGameOverSound(){
        gameOverFlag.set()
    }
    
    void playYouKilledSound(){
        killFlag.set()
    }
    
    
    
    
}