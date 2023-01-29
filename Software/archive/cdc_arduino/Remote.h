#ifndef REMOTE_h
#define REMOTE_h

#include <Arduino.h>
#include "BK3254.h"
#include <avr/interrupt.h>
#include <SoftwareSerial.h>

//#define NoneRemote
#define SteringWheelRemote
//#define OneButtonRemote

//#define DEBUG



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
  
private:
  enum call : uint8_t;
  bool longpress;
  uint8_t pinMute;

  void checkSerial(void);
  void checkRemoteCommand(void);
  
};


#endif
