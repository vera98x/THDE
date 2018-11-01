//
// Created by djrel on 11/1/2018.
//

#ifndef THDE_COMMANDLISTENER_HPP
#define THDE_COMMANDLISTENER_HPP
#include "msg.hpp"
class commandListener {
public:
	virtual void commandReceived(const msg & m) = 0;
};

#endif //THDE_COMMANDLISTENER_HPP
