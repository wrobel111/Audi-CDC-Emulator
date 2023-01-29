/*****************************************************************************


   Audi/Volkswagen Audi Interface
   
   Target: arduino atmega328(tested)


   Orginal Author: Vincent

   Date: January, 2011

   Version: 0.01


   Originally ported from VWCDPIC Project:

   http://www.k9spud.com/vwcdpic/

   Ported from Matthias Koelling's AVR Code:

   www.mikrocontroller.net/attachment/31279/vwcdc.c

   ---------------------------------
   16. Mar 2015:

   https://github.com/tomaskovacik/
   arduino duemilanove works:

   RADIO PIN -> arduino pin

   DataOut   -> digital 8 (ICP1)
   DataIn    -> digital 11(PB3)
   Clock     -> digital 13(PB5)

   computer -> arduino pin
   serial TX -> digital 0 (PD0)
   serial RX -> digital 1 (PD1)

   19. Mar 2015  tomaskovacik
   port to atmega8

   14.Jun 2019 kovo
   this compile fine against mega324(P/A/PA/PB)
   
   ---------------------------------
   Mateusz Wrobel 27. Sep 2020
   - bluetooth BK3254, BK3266 module 
   - telephone comunication (music and calling)

 *****************************************************************************/
//#include "bluetooth_module.h"
#include "cdc_function.h"
#include "BK3254.h"
#include "Remote.h"
#include <SoftwareSerial.h>

extern SoftwareSerial swSerial;
extern BK3254 BT;
extern Remote remote;

extern volatile int test2;
extern int counter_10ms_u8;

void setup()
{
  Serial.begin(9600);
  Serial.println("Hello world!");
  Init_VWCDC();
  Serial.println("Finish init CDC");
  BT.begin(9600);
  Serial.println("Finish init Bluetooth");
  remote.initRemote();
  Serial.println("Finish init remote");

  BT.getInitStates();
  Serial.println("Finish setup()");
}
void loop() 
{
  Serial.println(test2);
  Serial.println(counter_10ms_u8);
  CDC_Protocol();
  BT.checkState();
  remote.protocol();
}

