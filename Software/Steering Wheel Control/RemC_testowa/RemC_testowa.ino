/* 
audi steering wheel decoder

ported for : attiny 85 (digispark) 16MHz
*/

#include "VAGRadioRemote.h"
#include <SoftwareSerial.h>


#define REMOTE_OUT 0
#define REMOTE_IN 2
#define COMUNICATION_PIN 4
#define SERIAL_TX_PIN 1
#define LONGPRESS_COUNT 20

SoftwareSerial mySerial(0, SERIAL_TX_PIN); // RX, TX
VAGRadioRemote remote(REMOTE_OUT, REMOTE_IN);

uint8_t phoneCounter = 0;
 
void setup() {
  mySerial.begin(9600);
  mySerial.println("Steering wheel decoder");
  remote.begin();
  pinMode(REMOTE_OUT,OUTPUT);
  //digitalWrite(COMUNICATION_PIN,LOW);
  mySerial.println("setup finish");
}

void loop() {
  if (remote.gotNewCode()) {
    if(remote.newCode()==0x40)  { //if R/T button code
      remote.send(0xA0);  //change mode 0xA0
    }
    else if(remote.newCode()==0xC0) { //if phone button code
      phoneCounter++;
      if(phoneCounter >= LONGPRESS_COUNT) {  //longpress detection
        mySerial.print("phone longpress\n");
        phoneCounter = 0;
      }
      else {
        mySerial.print("phone press\n");
      }
      
    }
    else {  //if other button send normal comand
      remote.send(remote.newCode());
    }
    remote.clearGotNewCode();
  }    
}
