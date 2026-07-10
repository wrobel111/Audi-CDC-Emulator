#include "Arduino.h"
#include "led.h"
#include "bk3254.h"

extern BK3254 bt;         // Twój obiekt BT
extern volatile uint8_t flag_250ms_led;  // Flaga do migania
uint8_t ledState = LOW;

void initLed(void){
  pinMode(pinLed, OUTPUT);
}

void turnOnLed(void){
  digitalWrite(pinLed, HIGH);
}

void turnOffLed(void){
  digitalWrite(pinLed, LOW);
}

void blinkLed(void){
  if (flag_250ms_led) {
    flag_250ms_led = false;
    ledState = !ledState;
    digitalWrite(pinLed, ledState);
  }
}

void updateLedStatus() {
  // Sprawdź stan BT i ustaw LED odpowiednio
  switch(bt.BTState) {
    case BK3254::Connected:
      // Połączenie aktywne: LED ON
      turnOnLed();
      break;

    case BK3254::Disconnected:
    case BK3254::Pairing:
    case BK3254::Connecting:
      // Migaj LED-em co 250 ms
      blinkLed();
      break;

    default:
      // Inne stany: LED OFF
      turnOffLed();
      break;
  }
}
