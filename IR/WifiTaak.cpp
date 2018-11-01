//
// Created by djrel on 10/31/2018.
//
#include "hwlib.hpp"
#include "rtos.hpp"
#include "msg.hpp"
#include "hardware_usart.hpp"
#include "commandListener.hpp"
#include "WifiTaak.hpp"

WifiTaak::WifiTaak(UARTLib::HardwareUART &ESP, commandListener *cl) :
	task(4, "WiFi Taak"),
	cmdChannelOut(this, "cmdChannelIn (WiFiTaak)"),
	wifi_chip(ESP),
	waiting_timeout(this, "WifiTaak waiting timeout"),
	cl(cl),
	state(STATE::WAITING)
	{}

void WifiTaak::main(){
	for (;;) {
		switch (state) {
			case STATE::WAITING: {
				if (wifi_chip.char_available()) {
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
				hwlib::string<30> s;
				while (wifi_chip.char_available()) {
					s << wifi_chip.getc();
				}
				msg received(s);
				cl->commandReceived(received);
				state = STATE::WAITING;
			}
				break;
			case STATE::SENDING: {
				msg m = cmdChannelOut.read();
				hwlib::string<30> s = "";
				m.serialize(s);
				wifi_chip.send(s);
				state = STATE::WAITING;
			}
		}
	}
}

void WifiTaak::commandReceived(const msg &m) {
	cmdChannelOut.write(m);
}

void WifiTaak::setListener(commandListener *cl_def){
	cl = cl_def;
}