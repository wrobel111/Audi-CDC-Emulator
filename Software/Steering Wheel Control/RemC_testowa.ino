#include "VAGRadioRemote.h"
#include <SoftwareSerial.h>


#define REMOTE_IN 2
#define REMOTE_OUT 1
#define COMUNICATION_PIN 4

//SoftwareSerial mySerial(0, 4); // RX, TX
VAGRadioRemote remote(REMOTE_OUT, REMOTE_IN);
 
void setup() {
  //mySerial.begin(9600);
  remote.begin();
  pinMode(COMUNICATION_PIN,OUTPUT);
  digitalWrite(COMUNICATION_PIN,LOW);
}

void loop() {
  if (remote.gotNewCode()) {
    if(remote.newCode()==0x40)  { //if R/T button
      remote.send(0xA0);  //change mode 0xA0
    }
    else if(remote.newCode()==0xC0) { //if phone button
      digitalWrite(COMUNICATION_PIN, HIGH);
      delay(10);
      digitalWrite(COMUNICATION_PIN,LOW);
    }
    else {  //if other button send normal comand
      remote.send(remote.newCode());
    }
    remote.clearGotNewCode();
  }    
}
