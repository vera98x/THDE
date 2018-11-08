//
// Created by djrel on 10/31/2018.
//
#include "hwlib.hpp"
#include "rtos.hpp"
#include "msg.hpp"
#include "hardware_usart.hpp"
#include "commandListener.hpp"
#include "WifiControl.hpp"

WifiControl::WifiControl(UARTLib::HardwareUART &ESP, commandListener *cl) :
	task(9, "WiFi Taak"),
	cmdChannelOut(this, "cmdChannelIn (WiFiTaak)"),
	wifi_chip(ESP),
	waiting_timeout(this, "WifiControl waiting timeout"),
	cl(cl),
	state(STATE::WAITING)
	{}

void WifiControl::main(){
	hwlib::string<30> s;
    for (;;) {
		switch (state) {
			case STATE::WAITING: {
				
                if (wifi_chip.char_available()) {
                    hwlib::wait_us(5000);
                    state = STATE::LISTENING;
				}
				waiting_timeout.set(100);
				auto e = wait(waiting_timeout + cmdChannelOut);
				if (e == cmdChannelOut) {
					state = STATE::SENDING;
				}
				hwlib::wait_us(100);
			}
				break;
			case STATE::LISTENING: {
				
				char c = 0;
                if (wifi_chip.char_available()) {
					c = wifi_chip.getc();
                    s << c;
                    
                    hwlib::wait_us(100);
				}
                if (c == ';'){
                    hwlib::cout << "\n" << s << "\n";
                    msg received(s);
                    cl->commandReceived(received);
                    state = STATE::WAITING;
                    s.clear();
                }
			}
                hwlib::wait_us(100);
				break;
			case STATE::SENDING: {
				msg m = cmdChannelOut.read();
				s.clear();
				m.serialize(s);
				wifi_chip.send(s);
                hwlib::cout<< s << " \n";
				state = STATE::WAITING;
				s.clear();
			}
		}
	}
}

void WifiControl::commandReceived(const msg &m) {
	cmdChannelOut.write(m);
}

void WifiControl::setListener(commandListener *cl_def){
	cl = cl_def;
}