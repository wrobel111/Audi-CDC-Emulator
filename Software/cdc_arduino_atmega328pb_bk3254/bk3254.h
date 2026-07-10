#ifndef BK3254_h
#define BK3254_h

#include <Arduino.h>

#define CHIP_BK3254

//#define DEBUG

#define INITVOLUME "C"
#define resetBTpin 2

// UART COMMANDS

// works only on v1.2 
#define BK3254_CHANGE_NAME "SNAME+" //change name
#define BK3254_VOICES_ON "TONEON"
#define BK3254_VOICES_OFF "TONEOFF"
#define BK3254_GET_VOICES_STATE "MTONE"
#define BK3254_GOBACKON "GOBACKON"
#define BK3254_GOBACKOFF "GOBACKOFF"
#define BK3254_GET_GOBACK_STATE "MGOBACK"
#define BK3254_CALLON "CALLON"
#define BK3254_CALLOFF "CALLOFF"
#define BK3254_GET_CALL_STATE "MCALL"
#define BK3254_REBOOT "REBOOT"
#define BK3254_AUTOPLAYON "MP3AUTOPLYON" //
#define BK3254_AUTOPLAYOFF "MP3AUTOPLYOFF" //
#define BK3254_GET_AUTOPLAY_STATE "MMP3AUTOPLY" //
//probably work only with V1.3 based on this: https://github.com/tomaskovacik/kicad-library/blob/master/library/datasheet/F-6888_bk3254/BK3254_V1.3_HYT.zh-CN.en.pdf
// on 1.2 firmware this only disconnect BT
#define BK3254_CHANGE_PIN "SPIN+"

//works with v1.1 for sure:
#define BK3254_PAIRING "PR" //Pairing   
#define BK3254_PAIRING_DEVICE "AC" //The last paired device connected (connect to last device?)  
#define BK3254_DISCONNECT "DC" //Disconect   
#define BK3254_CALL_ANSWARE "CA" //Answare the call  
#define BK3254_CALL_REJECT "CJ" //To reject a call  
#define BK3254_CALL_HANGUP "CE" //Hang up the phone   
#define BK3254_CALL_REDIAL "CR" //Last number redial  
#define BK3254_MUSIC_TOGGLE_PLAY_PAUSE "PP" //Music Play / Pause
#define BK3254_MUSIC_PLAY "PA" //Music Play
#define BK3254_MUSIC_PAUSE "PU" //Music Pause
#define BK3254_MUSIC_NEXT_TRACK "PN" //next track/ FM next station   
#define BK3254_MUSIC_PREVIOUS_TRACK "PV" //previous piece/ FM On one   
#define BK3254_VOLUME_UP "VP" //Volume Up   
#define BK3254_VOLUME_DOWN "VD" //Volume down   
#define BK3254_VOLUME_SET "V" //+x: 0 - 16  Set the volume  com+correct: COM_VOLx\n / error: ERR\n
#define BK3254_VOLUME_GET_CURRENT "GV" //Query current volume  correct: VOLx\n (x:0-16) / error: ERR\n
#define BK3254_SHUTDOWN "PWD" //Shutdown  
#define BK3254_STANDBY "PWDS" //Soft-Off  
#define BK3254_POWERUP "PWOS" //Soft Power   ???
#define BK3254_INPUT_SWITCH "MC" //Switching to the next operating mode  
#define BK3254_INPUT_BLUETOOTH "MBT" //Bluetooth mode  
#define BK3254_INPUT_TF "MSD" //TF/SDcard Mode (if available)   
#define BK3254_INPUT_AUX "MAX" //AUX Mode (if available)   
#define BK3254_INPUT_FM "MFM" //FM Mode (if available)  
#define BK3254_INPUT_USB "MUD" //USB Disk mode (if available)  
#define BK3254_INPUT_GET_CURRENT "IQ" //Query the current mode  
#define BK3254_MUSIC_PLAYBACK_MODE_REPEAT_ALL "SMA" //Repeat All Tracks (TF/USB disk mode) COM+SMA\r\n correct: COM_SMA\n / error: ERR\n; from firmware 1.4: correct: PLAY_M0 \ n
#define BK3254_MUSIC_PLAYBACK_MODE_REPEAT_ONE "SMO" //Single loop (TF/USB disk mode) COM+SMO\r\n correct: COM_SMO\n / error: ERR\n; rom firmware 1.4: correct: PLAY_M1 \ n
#define BK3254_MUSIC_PLAYBACK_MODE_REPEAT_NONE "SMNO" //No single loop (TF/USB disk mode) COM+SMNO\r\n correct: COM_SMNO\n / error: ERR\n
#define BK3254_MUSIC_PLAYBACK_MODE_GET_CURRENT "GSM" //The current inquiry MP3 Play Mode (TF/USB disk mode) COM+SMR\r\n correct: COM_SMA\n / COM_SMO / COM_SMNO\n
#define BK3254_MUSIC_PLAY_SONG_NUMBER "SMP" //SMP+XXXX, Play selections (TF/USB disk mode)  XXXX: 0001-9999, 0001 Represents the 1 first
#define BK3254_TF_GET_SONG_NUMBER "MRMP3" //Query currently playing MP3 Song number   correct: music_mun = x\n , x: 1-9999 / error: ERR\n ???
#define BK3254_TF_USB_GET_NUMBER_OF_SONGS "MMMP3" //Query current mode MP3 The number of songs (TF/USB disk mode)   correct: MMMPx\n,x: 1-1999 / error: ERR\n
#define BK3254_USB_GET_SONG_NUMBER "MRUSB" //Query currently playing USB Disc song number  correct: music_mun = x\n,x: 1-1999 / error: ERR\n
#define BK3254_FM_START_SEARCH "SC" //FM start station search   
#define BK3254_FM_STOP_SEARCH "ST" //FM Stop station search  
#define BK3254_FM_GET_FREQ "GF" //Get the current radio frequency   FM_FQ = 875 ~ 1081
#define BK3254_FM_TUNE_PRESET "SFM" //SFM+XX,Select FM preset 01-99  correct: FM_FQ = 998\n / error: ERR\n
#define BK3254_FM_TUNE_FREQ "SETFM" //SETFM+XXX, XXX : 875 to 1081   set up FM frequency to XXX  correct: OK\n / error: ERR\n
#define BK3254_FM_GET_FREQ2 "MRFM" //Query current FM frequency (FM Mode)  correct: FM_FQ = xxx\n. xxx: 875-1081(87.5-108.1Mhz) / error: ERR\n ?? wht is diference to FM+GF ?
#define BK3254_FM_GET_PRESET "MMFM" //Query currently selected preset (FM Mode)   correct: MFMxx \ n,xx: 01-99 / error: ERR\n
#define BK3254_FM_GET_FREQ_OF_PRESET "MFFM" //MFFM+XX, Inquire FM of xx No. A frequency corresponding to (FM Mode)   correct: FM_FQ = xxx\n, error: ERR\n

//Query / feedback commands
#define BK3254_GET_ADDRESS "MR" //Queries Bluetooth address   AD: 191919191919\r\n
#define BK3254_GET_PIN_CODE "MP" //PIN Code query  PN: 0000\r\n , droped in firmware V1.4
#define BK3254_GET_NAME "MN" //Bluetooth name query  NA: BK3254\r\n
#define BK3254_GET_CONNECTION_STATUS "MO" //Bluetooth connection status inquiry   connection succeeded: C1\r\n / no connection: C0\r\n
#define BK3254_GET_MUSIC_STATUS "MV" //Bluetooth playback status inquiry   Play: MB\r\n / time out: MA\r\n / disconnect: M0\r\n
#define BK3254_GET_HFP_STATUS "MY" //Bluetooth inquiry HFP status  disconnect: M0\r\n / connection: M1\r\n / Caller: M2\r\n / Outgoing: M3\r\n / calling: M4\r\n
#define BK3254_GET_SW_VERSION "MQ" //GET SW VERSION

class BK3254
{
  public:

	enum music : uint8_t {
		Playing,
		Stopped
	};

	enum call : uint8_t {
		IncomingCall,
		OutgoingCall,
		CallInProgress,
		Idle
	};

	enum bluetooth : uint8_t {
		Connected,
		Disconnected,
		Pairing,
		Connecting
	};
	
	enum input : uint8_t {
		BT,
		AUX,
		USB,
		SD,
		FM
	};

  enum power : uint8_t {
		On,
		Off,
		ShutdownInProgress
	};

  enum playmode : uint8_t {
		RepeatAll,
		RepeatOne,
		RepeatNone,
		Shuffle
	};

  bluetooth BTState = Disconnected;
	call CallState = Idle;
	music MusicState = Stopped;
	input InputSelected = BT;
  power PowerState = Off;
  playmode ModeOfPlay = RepeatOne;

  uint8_t currentVolume=8;
  uint16_t NumberOfSongs=0;
  uint16_t CurrentlyPlayingSong;
  uint16_t CurrentFrequency = 0;
  uint8_t CurrentPreset = 0;

  String bt_serial_data = "";
  String bt_serial_last_data = "";
  bool bt_serial_data_ready = false;

  String CallerID="";
  String BT_ADDR="";
  String BT_NAME="";
  String BT_PIN="";

  void init(void);
  void checkState(void);
  void send(String data);
  void DBG(String text);
  String returnCallerID(String receivedString);
  String returnBtModuleName(String receivedString);
  void callAnsware(void);
  void callReject(void);
  void callHangUp(void);
  void callRedial(void);
  void musicPlay(void);
  void musicStop(void);
  void musicNextTrack(void);
  void musicPreviousTrack(void);
  void volumeUp(void);
  void volumeDown(void);
  void volumeSet(String volume);
  void getStatusCall(void);
  void voicesOff(void);
  void voicesOn(void);
  void switchInputToBluetooth(void);
  void switchInputToCard(void);
  void switchInputToAux(void);
  void switchInputToUsb(void);

  void printBTstate();
  

  uint8_t checkResponce(void);
  uint8_t getNextEventFromBT(void);
  uint8_t decodeReceivedString(String receivedString);
  uint8_t simpleDecodeReceivedString(String receivedString);
};
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
