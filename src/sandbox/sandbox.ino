#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
#include <Wire.h>

#include "IRReceiver.h"
#include "IRSender.h"
#include "GameData.h"

Adafruit_PCD8544 static display = Adafruit_PCD8544(7, 6, 5, 4, 3);
const int fireButtonPin = 14; //ToDo, add here all Pins

IRReceiver myReceiver;
IRSender mySender;
GameData myGameData;

unsigned long lastShot;
int waitAfterFire = 1000;

void setup()
{
  Serial.begin(115200);
  Wire.begin();
  pinMode(fireButtonPin,INPUT_PULLUP);
  display.begin();
  display.clearDisplay();
  display.setContrast(62);
  welcome();
  Serial.println("Ready");

}

void loop()
{
  if(myGameData.getHealth() <= 0) //Game Over
  {
    Serial.println("Game Over");
    gameOver();
    while (1)
    {

    }
  }
  else{
    if(myReceiver.isAvailable())
    {
      Serial.print("incoming Data");
      int result = myReceiver.getData();
      myReceiver.clearData();	
      Serial.println(result);
      interpretData(result);


    }
    if(digitalRead(fireButtonPin) == LOW && allowToFire())
    {
      Serial.println("FIRE");
      mySender.sendIR(myGameData.getPlayer(),1);
      myGameData.setAmmo(myGameData.getAmmo()-1);
      lastShot = millis();
      writeDataToDisplay();
      Wire.beginTransmission(1); // transmit to device #1
      Wire.write(1);              // sends one byte
      Wire.endTransmission();    // stop transmitting
    }
  }
}
//-----------------------------------Display
void welcome()
{
  display.setTextSize(2);
  display.setCursor(0,0);
  display.println(" Ready\n  to\nRumble?");
  display.display();
  //while(digitalRead(fireButtonPin) == HIGH){}
  delay(2000);
  writeDataToDisplay();


}

void gameOver()
{
  display.setTextSize(2);
  display.setCursor(0,10);
  display.println("Game\nOver");
  display.display();

}

void writeDataToDisplay()
{
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0,0);
  display.write(3);
  display.print(myGameData.getHealth());
  display.setCursor(60,0);
  display.write(15);
  display.print(myGameData.getAmmo());
  display.display();


}
//-----------------------------------
bool allowToFire()
{
  if(millis() - lastShot > waitAfterFire)
  {
    return true;
  }
  return false;
}

void interpretData(int incomingData)
{
  int temp = incomingData;
  temp = temp >> 4;
  Serial.print("player");
  Serial.println(temp);
  if(temp == myGameData.getPlayer())
  {
    //error
  }
  else
  {
    gotHit(incomingData);
  }

}

void gotHit( int incomingData)
{
  myGameData.setHealth( myGameData.getHealth()- (incomingData & B00001111));
  Serial.print("health");
  Serial.println(myGameData.getHealth());
  writeDataToDisplay();
  Wire.beginTransmission(1); // transmit to device #1
  Wire.write(2);              // sends one byte
  Wire.endTransmission();    // stop transmitting
}




