#include <ioavr.h>
#include <inavr.h>
#include "TWI_Master.h"

#define TWI_GEN_CALL         0x00  // The General Call address is 0

// Sample TWI transmission commands
#define TWI_CMD_MASTER_WRITE 0x10
#define TWI_CMD_MASTER_READ  0x20

// Sample TWI transmission states, used in the main application.
#define SEND_DATA             0x01
#define REQUEST_DATA          0x02
#define READ_DATA_FROM_BUFFER 0x03

void setup()
{
	Serial.begin ( 115200 );
	TWI_Master_Initialise();
	sei();
	Serial.println ( "Master ready to command" );
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
		sendTWI ( incomingInt );
	}
}

void sendTWI ( uint16_t foo )
{
	if ( foo > 255 )
	{
		Serial.println ( "Input to big" );
		return;

	}

	unsigned char messageBuf[3];

	messageBuf[0] = ( 0x10 << TWI_ADR_BITS ) | ( FALSE << TWI_READ_BIT ); // The first byte must always consit of General Call code or the TWI slave address.
	messageBuf[1] = TWI_CMD_MASTER_WRITE;             // The first byte is used for commands.
	messageBuf[2] = foo;                        // The second byte is used for the data.
	TWI_Start_Transceiver_With_Data ( messageBuf, 3 );
}

unsigned char TWI_Act_On_Failure_In_Last_Transmission ( unsigned char TWIerrorMsg )
{
	// A failure has occurred, use TWIerrorMsg to determine the nature of the failure
	// and take appropriate actions.
	// Se header file for a list of possible failures messages.

	// Here is a simple sample, where if received a NACK on the slave address,
	// then a retransmission will be initiated.
	Serial.println ( "Failure In Last Transmission" );

	if ( ( TWIerrorMsg == TWI_MTX_ADR_NACK ) | ( TWIerrorMsg == TWI_MRX_ADR_NACK ) )
		TWI_Start_Transceiver();

	return TWIerrorMsg;
}