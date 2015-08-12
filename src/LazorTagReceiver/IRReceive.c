#include "Arduino.h"
#include "IRReceive.h"
#include "../LazorTag.h"

//Config
const int irRecPin = 2;

volatile uint16_t data[IR_SHOOT_MAX_SIGNALS];
volatile int myCount;
volatile unsigned long myLastTimeStamp;
volatile boolean myIsFinished = false;
volatile boolean myHasError = false;

void interrupt()
{
	if ( myIsFinished )
	{
		return;
	}
	unsigned long currentTime = micros();
	unsigned long duration = currentTime - myLastTimeStamp;

	if ( myLastTimeStamp == 0 || duration > IR_MAX_WAIT ) //timer bauen
	{
		myLastTimeStamp = micros(); //error
		return;
	}
	if ( myCount >= IR_SHOOT_MAX_SIGNALS )
	{
		myHasError = true;
		myCount = 0;
		myLastTimeStamp = 0;
		myIsFinished = true;
		return;
	}

	data[myCount] = duration;
	myLastTimeStamp = currentTime;
	myCount++;

	if ( duration >= ( IR_END - IR_DIFF ) )
	{
		myCount = 0;
		myLastTimeStamp = 0;
		myIsFinished = true;
	}

}

boolean isDataAvailable()
{
	return myIsFinished; //Todo Error?
}

boolean getData ( uint16_t copy[] )
{
	int i;
	for ( i = 0; i < IR_SHOOT_MAX_SIGNALS; i++ )
	{
		copy[i] = data[i];
	}
}

void resetData()
{
	int i;
	for ( i = 0; i < IR_SHOOT_MAX_SIGNALS; i++ )
	{
		data[i] = 0;
	}
	myIsFinished = false;
}

IRReceiveInit ( )
{
	pinMode ( irRecPin, INPUT );
	resetData();
	attachInterrupt ( 0, interrupt, CHANGE );
}