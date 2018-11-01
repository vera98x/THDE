//
// Created by djrel on 10/31/2018.
//

#ifndef UART_WIFITAAK_HPP
#define UART_WIFITAAK_HPP

#include "rtos.hpp"
#include "msg.hpp"
#include "hardware_usart.hpp"
#include "commandListener.hpp"
class WifiTaak : rtos::task<>,  commandListener{
private:
	rtos::channel<msg, 10> cmdChannelOut;
	UARTLib::HardwareUART &wifi_chip;
	commandListener * cl;
	enum class STATE {
		LISTENING,
		SENDING,
		WAITING

	};
	STATE state;

	void main() override {
		for (;;) {
			switch (state){
				case STATE::WAITING:
				{
					if(wifi_chip.char_available()){
						state = STATE::LISTENING;
					}
					if(cmdChannelOut.getCurrentQSize() > 0){
						state = STATE::SENDING;
					}
				}
				break;
				case STATE::LISTENING:
				{
					hwlib::string<30> s;
					while(wifi_chip.char_available()){
						s<<wifi_chip.getc();
					}
					msg received(s);
					r.receiveCmd(received);
				}
			}
		}
	}

public:
	WifiTaak(UARTLib::HardwareUART &ESP, commandListener * cl = nullptr) : task(4, "WiFi Taak"), cl(cl), state(STATE::WAITING),
										   cmdChannelOut(this, "cmdChannelIn (WiFiTaak)"), wifi_chip(ESP) {}

	void commandReceived(const msg & m) {
		cmdChannelOut.write(m);
	}
    
    void setListener(commandListener * cl_def){
        cl = cl_def;
    }
    
};


#endif //UART_WIFITAAK_HPP
