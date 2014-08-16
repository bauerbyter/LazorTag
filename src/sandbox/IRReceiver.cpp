/*
* IRReceiver.cpp
 *
 * Created: 08.03.2014 09:28:53
 *  Author: bauerbyter
 */

#include "Arduino.h"
#include "IRReceiver.h"

#define HEADER_START 2200
#define HEADER_END 2600 //Todo
#define END_START 2800
#define ONE_START 900
#define ONE_STOP 1800
#define ZERO_START 400
#define ZERO_STOP 900
#define OFF_START 400
#define OFF_STOP 900


size_t const datalength = 8;
volatile byte recievedData[datalength];
volatile bool isReceivingComplete;
volatile bool isReceivingError;

enum
{
  IR_IDLE,
  IR_START,
  IR_ON,
  IR_OFF
}
volatile irState = IR_IDLE;
volatile unsigned long lastTimeStamp;
volatile int bitsRead;

void somethingWrong(int i, unsigned long time)
{
  Serial.print("Error: ");
  Serial.println(i);
  Serial.print("Time: ");
  Serial.println(time);
  isReceivingError = true;
  irState = IR_IDLE;
}

void infraredWatcher()
{
  bool isActive = (PIND & (1<<2)) == 0; //active low
  unsigned long timeStamp = micros();
  unsigned long timeBetween = timeStamp - lastTimeStamp;
  lastTimeStamp = timeStamp;

  if( isReceivingComplete ) //Cooldown
  {
    return;
  }
  switch (irState)
  {
  case IR_IDLE:
    if (isActive)
    {
      bitsRead = 0;
      irState = IR_START;
    }
    break;
  case IR_START:
    if (isActive)
    {
      somethingWrong(1, timeBetween);
    }
    else
    {
      if (HEADER_START <= timeBetween && timeBetween <= HEADER_END )
      {

        isReceivingError = false;
        irState = IR_OFF;

      }
      else
      {
        somethingWrong(2, timeBetween);
      }
    }
    break;
  case IR_OFF:
    if (OFF_START <= timeBetween && timeBetween <= OFF_STOP )
    {
      irState = IR_ON;

    }
    else
    {
      somethingWrong(3, timeBetween);
    }
    break;
  case IR_ON:
    if (ZERO_START <= timeBetween && timeBetween <= ZERO_STOP )
    {
      recievedData[bitsRead] = 0;
      bitsRead++;
      irState = IR_OFF;
    }
    else if (ONE_START <= timeBetween && timeBetween <= ONE_STOP )
    {
      recievedData[bitsRead] = 1;
      bitsRead++;
      irState = IR_OFF;
    }
    else if (timeBetween >= END_START)
    {
      if (bitsRead == datalength)
      {	
        irState = IR_IDLE;
        isReceivingComplete = true;
      }
      else
      {
        somethingWrong(4, timeBetween);

      }
    }
    else
    {
      somethingWrong(5, timeBetween);
    }
    break;
  }
}

IRReceiver::IRReceiver()
{
  pinMode(2,INPUT);
  isReceivingError = false;
  isReceivingComplete = false;
  attachInterrupt(0, infraredWatcher , CHANGE); //Use Interrupt 0 = Pin 2
}

bool IRReceiver::isAvailable()
{
  return isReceivingComplete;
}

void IRReceiver::clearData()
{
  isReceivingComplete = false;
  isReceivingError = false;
  memset((void*)recievedData,0,datalength);
}


int IRReceiver::getData()
{
  int result = 0;
  for (int i = 0; i < datalength; i++)
  {
    bitWrite(result,datalength-1-i,recievedData[i]);
  }
  return result;
}







