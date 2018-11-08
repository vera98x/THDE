/**
 * @file
 * @brief     MSG ADT.
 *
 *	This class can contain a message that has been received or can be sent to the GameManager through the WifiControl.
 *	It contains methods for constructing itself from a string and for serializing it's member variables back to a string for sending.
 *
 * @author    Florian Humblot
 * @license   Boost
 */
#ifndef _MSG_HPP
#define _MSG_HPP
#include "hwlib.hpp"
class msg {

public:
	/**
	 * @brief enum of the commands we can receive to keep the code readable.
	 */
	enum class CMD : uint8_t {
		R_PLAYER_NAME,  //0
		R_SELECTED_DMG, //1
		R_PLAYER_ID,    //2
		R_START_GAME,   //3
		R_KILLED_BY,    //4
		R_KILL_CONFIRM, //5
		R_LAST_MINUTE,  //6
		R_GAME_OVER,    //7
		T_KILLED_BY,    //8
		R_HP,           //9
        T_REQ_PLAYERID, //10

		N_NOTINITIALISED

	};


	/**
	 * @brief what command this message represents - defaults to CMD::NOT_INITIALIZED
	 */
	CMD command = CMD::N_NOTINITIALISED;
	/**
	 * @brief parameter of the command (can be a name or a number) default is an empty char array, but can grow up to 15 characters.
	 */
	char naam[15] = "";
	/**
	 * @brief parameters of the command (can be a name or a number) default is 0, contains a value associated with the command.
	 */
	uint8_t waarde = 0;

	/**
	 * @brief default empty constructor. Does absolutely nothing
	 */
	msg(){}
	/**
	 * @brief constructs a message based on a hwlib::string<>
	 * @param s
	 */
	msg(hwlib::string<0> & s){
		hwlib::cout << "start contructor!!!! \n";
        int colon = s.find(':');
		hwlib::cout << "Colon: " << colon << '\n';
		int comma = s.find(',');
		hwlib::cout << "comma: " << comma << '\n';
		int semicolon = s.find(';');
		hwlib::cout << "semicolon: " << semicolon<< '\n';

		command = static_cast<CMD>(s[colon+1] - '0');
		if(command == CMD::R_PLAYER_NAME || command == CMD::R_KILLED_BY || command == CMD::R_KILL_CONFIRM){
			for(uint8_t i = comma + 1, j = 0; i < semicolon; i++, j++){
				naam[j] = s[i];
			}
		}else{
			waarde = static_cast<uint8_t>(s[comma+1] - '0');
		}
	}

	/**
	 * Serialize the object into a hwlib::string<> to make sending it through UART possible.
	 * @param s
	 */
	void serialize(hwlib::string<0> & s){
		s.clear();
		s << "CMD:";
		s << (char)((uint8_t)command + '0') << ',';
		if(command == CMD::R_PLAYER_NAME || command == CMD::R_KILLED_BY || command == CMD::R_KILLED_BY){
			s << naam;
		}else{
            //hwlib::cout << waarde << '\n';
			s << (char)(waarde + '0');
		}
		s << ';';
	}
};

#endif