#ifndef LazorTag_h
#define LazorTag_h

#include "Arduino.h"

//IR-Signal defines
#define IR_SHOOT_MAX_BITS 		8
#define IR_SHOOT_MAX_SIGNALS	( ( IR_SHOOT_MAX_BITS + 2 ) *2 )
#define IR_HEADER 				2400
#define IR_END 					3000
#define IR_ONE 					1200
#define IR_ZERO					600
#define IR_OFF 					600
#define IR_MAX_WAIT 			4000
#define IR_DIFF 				150 // the max deviation of the IR-Signals

//Timer defines
#define T_HEADER 				( IR_HEADER / 4 ) 	// 600
#define T_END 					( IR_END / 4 )		// 750
#define T_ONE 					( IR_ONE / 4 )		// 300
#define T_ZERO 					( IR_ZERO / 4 )		// 150
#define T_OFF 					( IR_OFF / 4 )		// 150 
#define T_MAX_WAIT				( IR_MAX_WAIT / 4 )	// 1000 




#endif