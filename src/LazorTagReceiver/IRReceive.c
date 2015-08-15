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


void startTimer1()
{
	TCNT1 = 0;
	TCCR1B |= ( 1 << CS11 ) | ( 1 << CS10 ); // set prescaler to 64 and start TIMER1 (micro-Timer)
}

void stopTimer1()
{
	TCCR1B &= ~ ( ( 1 << CS10 ) | ( 1 << CS11 ) ); //Set Bit to 0 => StopTimer
}

ISR ( TIMER1_COMPA_vect )
{
	myHasError = true;
	stopTimer1();
}

void interrupt()
{
	stopTimer1();

	if ( myIsFinished || myHasError )
	{
		return;
	}

	startTimer1();

	unsigned long currentTime = micros();
	unsigned long duration = currentTime - myLastTimeStamp;

	if ( myLastTimeStamp == 0 )
	{
		myLastTimeStamp = currentTime;
		stopTimer1();
		return;
	}
	if ( myCount >= IR_SHOOT_MAX_SIGNALS )
	{
		myHasError = true;
		stopTimer1();
		return;
	}

	data[myCount] = duration;
	myLastTimeStamp = currentTime;
	myCount++;

	if ( duration >= ( IR_END - IR_DIFF ) )
	{
		myIsFinished = true;
		stopTimer1();
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

	//Timer1 for duration
	TCCR1A = 0;     // set entire TCCR1A register to 0
	TCCR1B = 0;     // same for TCCR1B
	TCCR1B |= ( 1 << WGM12 );	// Mode 4, CTC on OCR1A
	TIMSK1 |= ( 1 << OCIE1A ); //Set interrupt on compare match
	OCR1A = T_MAX_WAIT;
}