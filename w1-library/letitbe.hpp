#ifndef _LETITBE_HPP
#define _LETITBE_HPP
#include "melody.hpp"
class letitbe : public melody {
public:
void play( note_player & p ){
      p.play( note{ 1318, 125000 } );
      p.play( note{ 1174, 250000 } );
      p.play( note{ 1046, 500000 } );
      p.play( note{ 1318, 125000 } );
      p.play( note{ 1568, 250000 } );
      p.play( note{ 880, 250000 } );
      p.play( note{ 1568, 375000 } );
      p.play( note{ 1568, 125000 } );
      p.play( note{ 1568, 250000 } );
      p.play( note{ 1318, 250000 } );
      p.play( note{ 1174, 125000 } );
      p.play( note{ 1046, 250000 } );
      p.play( note{ 440, 125000 } );
      p.play( note{ 784, 250000 } );
      p.play( note{ 1318, 750000 } );
      p.play( note{ 0, 500000 } );
      p.play( note{ 1318, 250000 } );
      p.play( note{ 1318, 125000 } );
      p.play( note{ 1396, 375000 } );
      p.play( note{ 1318, 250000 } );
      p.play( note{ 1318, 250000 } );
      p.play( note{ 1174, 250000 } );
      p.play( note{ 0, 125000 } );
      p.play( note{ 1318, 125000 } );
      p.play( note{ 1174, 125000 } );
      p.play( note{ 1174, 250000 } );
      p.play( note{ 1046, 1500000 } );
    }
};
#endif
