#include "Arduino.h"
#include "IRReceive.h"
#include "../LazorTag.h"

/*Design Desition
 - Errors:

*/
//Config
const int irRecPin = 2;

volatile uint16_t data[IR_SHOOT_MAX_SIGNALS];
volatile int myCount;
volatile unsigned long myLastTimeStamp;
volatile boolean myIsFinished = false;
volatile boolean myHasError = false;


void interrupt()
{
	if ( myIsFinished || myHasError )
	{
		return;
	}
	unsigned long currentTime = micros();
	unsigned long duration = currentTime - myLastTimeStamp;

	if ( myLastTimeStamp == 0 || duration > IR_MAX_WAIT ) //timer bauen der nach einer Zeit Error wirft
	{
		myHasError = true;
		return;
	}
	if ( myCount >= IR_SHOOT_MAX_SIGNALS )
	{
		myHasError = true;
		return;
	}

	data[myCount] = duration;
	myLastTimeStamp = currentTime;
	myCount++;

	if ( duration >= ( IR_END - IR_DIFF ) )
	{
		myIsFinished = true;
	}

}

boolean IRR_IsDataAvailable()
{
	if ( myHasError )
	{
		return false;
	}
	return myIsFinished;
}

boolean IRR_HasError()
{
	return myHasError;
}

boolean IRR_GetData ( uint16_t copy[] )
{
	int i;
	for ( i = 0; i < IR_SHOOT_MAX_SIGNALS; i++ )
	{
		copy[i] = data[i];
	}
}

void IRR_ResetData()
{
	int i;
	for ( i = 0; i < IR_SHOOT_MAX_SIGNALS; i++ )
	{
		data[i] = 0;
	}

	myCount = 0;
	myLastTimeStamp = 0;

	myIsFinished = false;
	myHasError = false;

}

IRR_Init ( )
{
	pinMode ( irRecPin, INPUT );
	IRR_ResetData();
	attachInterrupt ( 0, interrupt, CHANGE );
}