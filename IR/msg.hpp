#ifndef _MSG_HPP
#define _MSG_HPP
#include "hwlib.hpp"
class msg {

public:
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

		N_NOTINITIALISED

	};



	CMD command = CMD::N_NOTINITIALISED;
	char naam[15] = "";
	uint8_t waarde = 0;

	msg(){}
	/*msg(hwlib::string<0> & s){
		int colon = s.find(':');
		int comma = s.find(',');
		int semicolon = s.find(';');

		command = static_cast<CMD>(s[colon+1] - '0');
		if(command == CMD::R_PLAYER_NAME || command == CMD::R_KILLED_BY){
			for(uint8_t i = comma + 1, j = 0; i < semicolon; i++, j++){
				naam[j] = s[i];
			}
		}else{
			waarde = static_cast<uint8_t>(s[comma+1] - '0');
		}
	}

	void serialize(hwlib::string<0> & s){
		s.clear();
		s << "CMD:" << ((uint8_t)command + '0');
		if(command == CMD::R_PLAYER_NAME || command == CMD::R_KILLED_BY){
			s << naam;
		}else{
			s << (waarde + '0');
		}
		s << ';';
	}*/
};

#endif 