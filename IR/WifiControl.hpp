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

class WifiControl : rtos::task<>, public commandListener {
private:
	rtos::channel<msg, 10> cmdChannelOut;
	UARTLib::HardwareUART &wifi_chip;
	rtos::timer waiting_timeout;
	commandListener *cl;
	enum class STATE {
		LISTENING,
		SENDING,
		WAITING

	};
	STATE state;

	void main() override ;

public:

	explicit WifiControl(UARTLib::HardwareUART &ESP, commandListener *cl = nullptr);

	void commandReceived(const msg &m) override;

	void setListener(commandListener *cl_def);

};


#endif //UART_WIFITAAK_HPP
