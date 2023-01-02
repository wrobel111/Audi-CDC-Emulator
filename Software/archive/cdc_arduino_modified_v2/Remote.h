#ifndef REMOTE_h
#define REMOTE_h

#include <Arduino.h>

//#define SteringWheelRemote
//#define OneButtonRemote

void InitRemote(void);
void CheckRemote(void);
void RemoteProtocol(void);
void SetNewCode(void);
uint8_t GetNewCode(void);

#endif
