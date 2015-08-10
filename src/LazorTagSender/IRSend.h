#ifndef IRSend_h
#define IRSend_h

#include "Arduino.h"

class IRSend
{
public:
	IRSend();
	void sendData ( uint16_t data );
	boolean getIsRunning();
};

#endif