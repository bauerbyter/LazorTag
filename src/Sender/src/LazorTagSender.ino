#include "IRSend.h"
#include <Wire.h>

void setup()
{
  Serial.begin(115200);
  initSend();
  sei();                        // enable interrupts
  Wire.begin(9);                // join i2c bus with address #8
  Wire.onReceive(receiveEvent); // register event
  Serial.println("Sender ready");
}

void loop()
{
  delay(100);
}

void receiveEvent(int howMany)
{
  byte x = Wire.read();           // receive byte as an integer

  Serial.print("Incoming TWI: "); // print the integer
  Serial.println(x);              // print the integer
  sendData(x);
}
