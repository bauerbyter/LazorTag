#include <Wire.h>

void setup() {
  Wire.begin();        // join i2c bus (address optional for master)
  Serial.begin(115200);  // start serial for output
}

void loop() {
  Wire.requestFrom(8, 1);    // request 6 bytes from slave device #8

  while (Wire.available()) { // slave may send less than requested
    int i = Wire.read(); // receive a byte as character
    if(i != 0){
      Serial.println(i);
    }

  }

  delay(10);
}
