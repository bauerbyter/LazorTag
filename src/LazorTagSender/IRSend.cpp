#include "Arduino.h"
#include "IRSend.h"

#define HEADER 600 //2400us
#define END 750 //3000us
#define ONE 300 //1200us
#define ZERO 150 //600us
#define OFF 150 // 600us 
#define MAXBITS 8

const int IRLedPin = 11;
volatile int myCounter = 0;
volatile boolean myIsRunning = false;

uint16_t myData = 0;

void startTimer1()
{
	TCCR1B |= ( 1 << CS11 ) | ( 1 << CS10 ); // set prescaler to 64 and start TIMER1 (micro-Timer)
}

void startTimer2()
{
	TCCR2A |= ( 1 << COM2A0 ); //turn on
}

void stopTimer1()
{
	TCCR1B &= ~ ( ( 1 << CS10 ) | ( 1 << CS11 ) ); //Set Bit to 0 => StopTimer
}

void stopTimer2()
{
	TCCR2A &= ~ ( 1 << COM2A0 );
}

void toggleTimer2()
{
	TCCR2A ^= ( 1 << COM2A0 ); //Toggle PWM on/off
}

ISR ( TIMER1_COMPA_vect )
{
	if ( myCounter >= MAXBITS + 1 ) //+1 because of END
	{
		stopTimer1();
		stopTimer2();
		digitalWrite ( IRLedPin, LOW );
		myCounter = 0;
		myData = 0;
		myIsRunning = false;
	}
	else
	{
		if ( TCCR2A & ( 1 << COM2A0 ) ) //if HIGH
		{
			stopTimer2();
			digitalWrite ( IRLedPin, LOW );
			OCR1A = OFF;
		}
		else
		{
			if ( myCounter == MAXBITS )
			{
				OCR1A = END;
			}

			else if ( myData & ( 1 << myCounter ) ) //Highest bit first
			{
				OCR1A = ONE;
			}
			else
			{
				OCR1A = ZERO;
			}
			startTimer2();
			myCounter++;
		}
	}
}

void initSend( )
{
	pinMode ( IRLedPin, OUTPUT );
	//38khz Timer2
	TCCR2A = 0;     // set entire TCCR2A register to 0
	TCCR2B = 0;     // same for TCCR2B
	TCCR2A |= ( 1 << WGM21 );
	TCCR2B |= ( 1 << CS20 );
	OCR2A = 209; // oder 210 --SET TOP COUNTER2 VALUE (FOR CTC)

	//Timer1 for duration
	TCCR1A = 0;     // set entire TCCR1A register to 0
	TCCR1B = 0;     // same for TCCR1B
	TCCR1B |= ( 1 << WGM12 );	// Mode 4, CTC on OCR1A
	TIMSK1 |= ( 1 << OCIE1A ); //Set interrupt on compare match
}

void sendData ( uint16_t data )
{
	if ( myIsRunning )
	{
		return;
	}
	myIsRunning = true;
	myData = data;
	myCounter = 0;
	OCR1A = HEADER;
	digitalWrite ( IRLedPin, LOW );
	startTimer1();
	startTimer2();
}

boolean getIsRunning()
{
	return myIsRunning;
}
