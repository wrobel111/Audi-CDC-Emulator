#ifndef led_h
#define led_h

#define pinLed PD6  //for cdc emulator board

void initLed(void);
void turnOnLed(void);
void turnOffLed(void);
void blinkLed(void);
void updateLedStatus(void);

#endif