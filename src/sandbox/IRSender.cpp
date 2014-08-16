#include "Arduino.h"
#include "IRSender.h"


#define HEADER 38400
#define END 48000
#define ONE 19200
#define ZERO 9600
#define OFF 9600

const int IRLedPin = 11;
const int muzzleLed= 8;

volatile int bitSended = 0;
volatile byte message = 0;

void initSendTimer()
{
  pinMode(IRLedPin, OUTPUT);
  pinMode(muzzleLed, OUTPUT);

  cli();
  TCCR1A = 0;     // set entire TCCR1A register to 0
  TCCR1B = 0;     // same for TCCR1B
  // turn on CTC mode:
  TCCR1B |= (1 << WGM12);
  // enable timer compare interrupt:
  TIMSK1 |= (1 << OCIE1A);

  TCCR2A = 0;     // set entire TCCR2A register to 0
  TCCR2B = 0;     // same for TCCR2B
  // set compare match register to desired timer count:
  // turn on CTC mode:
  TCCR2A |= (1 << WGM21);
  TCCR2B |= (1 << CS20);
  OCR2A =  208;          // 38khz

  // enable global interrupts:
  sei();
}

void startTimer(){
  digitalWrite(muzzleLed, HIGH);
  bitSended = 0;
  OCR1A = HEADER; //ToDo Variable
  TCCR1B |= (1 << CS10); //turn on
  TCCR2A |= (1 << COM2A0); //turn on
}


void sendIR(int player, int damage)
{
  message = player << 4;
  message = message | damage;
  startTimer();
}



ISR(TIMER1_COMPA_vect)
{
  digitalWrite(muzzleLed, HIGH);
  if(bitSended == 9) //Finished //Todo als Variable
  {
    TCCR1B &= ~(1 << CS10); //Set Bit to 0 => StopTimer
    TCCR2A &= ~(1 << COM2A0); //=>StopPWM
    digitalWrite(IRLedPin, LOW); //Be sure LED is OFF

    bitSended = 0; //reset
    message = 0;

    digitalWrite(muzzleLed, LOW);
  }

  else{

    if( TCCR2A & (1 << COM2A0)) //if HIGH
    {
      TCCR2A &= ~(1 << COM2A0); //turn off
      digitalWrite(IRLedPin, LOW);
      OCR1A = OFF;

    }
    else
    {
      if(bitSended == 8 )
      {
        OCR1A = END;
      }
      else if(bitRead(message, 7-bitSended)) //Highest bit first
      {
        OCR1A = ONE;
      }
      else
      {
        OCR1A = ZERO;
      }
      TCCR2A |= (1 << COM2A0); //turn on
      bitSended++;
    }
  }
  digitalWrite(muzzleLed, LOW);
}

