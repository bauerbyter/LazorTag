#include <Wire.h>

int timestamp = 0;

void setup() {
  Wire.begin();        // join i2c bus (address optional for master)
  Serial.begin(115200);  // start serial for output
}

void loop() {

  if ( Serial.available() > 0 )
  {
    // read the incoming byte:
    byte incomingInt = Serial.parseInt();
    Serial.print ( "I received: " );
    Serial.println ( incomingInt );
    //while ( irSend.getIsRunning() ) {}
    Wire.beginTransmission(9); // transmit to device #8
    Wire.write(incomingInt);
    Wire.endTransmission();    // stop transmitting
  }
  if(millis() - timestamp >= 500){
    Wire.requestFrom(8, 1);    // request 6 bytes from slave device #8

    while (Wire.available()) { // slave may send less than requested int i =
      byte b = Wire.read(); // receive a byte as character if(i != 0){ Serial.println(i); }
      if(b != 0){
        Serial.print("Got Hit: ");
        Serial.println(b);
      }
    }
    timestamp = millis();
  }
}
