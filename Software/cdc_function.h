#ifndef VAGRadioRemote_h
#define VAGRadioRemote_h

#include <Arduino.h>
#include <avr/io.h>
#include <avr/sfr_defs.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <util/delay.h>

#define TRUE 1
#define FALSE 0

#define BLUETOOTH

#define DEBUG

//#define JUST_HEX_TO_SERIAL

//#define DUMPMODE
//#define DUMPMODE2

// Low period of PWTX line:
// 0: ~650us
// 1: ~1.77ms
// S: ~4.57ms

// one tick is 0.5µs
#define STARTTHRESHOLD  6400       // greater than this signifies START bit
#define HIGHTHRESHOLD   2496       // greater than this signifies 1 bit.
#define LOWTHRESHOLD     512       // greater than this signifies 0 bit.
#define PKTSIZE          -32       // command packets are 32 bits long.

// do not refresh head unit faster than 5.5ms (currently not implemented)
// 5.24s slow refresh rate when head unit in FM/AM/Tape mode (not implemented)

//#define REFRESH_PERIOD   50       // default to refresh head unit every 50.0ms
#define SECONDWAIT      -20         // wait 20 * 50ms to get 1 second (50ms*20 = 1000ms = 1s)
#define POWERIDENTWAIT  -10         // wait 10 * 1s to 10 seconds between VWCDPICx.x display
#define SCANWAIT        -100        // wait 100 * 50ms to get 5sec (50ms*100 = 5000ms = 5s)
#define _10MS           156
#define _50MS           500
#define _700US          7

#define TX_BUFFER_END   12
#define CAP_BUFFER_END  24

#define VER_MAJOR       '1'
#define VER_MINOR       '1'
#define VER_PATCHLEVEL  'a'

#if defined(__AVR_ATmega88__) || defined(__AVR_ATmega88A__) || defined(__AVR_ATmega328__) || defined(__AVR_ATmega328A__) || defined(__AVR_ATmega328P__) || defined(__AVR_ATmega328PA__) || defined(__AVR_ATmega328PB__)
#define RADIO_COMMAND      PB0 //ICP  8
#define RADIO_COMMAND_DDR  DDRB
#define RADIO_COMMAND_PORT  PORTB
#define RADIO_COMMAND_PIN PINB
#define RADIO_CLOCK        PB5  //13
#define RADIO_CLOCK_DDR    DDRB
#define RADIO_CLOCK_PORT  PORTB
#define RADIO_DATA         PB3  //11
#define RADIO_DATA_DDR     DDRB
#define RADIO_DATA_PORT  PORTB
#define RADIO_ACC 3 // PD3 = INT1
#endif



#if defined(__AVR_ATmega324__) || defined(__AVR_ATmega324P__) || defined(__AVR_ATmega324A__) || defined(__AVR_ATmega324PA__) || defined(__AVR_ATmega324PB__)
#define RADIO_COMMAND      PD6 //ICP 
#define RADIO_COMMAND_DDR  DDRD
#define RADIO_COMMAND_PORT  PORTD
#define RADIO_COMMAND_PIN PIND
#define RADIO_CLOCK        PB7
#define RADIO_CLOCK_DDR    DDRB
#define RADIO_CLOCK_PORT  PORTB
#define RADIO_DATA         PB5
#define RADIO_DATA_DDR     DDRB
#define RADIO_DATA_PORT  PORTB
#define RADIO_ACC 3
#endif


// Command Codes
// -------------
//
// First byte is always 53
// Second byte is always 2C
// Third and Fourth bytes always add up to FF
// All command codes seem to be a multiple of 4.
//
//
// 53 2C 0C F3 CD 1
// 53 2C 10 EF DISABLE
// 53 2C 14 EB Change CD (ignored)
// 53 2C 18 E7 Previous CD (only on Audi Concert <)
// 53 2C 2C D3 CD 5 (first packet)
// 53 2C 38 C7 Change CD/Next CD (aka MINQUIRY)
// 53 2C 4C B3 CD 3 (first packet)
// 53 2C 58 A7 Seek Back Pressed
// 53 2C 60 9F Mix 1
// 53 2C 68 97 Up on Mk3 premium (Adam Yellen)
// 53 2C 78 87 Dn
// 53 2C 8C 73 CD 2 (first packet)
// 53 2C A0 5F Scan
// 53 2C A4 5B something to do with power on (Audi Concert)
// 53 2C A8 57 Dn on Mk3 premium (Adam Yellen <adam@yellen.com>)
// 53 2C AC 53 CD 6 (first packet)
// 53 2C CC 33 CD 4 (first packet)
// 53 2C D8 27 Seek Forward Pressed
// 53 2C E0 1F Mix 6
// 53 2C E4 1B ENABLE
// 53 2C F8 07 Up


//#define  Do_PLAY           0x08  // mix button held down (RCD300 head unit only)
#define  Do_PLAY           0x03  // mix button held down (RCD300 head unit only)
#define  Do_LOADCD         0x01  // not used
#define  Do_ENABLE_MK      0x08  // mk concert1
#define  Do_CD1            0x0C  // CD 1
#define  Do_DISABLE        0x10  // DISABLE
#define  Do_CHANGECD       0x14  // Change CD (changer ignores & no ACK)
#define  Do_PREVCD         0x18  // PREVIOUS CD (Audi Concert head unit)
#define  Do_CD5            0x2C  // CD 5
#define  Do_TP             0x30  // TP info (TP button pressed)
#define  Do_SEEKFORWARD_MK 0x38  // mk concert 1 LOAD CD (aka MINQUIRY).
// Also means "Next CD" if no CD button pressed
#define  Do_CD3            0x4C  // CD 3
#define  Do_SEEKBACK       0x58  // SEEK BACK
#define  Do_MIX_CD         0x60  // MIX 1 (mix tracks within one disc)
#define  Do_UP_MK3         0x68  // UP (Mk3 head unit)
#define  Do_DOWN           0x78  // DOWN
#define  Do_CD2            0x8C  // CD 2
#define  Do_SCAN           0xA0  // SCAN
#define  Do_UNKNOWNCMD     0xA4  // power on CD mode?? (Audi Concert head unit)
#define  Do_DOWN_MK3       0xA8  // DOWN (Mk3 head unit only)
#define  Do_CD6            0xAC  // CD 6
#define  Do_CD4            0xCC  // CD 4
#define  Do_SEEKFORWARD    0xD8  // Seek Forward
#define  Do_MIX            0xE0  // MIX 6 (mix tracks across all discs)
#define  Do_ENABLE         0xE4  // ENABLE
#define  Do_UP             0xF8  // UP

#if defined(__AVR_ATmega8__) || defined(__AVR_ATmega128__)
#define TCCR2A TCCR2
#define TCCR2B TCCR2
#define COM2A1 COM21
#define COM2A0 COM20
#define OCR2A OCR2
#define TIMSK2 TIMSK
#define OCIE2A OCIE2
#define TIMER2_COMPA_vect TIMER2_COMP_vect
#define TIMSK1 TIMSK
#define TIFR1 TIFR
#define ICIE1 TICIE1
#define TIMSK0 TIMSK
#define TIFR2 TIFR
#define OCF2A OCF2
#define TCCR0B TCCR0
#endif




void Init_VWCDC(void);  
void OutputPacket(void);
void CDC_Protocol(void);

static void DecodeCommand(void);
static void ScanCommandBytes(void);
static void DumpFullCommand(void);
static uint8_t GetCaptureByte(void);
static void SetStateIdle(void);
static void SetStateTP(void);
static void SetStateIdleThenPlay(void);
static void SetStatePlay(void);
static void SetStateInitPlay(void);
static void SetStatePlayLeadIn(void);
static void SetStateTrackLeadIn(void);
static void SendDisplayBytes(void);
static void SendDisplayBytesNoCD(void);
static void SendDisplayBytesInitCD(void);
static void SendFrameByte(uint8_t byte_u8);
static void SendByte(uint8_t byte_u8);
static void EnqueueString(const uint8_t *addr PROGMEM);
static void EnqueueHex(uint8_t hexbyte_u8);
static void ResetTime(void);
static void SendStateIdle(void);
static void SendStateTP(void);
static void SendStatePlayLeadInEnd(void);
static void SendStateInitPlayEnd(void);
static void SendStateInitPlayAnnounceCD(void);
static void SendStatePlayLeadInAnnounceCD(void);
static void SendPacket(void);
static void android_buttons();
static void printstr_p(const char *s);
static uint8_t cdButtonPushed(uint8_t cdnumber);

uint8_t GetFlag100ms(void);
uint8_t GetFlag500ms(void);
void ClearFlag100ms(void);
void ClearFlag500ms(void);


#endif
