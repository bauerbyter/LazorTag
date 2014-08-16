#include "Arduino.h"
#include "GameData.h"

class IRReceiver
{
	public:
		IRReceiver();
		bool isAvailable();
		int getData();
		void clearData();	
	private:
		
};

