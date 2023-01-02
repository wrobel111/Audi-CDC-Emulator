#include "Remote.h"
#include "bk3266.h"
#include <avr/interrupt.h>
#include <SoftwareSerial.h>

volatile bool RemoteGotNewCode = false;
String RemoteNewCode = "";
String PhoneCode = "0xC0\r\n";
uint8_t pinMute = 5;

//SoftwareSerial SwSerial(2, -1); // RX, TX

void InitRemote(void) {
  
  #ifdef SteringWheelRemote
  attachInterrupt(digitalPinToInterrupt(2), SetNewCode, RISING);
  #endif

  #ifdef OneButtonRemote
  attachInterrupt(digitalPinToInterrupt(2), SetNewCode, RISING);
  #endif
}

void SetNewCode(void) {
  Serial.println("Received from steering wheel");
  RemoteGotNewCode = true;
}

void CheckRemote(void)  {
  if (Serial.available() > 0) { // if we have something
    while (Serial.available())  {
      char sign = (char)Serial.read();
      RemoteNewCode += sign;
      if(sign == '\n')  {
        RemoteGotNewCode = true;
      }
    }
  }
}

void RemoteProtocol(void) {
  if (RemoteGotNewCode) {
    Serial.println("Phone button");
      if(GetStatusBK3266() == INCOMING_CALL) {
        SendBK3266("BT+CA\r\n");  //Answare the call
      }
      else if(GetStatusBK3266() == CALL_IN_PROGRESS) {
        SendBK3266("BT+CE\r\n");  //Hang up the phone
      }
      else  if(GetStatusBK3266() == OUTGOING_CALL){
        SendBK3266("BT+CE\r\n");  //Hang up the phone 
      }
      else  if(GetStatusBK3266() == CONNECTED_IDLE) {
        SendBK3266("BT+CR\r\n");  //Last number redial
      }
    RemoteGotNewCode = false;  //reset flag
  }

  if ((GetStatusBK3266()== INCOMING_CALL) || (GetStatusBK3266()== CALL_IN_PROGRESS) || (GetStatusBK3266()== OUTGOING_CALL)) {
    digitalWrite(pinMute, HIGH);
  }
  else {
    digitalWrite(pinMute, LOW);
  }
}
