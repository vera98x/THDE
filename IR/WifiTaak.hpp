//
// Created by djrel on 10/31/2018.
//

#ifndef UART_WIFITAAK_HPP
#define UART_WIFITAAK_HPP

#include "rtos.hpp"
#include "msg.hpp"
#include "hardware_usart.hpp"

class WifiTaak : rtos::task<> {
private:
	rtos::channel<msg, 10> cmdChannelOut;
	UARTLib::HardwareUART &wifi_chip;
	runGameController & r;
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
	WifiTaak(UARTLib::HardwareUART &ESP, runGameController & r) : task(4, "WiFi Taak"), r(r), state(STATE::WAITING),
										   cmdChannelOut(this, "cmdChannelIn (WiFiTaak)"), wifi_chip(ESP) {}

	void sendMsg(msg &m) {
		cmdChannelOut.write(m);
	}
};


#endif //UART_WIFITAAK_HPP
