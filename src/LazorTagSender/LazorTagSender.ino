#include "IRSend.h"

IRSend irSend;
void setup()
{
	Serial.begin ( 115200 );
}

void loop()
{
	if ( Serial.available() > 0 )
	{
		// read the incoming byte:
		int incomingInt = Serial.parseInt();
		Serial.print ( "I received: " );
		Serial.println ( incomingInt );
		while ( !irSend.getIsFinished() ) {}
		irSend.sendData ( incomingInt );
	}
}