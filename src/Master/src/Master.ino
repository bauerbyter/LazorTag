#include "Display5110.h"
#include "GameData.h"
#include <Wire.h>

GameData myGameData = { 99, 0, 99 }; // display.bla(&myGameData);

int myTimestamp = 0;

const int myReceiverCount        = 5;                 // #define
int myReceivers[myReceiverCount] = { 2, 4, 8, 3, 6 }; // Addresses
#define SIZEOF(array) \
  (sizeof(array) / sizeof(*array))                    // int x =
                                                      // SIZEOF(myReceivers);

void SerialZeug()                                     // kann bald weg
{
  if (Serial.available() > 0)
  {
    // read the incoming byte:
    byte incomingInt = Serial.parseInt();
    Serial.print("I received: ");
    Serial.println(incomingInt);

    // while ( irSend.getIsRunning() ) {}
    Wire.beginTransmission(9); // transmit to device #8
    Wire.write(incomingInt);
    Wire.endTransmission();    // stop transmitting
  }
}

void setup()
{
  Wire.begin();         // join i2c bus (address optional for master)
  Serial.begin(115200); // start serial for output
  // Todo: initDisplay
}

void loop()
{
  SerialZeug();

  if (millis() - myTimestamp >= 250)
  {
    readReceiver();
  }

  // todo: if button was pressed, lass die sau raus.
}

void readReceiver()
{
  for (int i = 0; i < myReceiverCount; i++)
  {
    Wire.requestFrom(myReceivers[i], 2);

    while (Wire.available())
    {
      int incomingInt = Wire.read();

      if (incomingInt != 0) // 0 heißt gibt nix
      {
        // todo: in Array speichern
      }
    }
  }

  // todo: Wenn alles fertig die zahlen auswerten
  myTimestamp = millis();
}

// Todo: Interrupt welchen den buttonstate ändert und dann im loop wird gefeuert
