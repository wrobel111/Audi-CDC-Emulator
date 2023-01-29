#include "Arduino.h"
#include "Remote.h"
#include "bk3254.h"
#include <avr/interrupt.h>
#include <SoftwareSerial.h>

Remote remote;
extern BK3254 bt;

enum Remote::call : uint8_t {
		IncomingCall,
		OutgoingCall,
		CallInProgress,
		Idle
	};

Remote::Remote()  {
  GotNewCode = false;
  CorrectNewCode = false;
  RemoteNewCode = "";
  pinMute = 5;
}

void Remote::initRemote(void) {
  pinMode(pinMute,OUTPUT);
  digitalWrite(pinMute, LOW);
  
  #ifdef SteringWheelRemote
  // comunication through uart, only Rx
    #ifdef DEBUG 
    Serial.println("Stering wheel remote control");
    #endif
  #endif

  #ifdef OneButtonRemote
  attachInterrupt(digitalPinToInterrupt(2), setNewCode, RISING);
    #ifdef DEBUG
    Serial.println("One button remote control");
    #endif
  #endif

  #ifdef NoneRemote
    #ifdef DEBUG
    Serial.println("No remote control");
    #endif
  #endif
}

void Remote::checkSerial(void)  {
  if (Serial.available() > 0) { // if we have something
    while (Serial.available())  {
      char sign = (char)Serial.read();
      RemoteNewCode += sign;
      if(sign == '\n')  {
        GotNewCode = true;
        #ifdef DEBUG
          Serial.println(RemoteNewCode);
        #endif
      }
    }
  }
}

void Remote::checkRemoteCommand(void) {
  if (GotNewCode == true) {
    if ((RemoteNewCode == "phone press\n")) {
      CorrectNewCode = true;
      longpress = false;
      #ifdef DEBUG
        Serial.println("phone button press");
      #endif
      
    }
    else if (RemoteNewCode == "phone longpress\n") {
      CorrectNewCode = true;
      longpress = true;
      #ifdef DEBUG
        Serial.println("phone button longpress");
      #endif
    }
    else {
      CorrectNewCode = false;
    }

    RemoteNewCode = ""; //clear buffer
    GotNewCode = false;
  }
}

void Remote::setNewCode() {
  CorrectNewCode = true;  //set flag
}

void Remote::protocol(void) {
  
  #ifdef SteringWheelRemote
  checkSerial();  //check serial buffer, was sth recived?
  checkRemoteCommand();
  #endif

  if (CorrectNewCode) {
    if(longpress == false) Serial.println(bt.CallState);
    if(longpress == true) {
      if(bt.CallState == IncomingCall) {
        bt.callAnsware();  //Answare the call
      }
      else if(bt.CallState == CallInProgress) {
        bt.callHangUp();  //Hang up the phone
      }
      else  if(bt.CallState == OutgoingCall){
        bt.callHangUp();  //Hang up the phone 
      }
      else  if(bt.CallState == Idle) {
        bt.callRedial();  //Last number redial
      }
    }
    CorrectNewCode = false;  //reset flag
    longpress = false;
  }

  if ((bt.CallState == IncomingCall) || (bt.CallState == CallInProgress) || (bt.CallState == OutgoingCall)) {
    digitalWrite(pinMute, HIGH);
  }
  else {
    digitalWrite(pinMute, LOW);
  }
}
