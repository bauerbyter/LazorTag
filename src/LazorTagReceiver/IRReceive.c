#include "Arduino.h"
#include "IRReceive.h"

#define MAX_WAIT 4500
#define MAX_BITS 20

const int irRecPin = 2;


volatile uint16_t data[MAX_BITS];
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

	if ( myLastTimeStamp == 0 || duration > MAX_WAIT ) //timer bauen
	{
		myLastTimeStamp = micros(); //error
		return;
	}
	if ( myCount >= MAX_BITS )
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

	if ( duration >= 2800 )
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
	for ( i = 0; i < MAX_BITS; i++ )
	{
		copy[i] = data[i];
	}
}

void resetData()
{
	int i;
	for ( i = 0; i < MAX_BITS; i++ )
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