/**
 * @file
 * @brief     buttonListener.
 *
 *	A buttonListener interface
 * 
 * @author    Vera Schoonderwoerd
 * @license   Boost
 */

#ifndef _BUTTONLISTENER_HPP
#define _BUTTONLISTENER_HPP


class buttonListener {

public: 
    /**
	 * @brief pure virtual function to be implemented by buttonListeners.
	 */
    virtual void shootGun() = 0;
};

#endif