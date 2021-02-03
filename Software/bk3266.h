#ifndef BK3266_h
#define BK3266_h

#include <Arduino.h>

#define CHIP_BK3266
//#define CHIP_BK3254

//BT module status
#define DISCONNECTED      0
#define CONNECTED_IDLE    1
#define CALL_IN_PROGRESS  2
#define INCOMING_CALL     3
#define OUTGOING_CALL     4
#define AUX               5
#define USB               6

void InitBK3266(void);
void CheckStateBK3266(void);
void CheckStateBK3254(void);
void SendBK3266(String data);
uint8_t GetStatusBK3266 (void);

#endif

/*
  AX_PA\n   AUX Playing status
  AX_PU\n   AUX In a suspended state
  BT_AC\n   Bluetooth Even being back
  BT_CN\n   Bluetooth connected
  BT_EC\n   Bluetooth He is busy
  BT_IC\n   Bluetooth A call
  BT_WP\n   Bluetooth In pairing state
  BT_WC\n   Bluetooth connection is in wait state
  BT_OC\n   Bluetooth Telephone shot
  BT_PA\n   Bluetooth Now Playing
  EEROR\n   error
  FM_FQ = 1081\n  Tunner frequency
  FM_PA\n   FM You are listening state
  FM_PU\r\n   FM In a suspended state
  FM_SC\n   FM The state of being seized Taiwan
  IA\r\n  disconnect
  II\r\n  connection succeeded
  IRx\n   Received key value by IR
  music_mun = x\n   Song number
  OK\n
  ON\r\n  Bluetooth turned on
  PLAY_ALL\n  Repeat All Tracks (TF/SDcard Mode)
  PLAY_ONE\n   Repeat One Track (TF/SDcard Mode)
  SD_PA\n   SD Card playing status
  SD_PU\n   SD Card is paused
  SY_PO\n   Bluetooth turned on
  SY_PF\n   Bluetooth off
  UD_PA\n   U plate Playing status
  UD_PU\n   U plate In a suspended state
  VOLx\n  The current volume x level
  AD: 191919191919\r\n
  PN: 0000\r\n
  NA: BK3254\r\n
  connection succeeded: C1\r\n
  no connection: C0\r\n
  Play: MB\r\n /
  Idle: MA\r\n
  disconnect: M0\r\n
  connection: M1\r\n
  Caller: M2\r\n
  Outgoing: M3\r\n
  calling: M4\r\n
  IR-"+123456789"
*/
