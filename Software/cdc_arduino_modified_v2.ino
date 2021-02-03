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
   - bluetooth BK3266 module 
   - telephone comunication (music and calling)


 *****************************************************************************/
#include "bk3266.h"
#include "cdc_function.h"
#include "Remote.h"
#include <SoftwareSerial.h>

int main()
{
  Serial.begin(9600);
  
  Init_VWCDC();
  InitRemote();
  InitBK3266();

  while (1)
  {
    #ifdef CHIP_BK3266
        CheckStateBK3266();
    #endif
    #ifdef CHIP_BK3254
       CheckStateBK3254();
    #endif
    
    CDC_Protocol();
    RemoteProtocol();
  } 
}
