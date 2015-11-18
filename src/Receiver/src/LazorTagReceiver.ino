#include "IRReceive.h"
#include "../../libraries/LazorTag.h"
#include <Wire.h>

byte testTWI = 0;
// 0 = error
int transformIrInInt ( uint16_t data[] ) //Todo: maybe return bool?
{
	int result = 0;
	int counter = 0;
	bool hasError = false;
	for ( int i = 0; i < IR_SHOOT_MAX_SIGNALS; i++ )
	{
		Serial.println ( data[i] );
		if ( i == 0 )
		{
			if ( !isInRange ( data[i], IR_HEADER ) )
			{
				hasError = true;

				Serial.println ( "HEADER fehlt" );
			}
		}
		else if ( isInRange ( data[i], IR_END ) )
		{
			Serial.println ( "fertig" );
			if ( hasError )
			{
				return 0;
			}
			return result;
		}
		else if ( i % 2 == 1 )
		{
			if ( !isInRange ( data[i], IR_OFF ) )
			{
				hasError = true;
				Serial.print ( "OFF fehler bei: " );
				Serial.println ( i );
			}
		}
		else
		{
			if ( isInRange ( data[i], IR_ONE ) )
			{
				//Serial.print("Ist eine 1: ");
				//Serial.println(data[i]);
				result |= 1 << counter;
			}
			else if ( isInRange ( data[i], IR_ZERO ) )
			{
				//result &= 0<<counter;
			}
			else
			{
				hasError = true;
				Serial.print ( "Bit fehler bei: " );
				Serial.println ( i );
			}
			counter ++;
		}
	}
	return 0;
}


void setup()
{
	Serial.begin ( 115200 );
	IRR_Init();
	Wire.begin(8);                // join i2c bus with address #8
  Wire.onRequest(requestEvent); // register event
	Serial.println ( "los gehts" );
}

void loop()
{
	uint16_t data[IR_SHOOT_MAX_SIGNALS];

	if ( IRR_IsDataAvailable() )
	{
		IRR_GetData ( data );
		int result = transformIrInInt ( data );
		Serial.print ( "Incoming Data: " );
		Serial.println ( result );
		IRR_ResetData ();
		testTWI = result;
	}
	else if ( IRR_HasError() )
	{
		//Do Something with the Error
		Serial.println ( "Error in IRReceive" );
		IRR_ResetData ();
	}

}

void requestEvent() {
  Wire.write( testTWI); // respond with message of 1 bytes
	testTWI=0;
  // as expected by master
}

boolean isInRange ( uint16_t value, uint16_t ref )
{
	if ( value > ref - IR_DIFF && value < ref + IR_DIFF )
	{
		return true;
	}
	return false;
}
