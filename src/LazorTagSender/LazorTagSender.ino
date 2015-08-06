#define HEADER 600 //2400ms
#define END 750 //3000ms
#define ONE 300 //1200ms
#define ZERO 150 //600ms
#define OFF 150 // 600ms 
#define MAXBITS 8


const int IRLedPin = 11;
volatile int myCounter = 0;
volatile bool myIsFinished = false;
int myData = 0;

void setup(){
	Serial.begin(115200);
	pinMode(IRLedPin, OUTPUT);
	bla();
	
}

void loop()
{
	if (Serial.available() > 0) {
		// read the incoming byte:
		int incomingByte = Serial.parseInt();
		
		// say what you got:
		Serial.print("I received: ");
		Serial.println(incomingByte);
		myIsFinished = false;
		sendData(incomingByte);
	}
}

//---------------------------------------------
//-----------------IRSend2----------------------
//---------------------------------------------

void bla( ){
	//38khz Timer2
	TCCR2A = 0;     // set entire TCCR2A register to 0
	TCCR2B = 0;     // same for TCCR2B
	TCCR2A |= (1 << WGM21);
	TCCR2B |= (1 << CS20);
	OCR2A = 209; // oder 210 --SET TOP COUNTER2 VALUE (FOR CTC)
	
	//Timer1 for duration
	TCCR1A = 0;     // set entire TCCR1A register to 0
	TCCR1B = 0;     // same for TCCR1B
	TCCR1B |= (1 << WGM12);	// Mode 4, CTC on OCR1A
	TIMSK1 |= (1 << OCIE1A);//Set interrupt on compare match
	
	sei();	// enable interrupts
}

void sendData(int data){
	if(myIsFinished){
		return;
	}
	myData = data;
	myCounter = 0;
	OCR1A = HEADER;
	digitalWrite(IRLedPin, HIGH);
	startTimer1();
	startTimer2();
}

ISR (TIMER1_COMPA_vect)
{
	if(myCounter >= MAXBITS +1) //+1 because of END
	{
		stopTimer1();
		stopTimer2();
		digitalWrite(IRLedPin, LOW);
		myCounter = 0;
		myData = 0;
		myIsFinished = true;
	}
	else{
		if( TCCR2A & (1 << COM2A0)) //if HIGH
		{
			stopTimer2();
			digitalWrite(IRLedPin, LOW);
			OCR1A = OFF;
		}
		else
		{
			if(myCounter == MAXBITS )
			{
				OCR1A = END;
			}
			
			else if(myData & (1<< myCounter)) //Highest bit first
			{
				OCR1A = ONE;
			}
			else
			{
				OCR1A = ZERO;
			}
			startTimer2();
			myCounter++;
		}
	}
}


void startTimer1(){
	TCCR1B |= (1 << CS11) | (1 << CS10);// set prescaler to 64 and start TIMER1 (micro-Timer)
}

void startTimer2(){
	//TCCR2B|= (1 << CS20); //Start Timer 2 (38khz) - no prescaler
	TCCR2A |= (1 << COM2A0); //turn on
}

void stopTimer1(){
	TCCR1B &= ~((1 << CS10) |(1 << CS11)); //Set Bit to 0 => StopTimer
}

void stopTimer2(){
	//TCCR2B &= ~(1 << CS20); //=>StopPWM
	TCCR2A &= ~(1 << COM2A0);
}

void toggleTimer2(){
	TCCR2A ^= (1 << COM2A0); //Toggle PWM on/off
}