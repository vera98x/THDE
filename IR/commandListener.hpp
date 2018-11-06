/**
 * @file
 * @brief Listener interface that accepts a command
 */

#ifndef THDE_COMMANDLISTENER_HPP
#define THDE_COMMANDLISTENER_HPP
#include "msg.hpp"
class commandListener {
public:
	/**
	 * @brief pure virtual function to be implemented by commandListeners.
	 * @param m
	 */
	virtual void commandReceived(const msg & m) = 0;
};

#endif //THDE_COMMANDLISTENER_HPP
