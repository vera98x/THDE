//
// Created by djrel on 10/31/2018.
//

#ifndef UART_WIFITAAK_HPP
#define UART_WIFITAAK_HPP
#include "hwlib.hpp"
#include "rtos.hpp"
#include "msg.hpp"
#include "hardware_usart.hpp"
#include "commandListener.hpp"
class WifiTaak : rtos::task<>,  commandListener{
private:
	rtos::channel<msg, 10> cmdChannelOut;
	UARTLib::HardwareUART &wifi_chip;
	rtos::timer waiting_timeout;
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
					waiting_timeout.set(100);
					auto e = wait(waiting_timeout + cmdChannelOut);
					if(e == cmdChannelOut) {
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
					cl->commandReceived(received);
				}
				break;
				case STATE::SENDING:
				{
					msg m = cmdChannelOut.read();
					hwlib::string<30> s = "";
					m.serialize(s);
					//wifi_chip.send(s);
				}
			}
		}
	}

public:
<<<<<<< HEAD
<<<<<<< HEAD
	WifiTaak(UARTLib::HardwareUART &ESP, commandListener * cl = nullptr) : 
		task(4, "WiFi Taak"), 
		cl(cl), 
		state(STATE::WAITING),
		cmdChannelOut(this, "cmdChannelIn (WiFiTaak)"),
		wifi_chip(ESP)
		{}
=======
	WifiTaak(UARTLib::HardwareUART &ESP, commandListener * cl = nullptr) : task(4, "WiFi Taak"), cl(cl), state(STATE::WAITING),
										   cmdChannelOut(this, "cmdChannelIn (WiFiTaak)"), wifi_chip(ESP), waiting_timeout(this, "WifiTaak waiting timeout") {}
>>>>>>> c9750d98017e882e066995bcc9673ac8eb926835
=======
	WifiTaak(UARTLib::HardwareUART &ESP, commandListener * cl = nullptr) :
		task(4, "WiFi Taak"),
		cmdChannelOut(this, "cmdChannelIn (WiFiTaak)"),
		wifi_chip(ESP),
		waiting_timeout(this, "WifiTaak waiting timeout"),
		cl(cl),
		state(STATE::WAITING)
	    {}
>>>>>>> 793e11a053aff7fe7dff93063f530a9101349c29

	void commandReceived(const msg & m) {
		cmdChannelOut.write(m);
	}
    
    void setListener(commandListener * cl_def){
        cl = cl_def;
    }
    
};


#endif //UART_WIFITAAK_HPP
