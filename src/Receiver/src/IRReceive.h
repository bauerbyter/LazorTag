#ifndef IRReceive_h
#define IRReceive_h

#ifdef __cplusplus
extern "C" {
# include "Arduino.h"

void    IRR_Init();
boolean IRR_IsDataAvailable();
boolean IRR_GetData(uint16_t copy[]);
boolean IRR_HasError();
void    IRR_ResetData();
}

#endif /* ifdef __cplusplus */
#endif /* ifndef IRReceive_h */
