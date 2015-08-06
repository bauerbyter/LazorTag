#define MAX_BIT 20

const int irRecPin = 2;

volatile unsigned long data[MAX_BIT];
volatile int myCount;
volatile unsigned long myLastTimeStamp;
volatile bool myIsFinished = false;
volatile bool myHasError = false;

void setup() {
	Serial.begin(115200);

	pinMode(irRecPin, INPUT);
	pinMode(13, OUTPUT);
	resetData();
	attachInterrupt(0, Interrupt, CHANGE);
	Serial.println("los gehts");


t
}

void loop() {
	if (myIsFinished)
	{
		digitalWrite(13, HIGH);
		
		for (int j = 0; j < MAX_BIT; j++)
		{
			//Serial.println(data[j]);
		}		
		getData();		
		resetData();
		digitalWrite(13, LOW);
		
		myIsFinished = false;
	}

}

void getData()
{
	int result = 0;
	int counter = 0;
	for(int i = 0; i < MAX_BIT; i++)
	{
		if(i == 0)
		{
			if(data[i] <2200 || data[i] > 2600)
			{
				myHasError = true;
				Serial.println("HEADER fehlt");
			}
		}
		else if(data[i] >2800)
		{
			Serial.println("fertig");
			i = MAX_BIT;
		}
		else if(i%2 == 1 )
		{
			if(data[i] < 400 || data[i] > 800)
			{
				myHasError = true;
				Serial.print("OFF fehler bei: ");
				Serial.println(i);
			}
		}
		else
		{
			
			if(data[i] > 1000 & data[i] < 1400)
			{
				//Serial.print("Ist eine 1: ");
				//Serial.println(data[i]);
				result |= 1<<counter;
			}
			else if (data[i] > 400 & data[i] < 800) //Todo eigentlich unnötig
			{
				//Serial.print("Ist eine 0: ");
				//Serial.println(data[i]);
				//result &= 0<<counter;
			}
			else
			{
				myHasError = true;
				Serial.print("Bit fehler bei: ");
				Serial.println(i);
			}
			counter ++;
		}
		
	}
	Serial.println(result);
}

void Interrupt() {
	if (myIsFinished)
	{
		return;
	}
	unsigned long currentTime = micros();
	unsigned long duration = currentTime - myLastTimeStamp;
	if (myLastTimeStamp == 0 || duration > 4500)
	{
		myLastTimeStamp = micros(); //error
		return;
	}
	if (myCount >= MAX_BIT)
	{
		myHasError = true;
		myCount = 0;
		myLastTimeStamp = 0;
		myIsFinished = true;
		return;
	}

	data[myCount] = duration;
	myLastTimeStamp = currentTime;
	myCount++;

	if (duration >= 2800) {
		myCount = 0;
		myLastTimeStamp = 0;
		myIsFinished = true;
	}

}

void resetData()
{
	for (int i = 0; i < MAX_BIT; i++)
	{
		data[i] = 0;
	}
}

