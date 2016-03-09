#include "Arduino.h"
   #include "Display5110.h"
   #include <SPI.h>
   #include <Adafruit_GFX.h>
   #include <Adafruit_PCD8544.h>

// Hardware SPI (faster, but must use certain hardware pins):
// SCK is LCD serial clock (SCLK) - this is pin 13 on Arduino Uno
// MOSI is LCD DIN - this is pin 11 on an Arduino Uno
// pin 5 - Data/Command select (D/C)
// pin 4 - LCD chip select (CS)
// pin 3 - LCD reset (RST)
Adafruit_PCD8544 display = Adafruit_PCD8544(5, 4, 3);

void initDisplay()
{
  display.begin();
  display.setContrast(60);

  display.clearDisplay(); // clears the screen and buffer
  // Show Logo
  display.display();
}

void drawGameData(struct GameData *gamedata)
{}
