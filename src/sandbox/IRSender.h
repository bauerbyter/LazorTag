#include "Arduino.h"

class IRSender
{
 private:

 public:
	IRSender();
	void sendIR(int player, int damage);
        void startTimer();
};


