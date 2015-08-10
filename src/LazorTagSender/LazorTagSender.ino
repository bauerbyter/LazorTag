#include "IRSend.h"

IRSend irSend;
void setup()
{
	Serial.begin ( 115200 );
	sei();	// enable interrupts
}

void loop()
{
	if ( Serial.available() > 0 )
	{
		// read the incoming byte:
		uint16_t incomingInt = Serial.parseInt();
		Serial.print ( "I received: " );
		Serial.println ( incomingInt );
		while ( !irSend.getIsRunning() ) {}
		irSend.sendData ( incomingInt );
	}
}