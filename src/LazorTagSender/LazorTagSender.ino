#include "IRSend.h"

void setup()
{
	Serial.begin ( 115200 );
	initSend();
	sei();	// enable interrupts
	Serial.println ( "Sender ready" );

}

void loop()
{
	if ( Serial.available() > 0 )
	{
		// read the incoming byte:
		uint16_t incomingInt = Serial.parseInt();
		Serial.print ( "I received: " );
		Serial.println ( incomingInt );
		//while ( irSend.getIsRunning() ) {}
		sendData ( incomingInt );
	}
}