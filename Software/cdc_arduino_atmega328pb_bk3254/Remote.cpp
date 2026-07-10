#include "Arduino.h"
#include "Remote.h"
#include "bk3254.h"
#include <avr/interrupt.h>

Remote remote;
extern BK3254 bt;

Remote::Remote() {
  GotNewCode = false;
  CorrectNewCode = false;
  RemoteNewCode = "";
  longpress = false;
}

void Remote::initRemote(void) {
  pinMode(pinMute, OUTPUT);
  digitalWrite(pinMute, LOW);

  #ifdef SteeringWheelRemote
    #ifdef DEBUG 
    Serial.println("Steering wheel remote control");
    #endif
  #endif

  #ifdef OneButtonRemote
  attachInterrupt(digitalPinToInterrupt(2), handleInterruptFromExternalButton, RISING);
    #ifdef DEBUG
    Serial.println("External button control");
    #endif
  #endif

  #ifdef OnlyRadioRemote
    #ifdef DEBUG
    Serial.println("Control from radio only");
    #endif
  #endif
}

void Remote::checkSerial(void) {
  if (Serial.available() > 0) { // if we have something
    while (Serial.available()) {
      char sign = (char)Serial.read();
      RemoteNewCode += sign;
      if (sign == '\n') {
        GotNewCode = true;

        #ifdef DEBUG
        Serial.print("Recived remote code: ");
        Serial.println(RemoteNewCode);
        #endif

        decodeRemoteCommand();
      }
    }
  }
}

void Remote::decodeRemoteCommand(void) {
  if (RemoteNewCode == "phone press\n") {
    CorrectNewCode = true;
    longpress = false;
    #ifdef DEBUG
    Serial.println("Short press phone");
    #endif
  } else if (RemoteNewCode == "phone longpress\n") {
    CorrectNewCode = true;
    longpress = true;
    #ifdef DEBUG
    Serial.println("Longpress phone");
    #endif
  } else {
    CorrectNewCode = false;
  }

  RemoteNewCode = "";
  GotNewCode = false;
}

void Remote::handleInterruptFromExternalButton() {
  CorrectNewCode = true;
}

void Remote::handleInterruptFromRadioButton() {
  CorrectNewCode = true;
  longpress = true;
}

void Remote::handleShortPress() {
  Serial.println(bt.CallState);
}

void Remote::handleLongPress() {
  switch (bt.CallState) {
    case BK3254::IncomingCall:
      bt.callAnsware();
      break;
    case BK3254::CallInProgress:
    case BK3254::OutgoingCall:
      bt.callHangUp();
      break;
    case BK3254::Idle:
      bt.callRedial();
      break;
  }
}

void Remote::updateMuteOutput() {
  bool callActive =
    (bt.CallState == BK3254::IncomingCall) ||
    (bt.CallState == BK3254::CallInProgress) ||
    (bt.CallState == BK3254::OutgoingCall);

  digitalWrite(pinMute, callActive ? HIGH : LOW); //if callActive is true then set pinMute to HIGH state, else set to LOW
}

void Remote::protocol(void) {
  #ifdef SteeringWheelRemote
  checkSerial();
  #endif

  if (CorrectNewCode) {
    if (!longpress)
      handleShortPress();
    else
      handleLongPress();

    CorrectNewCode = false;
    longpress = false;
  }

  updateMuteOutput();
}
