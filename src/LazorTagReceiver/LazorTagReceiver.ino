#include "IRReceive.h"

void setup()
{
	Serial.begin ( 115200 );
	IRR_Init();
	Serial.println ( "los gehts" );
}

void loop()
{
	uint16_t bla[20];

	if ( IRR_IsDataAvailable() )
	{
		IRR_GetData ( bla );
		readData ( bla ); //IF
		IRR_ResetData ();
	}
	else if ( IRR_HasError() )
	{
		//Do Something with the Error
		Serial.println ( "Error in IRReceive" );
		IRR_ResetData ();
	}

}
//Todo: edit for Lasertag.h and move to own c-file
void readData ( uint16_t data[] )
{
	int result = 0;
	int counter = 0;
	for ( int i = 0; i < 20; i++ )
	{
		if ( i == 0 )
		{
			if ( data[i] < 2200 || data[i] > 2600 )
			{
				//myHasError = true;
				Serial.println ( "HEADER fehlt" );
			}
		}
		else if ( data[i] > 2800 )
		{
			Serial.println ( "fertig" );
			i = 20;
		}
		else if ( i % 2 == 1 )
		{
			if ( data[i] < 400 || data[i] > 800 )
			{
				//myHasError = true;
				Serial.print ( "OFF fehler bei: " );
				Serial.println ( i );
			}
		}
		else
		{

			if ( data[i] > 1000 & data[i] < 1400 )
			{
				//Serial.print("Ist eine 1: ");
				//Serial.println(data[i]);
				result |= 1 << counter;
			}
			else if ( data[i] > 400 & data[i] < 800 ) //Todo eigentlich unnötig
			{
				//Serial.print("Ist eine 0: ");
				//Serial.println(data[i]);
				//result &= 0<<counter;
			}
			else
			{
				//myHasError = true;
				Serial.print ( "Bit fehler bei: " );
				Serial.println ( i );
			}
			counter ++;
		}
	}
	Serial.println ( result );
}





