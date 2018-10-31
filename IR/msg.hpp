#ifndef _MSG_HPP
#define _MSG_HPP

class msg {
    
public:
    enum class CMD : uint8_t { 
        R_PLAYER_NAME, //0 
        R_SELECTED_DMG, //1 
        R_PLAYER_ID, //2 
        R_START_GAME, //3 
        R_KILLED_BY, //4 
        R_KILL_CONFIRM, //5 
        R_LAST_MINUTE, //6 
        R_GAME_OVER, //7 
        T_KILLED_BY, //8 
        T_REQ_PLAYERID, //9
        R_HP,        //10
        
        N_NOTINITIALISED

    };
    
    
    CMD command = CMD::N_NOTINITIALISED; 
    char naam[15] = ""; 
    uint8_t waarde = 0;    
    
};

#endif 