#ifndef IRSend_h
#define IRSend_h

#include "Arduino.h"

class IRSend
{
public:
	IRSend();
	void sendData ( int data );
	boolean getIsRunning();
};

#endif