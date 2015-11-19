#ifndef IRSend_h
#define IRSend_h

#ifdef __cplusplus
extern "C" {
# include "Arduino.h"

void    initSend();
void    sendData(uint16_t data);
boolean getIsRunning();
}
#endif /* ifdef __cplusplus */
#endif /* ifndef IRSend_h */
