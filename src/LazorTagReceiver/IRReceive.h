#ifndef IRReceive_h
#define IRReceive_h

#ifdef __cplusplus
extern "C" {

#include "Arduino.h"

void IRReceiveInit();
boolean isDataAvailable();
boolean getData ( uint16_t copy[] );
void resetData();

}

#endif
#endif