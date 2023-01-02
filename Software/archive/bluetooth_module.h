#ifndef bluetooth_module_h
#define bluetooth_module_h

#include <Arduino.h>

#define CHIP_BK3254

//BT module basic status
#define DISCONNECTED      0
#define CONNECTED_IDLE    1
#define CALL_IN_PROGRESS  2
#define INCOMING_CALL     3
#define OUTGOING_CALL     4
#define AUX               5
#define USB               6

enum flowstate : uint8_t {
  START_52,
  START_42,
  START_23,
  PACKETSIZE,
  DATA,
  CRC,
  END_23,
  END_42,
  END_52
};

static flowstate state = START_52;

void InitBtModule(void);
void CheckStateBt(void);
void SendBt(String data);
uint8_t GetStatusBt (void);

void startFrame();
uint8_t senddata(uint8_t data);
uint8_t senddata(uint8_t data[]);
uint8_t pairingState();
uint8_t connect();
uint8_t disconnect();
uint8_t pickupCall();
uint8_t rejectcall();
uint8_t hangUpCall();
uint8_t redialLastCall();
uint8_t volumeUp();
uint8_t volumeDown();
uint8_t clearMemory();
uint8_t play();
uint8_t pause();
uint8_t stop();
uint8_t nextTrack();
uint8_t previousTrack();
uint8_t forward();
uint8_t rewind();
uint8_t getModuleName();
uint8_t getModuleAddr();
uint8_t phoneName();
uint8_t version();
uint8_t isAclLinked();
uint8_t callStatus();
uint8_t songStatus();
uint8_t getVolume();
uint8_t auxSearch();
uint8_t auxDisable();
uint8_t reset();
uint8_t idleMode();
uint8_t mute();
uint8_t unmute();
uint8_t clearKey();
uint8_t remoteAddr();
uint8_t transmit();
uint8_t receiver();
uint8_t setName(uint8_t data[]);
uint8_t call();
uint8_t search();
void endFrame();

uint8_t btCheckResponce();
uint8_t senddata(uint8_t data);
uint8_t senddata(uint8_t data[]);

String decodeResponce(uint8_t RSP);
String decodeReceivedData(uint8_t data);
String decodeCmdType(uint8_t cmd);
String decodeCmd(uint8_t cmd);

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
