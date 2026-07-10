#ifndef REMOTE_h
#define REMOTE_h

#include <Arduino.h>
#include "bk3254.h"
#include <avr/interrupt.h>

//#define OnlyRadioRemote
#define SteeringWheelRemote
//#define OneButtonRemote

#define pinMute PD5

#define DEBUG

class Remote {
public:
  bool GotNewCode;
  bool CorrectNewCode;
  String RemoteNewCode;
  String PhoneCode;

  Remote();
  void initRemote(void);
  void protocol(void);
  void setNewCode(void);
  void handleInterruptFromRadioButton(void);
  void handleInterruptFromExternalButton(void);
  
private:
  bool longpress;

  void checkSerial(void);
  void decodeRemoteCommand(void);
  void handleShortPress(void);
  void handleLongPress(void); 
  void updateMuteOutput(void);
};


#endif
