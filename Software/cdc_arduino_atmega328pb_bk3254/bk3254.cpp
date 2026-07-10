#include "bk3254.h"
#include "cdc_function.h"
#include <Arduino.h>

BK3254 bt;

extern uint8_t flag_250ms;

void BK3254::init()  {
  Serial1.begin(9600);
  delay(100);
  voicesOff();
  delay(100);
  volumeSet("D");
  Serial.println("BK3254 init finish");
}

void BK3254::checkState() {
  if(flag_250ms == true)  { 
    send("AT+MY\r\n");  //ask BK3254 for call state
    flag_250ms = false; 
  }

  if (Serial1.available() > 0) { // if we have something
    while (Serial1.available())  {
      char sign2 = (char)Serial1.read();
      if((sign2 != '\r') || (sign2 != '\n')) bt_serial_data += sign2;  // skip \r
      if(sign2 == '\n')  {
        bt_serial_data_ready = true;
      }
    }
  }
  if(bt_serial_data_ready == true)  {
      if (strcmp(bt_serial_data.c_str(), bt_serial_last_data.c_str()) != 0) {   //check if last string wasn't this same 
    // Strings are not equal
    decodeReceivedString(bt_serial_data);   //decode recived string
    }
      bt_serial_data_ready = false;         //clear flag
      bt_serial_last_data = bt_serial_data; //set last data string
      bt_serial_data = "";                  //clear string
    }
}

uint8_t BK3254::getNextEventFromBT(void) {
  char c;
  String receivedString;
  while (Serial1.available() > 0 && c != '\n') { //read serial buffer until \n
    c = (Serial1.read());
    if (c == 0xA) {
      if (receivedString == "") { //nothing before enter was received
  #if defined DEBUG
        //DBG(F("received only empty string\nrunning again myself...\n"));
  #endif
        //return BK3254::getNextEventFromBT();
      }
      uint8_t ret = decodeReceivedString(receivedString);
      receivedString="";
      return ret;
    }
    //append received buffer with received character
    if (c != 0xD) receivedString += c;  // skip \r
  }
  return 0;
}

uint8_t BK3254::checkResponce(void){    //not used in code
  uint16_t timeout=500;//500ms -- datasheet did not stated any timeout for "OK" responce, so I give him 500ms
  while (!getNextEventFromBT() && timeout > 0)
  {
    timeout--;
    delay(1); // wait 1milisecond
  }
  if (timeout == 0) {
#if defined DEBUG
       DBG(F("timeout")); DBG((String)timeout);
#endif
        return false;
      }
  return true;
}

void BK3254::send(String data)  {  
  Serial1.print(data);
}

void BK3254::DBG(String text) {
  Serial.print(text);
}
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

  disconnect: M0\r\n
  connection: M1\r\n
  Caller: M2\r\n
  Outgoing: M3\r\n
  calling: M4\r\n
  IR-"+123456789"
  BK3266 support:

  BT_WP Currently in Bluetooth mode, bluetooth are in pairing state
*/

uint8_t BK3254::simpleDecodeReceivedString(String receivedString)  {
  
  if (memcmp(&receivedString[0], "M0", 2) == 0) {
  #if defined DEBUG
      DBG(F("Disconnected\n"));
  #endif
      BTState = Disconnected;
      CallState = Idle;
    } else if (memcmp(&receivedString[0], "M1", 2) == 0) {
  #if defined DEBUG
      DBG(F("Connected\n"));
  #endif
      BTState = Connected;
      CallState = Idle;
    } else if (memcmp(&receivedString[0], "M2", 2) == 0) {
  #if defined DEBUG
      DBG(F("Incoming call\n"));
  #endif
      CallState = IncomingCall;
    } else if (memcmp(&receivedString[0], "M3", 2) == 0) {
  #if defined DEBUG
      DBG(F("Outgoing call\n"));
  #endif
      CallState = OutgoingCall;
    } else if (memcmp(&receivedString[0], "M4", 2) == 0) {
  #if defined DEBUG
      DBG(F("Call in progress\n"));
  #endif
      CallState = CallInProgress;
    }

    receivedString="";
  #if defined DEBUG
  //   DBG(F("Return true\n"));
  #endif
    return 1;
}
uint8_t BK3254::decodeReceivedString(String receivedString) {


  #if defined DEBUG
    //DBG(receivedString + "\n");
  #endif

    if (memcmp(&receivedString[0], "AX_PA", 5) == 0) {
  #if defined DEBUG
      DBG(F("Playing status\n"));
  #endif
      InputSelected = AUX;
      MusicState = Playing;
      PowerState = On;
    } else if (memcmp(&receivedString[0], "AX_PU", 5) == 0) {
  #if defined DEBUG
      DBG(F("AUX In a suspended state\n"));
  #endif
      InputSelected = AUX;
      MusicState = Stopped;
      PowerState = On;
    } else if (memcmp(&receivedString[0], "BT_AC", 5) == 0) {
  #if defined DEBUG
      DBG(F("Bluetooth Even being back\n"));
  #endif
      InputSelected = BT;
      PowerState = On;
    } else if (memcmp(&receivedString[0], "BT_CN", 5) == 0) {
  #if defined DEBUG
      DBG(F("Bluetooth connected\n"));
  #endif
      InputSelected = BT;
      BTState = Connected;
      PowerState = On;
      CallState = Idle;
    } else if (memcmp(&receivedString[0], "BT_EC", 5) == 0) {
  #if defined DEBUG
      DBG(F("Bluetooth call in progress\n"));
  #endif
      InputSelected = BT;
      CallState = CallInProgress;
      PowerState = On;
    } else if (memcmp(&receivedString[0], "BT_IC", 5) == 0) {
  #if defined DEBUG
      DBG(F("Bluetooth incomming call\n"));
  #endif
      InputSelected = BT;
      CallState = IncomingCall;
      PowerState = On;
    } else if (memcmp(&receivedString[0], "BT_WP", 5) == 0) {
  #if defined DEBUG
      DBG(F("Bluetooth In pairing state\n"));
  #endif
      InputSelected = BT;
      BTState = Pairing;
      PowerState = On;
      CallState = Idle;
    } else if (memcmp(&receivedString[0], "BT_WC", 5) == 0) {
  #if defined DEBUG
      DBG(F("Bluetooth connection is in wait state\n"));
  #endif
      InputSelected = BT;
      BTState = Connecting;
      PowerState = On;
      CallState = Idle;
    } else if (memcmp(&receivedString[0], "BT_OC", 5) == 0) {
  #if defined DEBUG
      DBG(F("Bluetooth Telephone outgoing call\n"));
  #endif
      InputSelected = BT;
      CallState = OutgoingCall;
      PowerState = On;
    } else if (memcmp(&receivedString[0], "BT_PA", 5) == 0) {
  #if defined DEBUG
      DBG(F("Bluetooth Now Playing\n"));
  #endif
      InputSelected = BT;
      CallState = Idle;
      MusicState = Playing;
      BTState = Connected;
      PowerState = On;
    } else if (memcmp(&receivedString[0], "ERR", 3) == 0) {
  #if defined DEBUG
      DBG(F("Error"));
  #endif
      PowerState = On;
      receivedString="";
  #if defined DEBUG
  //   DBG(F("Return false\n"));
  #endif
      return 0;
    } else if (memcmp(&receivedString[0], "FM_FQ=", 6) == 0) {
      CurrentFrequency = receivedString.substring(6).toInt();
  #if defined DEBUG
      DBG(F("Tunner frequency: "));DBG((String)CurrentFrequency + "MHz\n");
  #endif
      InputSelected = FM;
      BTState = Disconnected;
      PowerState = On;
    } else if (memcmp(&receivedString[0], "FM_PA", 5) == 0) {
  #if defined DEBUG
      DBG(F("You are listening state\n"));
  #endif
      InputSelected = FM;
      MusicState = Playing;
      BTState = Disconnected;
      PowerState = On;
    } else if (memcmp(&receivedString[0], "FM_PU", 5) == 0) {
  #if defined DEBUG
      DBG(F("FM In a suspended state\n"));
  #endif
      InputSelected = FM;
      MusicState = Stopped;
      BTState = Disconnected;
      PowerState = On;
    } else if (memcmp(&receivedString[0], "FM_SC", 5) == 0) {
  #if defined DEBUG
      DBG(F("FM The state of being seized Taiwan\n"));
  #endif
      InputSelected = FM;
      BTState = Disconnected;
      PowerState = On;
    } else if (memcmp(&receivedString[0], "IA", 2) == 0) {
  #if defined DEBUG
      DBG(F("Disconnect\n"));
  #endif
      InputSelected = BT;
      BTState = Disconnected;
      PowerState = On;
    } else if (memcmp(&receivedString[0], "II", 2) == 0) {
  #if defined DEBUG
      DBG(F("Connection succeeded\n"));
  #endif
      InputSelected = BT;
      BTState = Connected;
      PowerState = On;
    } else if (memcmp(&receivedString[0], "IR-", 3) == 0) {
  #if defined DEBUG
      DBG(F("Incoming call\n"));
  #endif
      InputSelected = BT;
      BTState = Connected;
      CallerID = BK3254::returnCallerID(receivedString);
      PowerState = On;
    } else if (memcmp(&receivedString[0], "IR", 2) == 0) {
  #if defined DEBUG
      DBG(F("Infrared command received\n"));
  #endif
      PowerState = On;
    } else if (memcmp(&receivedString[0], "PR-", 3) == 0) {
  #if defined DEBUG
      DBG(F("Outgoing call\n"));
  #endif
      InputSelected = BT;
      BTState = Connected;
      CallerID = BK3254::returnCallerID(receivedString);
      PowerState = On;
    } else if (memcmp(&receivedString[0], "MMMP", 4) == 0) {
      NumberOfSongs = receivedString.substring(4).toInt();
  #if defined DEBUG
      DBG(F("Number of songs: "));DBG((String)NumberOfSongs + "\n");
  #endif
    } else if (memcmp(&receivedString[0], "music_mun=", 10) == 0) {
      CurrentlyPlayingSong = receivedString.substring(10).toInt();
  #if defined DEBUG
      DBG(F("Song number currently playing: "));DBG(String(CurrentlyPlayingSong) + "\n");
  #endif
    } else if (memcmp(&receivedString[0], "OK", 2) == 0) {
  #if defined DEBUG
      DBG(F("Success\n"));
  #endif
    } else if (memcmp(&receivedString[0], "ON", 2) == 0) {
  #if defined DEBUG
      DBG(F("Bluetooth turned on\n"));
  #endif
      PowerState = On;
  #ifndef BK3266
    } else if (memcmp(&receivedString[0], "PLAY_ALL", 8) == 0 || memcmp(&receivedString[0], "PLAY_M0", 7) == 0) {
  #else
    } else if (memcmp(&receivedString[0], "COM_SMA", 7) == 0) {
  #endif
  #if defined DEBUG
      DBG(F("Repeat All Tracks (TF/SDcard Mode)\n"));
  #endif
      ModeOfPlay = RepeatAll;
  #ifndef BK3266
    } else if (memcmp(&receivedString[0], "PLAY_ONE", 8) == 0 || memcmp(&receivedString[0], "PLAY_M1", 7) == 0) {
  #else
    } else if (memcmp(&receivedString[0], "COM_SMO", 7) == 0) {
  #endif
  #if defined DEBUG
      DBG(F("Repeat One Track (TF/SDcard Mode)\n"));
  #endif
      ModeOfPlay = RepeatOne;
  #ifndef BK3266
    } else if (memcmp(&receivedString[0], "PLAY_M2", 7) == 0) {
  #else
    } else if (memcmp(&receivedString[0], "COM_SMNO", 8) == 0) {
  #endif
  #if defined DEBUG
      DBG(F("Repeat None (TF/SDcard Mode)\n"));
  #endif
      ModeOfPlay = RepeatNone;
    } else if (memcmp(&receivedString[0], "COM_SMR", 7) == 0) {
  #if defined DEBUG
      DBG(F("Shuffle (TF/SDcard Mode)\n"));
  #endif
      ModeOfPlay = Shuffle;
    } else if (memcmp(&receivedString[0], "SD_PA", 5) == 0) {
  #if defined DEBUG
      DBG(F("SD Card playing status\n"));
  #endif
      InputSelected = SD;
      MusicState = Playing;
    } else if (memcmp(&receivedString[0], "SD_PU", 5) == 0) {
  #if defined DEBUG
      DBG(F("SD Card is paused\n"));
  #endif
      InputSelected = SD;
      MusicState = Stopped;
    } else if (memcmp(&receivedString[0], "SY_PO", 5) == 0) {
  #if defined DEBUG
      DBG(F("Bluetooth turned on\n"));
  #endif
      PowerState = On;
      BTState = Disconnected; //init all values
      CallState = Idle;
      MusicState = Stopped;
      CallerID = "";
      BT_ADDR = "";
      BT_NAME = "";
      BT_PIN = "";
      CurrentFrequency = 0;
      CurrentPreset = 0;
    } else if (memcmp(&receivedString[0], "SY_PF", 5) == 0) {
  #if defined DEBUG
      DBG(F("Bluetooth off\n"));
  #endif
      PowerState = Off; //reset all values:
      BTState = Disconnected;
      CallState = Idle;
      MusicState = Stopped;
      CallerID = "";
      BT_ADDR = "";
      BT_NAME = "";
      BT_PIN = "";
      CurrentFrequency = 0;
      CurrentPreset = 0;
    } else if (memcmp(&receivedString[0], "UD_PA", 5) == 0) {
  #if defined DEBUG
      DBG(F("USB Playing status\n"));
  #endif
      InputSelected = USB;
      BTState = Disconnected;
      MusicState = Playing;
    } else if (memcmp(&receivedString[0], "UD_PU", 5) == 0) {
  #if defined DEBUG
      DBG(F("USB In a suspended state\n"));
  #endif
      InputSelected = USB;
      BTState = Disconnected;
      MusicState = Stopped;
  #ifdef BK3266
    } else if (memcmp(&receivedString[0], "COM_V", 5) == 0) {
      currentVolume = receivedString.substring(5).toInt();
  #else
    } else if (memcmp(&receivedString[0], "VOL", 3) == 0) {
      currentVolume = receivedString.substring(3).toInt();
  #endif
  #if defined DEBUG
      DBG(F("The current volume level: "));DBG((String)currentVolume + "\n");
  #endif
  #ifdef BK3266
    } else if (memcmp(&receivedString[0], "AD_", 3) == 0) {
  #else
    } else if (memcmp(&receivedString[0], "AD:", 3) == 0) {
  #endif
      BT_ADDR = receivedString.substring(3);
  #if defined DEBUG
      DBG(F("BT ADDRESS: "));DBG(BT_ADDR + "\n");
  #endif
  #ifdef BK3266
    } else if (memcmp(&receivedString[0], "PN_", 3) == 0) {
      BT_PIN = receivedString.substring(3);
  #else
    } else if (memcmp(&receivedString[0], " PN: ", 5) == 0) {
      BT_PIN = receivedString.substring(5);
  #endif
  #if defined DEBUG
      DBG(F("Pin received:"));DBG(BT_PIN + "\n");
  #endif
  #ifdef BK3266
    } else if (memcmp(&receivedString[0], "NA_", 3) == 0) {
  #else
    } else if (memcmp(&receivedString[0], "NA: ", 4) == 0) {
  #endif
      BT_NAME = BK3254::returnBtModuleName(receivedString);
  #if defined DEBUG
      DBG(F("BT name received: "));DBG(BT_NAME + "\n");
  #endif
    } else if (memcmp(&receivedString[0], "C1", 2) == 0) {
  #if defined DEBUG
      DBG(F("Connection succeeded\n"));
  #endif
      BTState = Connected;
    } else if (memcmp(&receivedString[0], "C0", 2) == 0) {
  #if defined DEBUG
      DBG(F("No connection\n"));
  #endif
      BTState = Disconnected;
    } else if (memcmp(&receivedString[0], "MB", 2) == 0) {
  #if defined DEBUG
      DBG(F("Play\n"));
  #endif
      MusicState = Playing;
    } else if (memcmp(&receivedString[0], "MA", 2) == 0) {
  #if defined DEBUG
      DBG(F("Idle\n"));
  #endif
      MusicState = Stopped;
    } else if (memcmp(&receivedString[0], "M0", 2) == 0) {
  #if defined DEBUG
      DBG(F("Disconnect\n"));
  #endif
      BTState = Disconnected;
    } else if (memcmp(&receivedString[0], "M1", 2) == 0) {
  #if defined DEBUG
      DBG(F("Connection\n"));
  #endif
      BTState = Connected;
    } else if (memcmp(&receivedString[0], "M2", 2) == 0) {
  #if defined DEBUG
      DBG(F("Caller\n"));
  #endif
      CallState = CallInProgress;
    } else if (memcmp(&receivedString[0], "M3", 2) == 0) {
  #if defined DEBUG
      DBG(F("Outgoing\n"));
  #endif
      CallState = OutgoingCall;
    } else if (memcmp(&receivedString[0], "M4", 2) == 0) {
  #if defined DEBUG
      DBG(F("Calling\n"));
  #endif
      CallState = IncomingCall;
    } else if (memcmp(&receivedString[0], "MFM", 3) == 0) {
  #if defined DEBUG
    DBG(F("Current preset: "));
  #endif
      CurrentPreset = receivedString.substring(3).toInt();
  #if defined DEBUG
    DBG(String(CurrentPreset));DBG(F("\n"));
  #endif

    } else if (memcmp(&receivedString[0], "MUSICPLYFINISH", 14) == 0 ) {
      MusicState = Stopped;
  #if defined DEBUG
    DBG(F("MUSICPLYFINISH\n"));
  #endif
    }
    receivedString="";
  #if defined DEBUG
  //   DBG(F("Return true\n"));
  #endif
    return 1;
}

String BK3254::returnCallerID(String receivedString) {
  #if defined DEBUG
    DBG(F("Calling: "));DBG(receivedString.substring(4, (receivedString.length() - 2)) + "\n");
  #endif
    return receivedString.substring(4, (receivedString.length() - 2)); //start at 4 cose: IR-"+123456789" or PR-"+123456789" and one before end to remove " and \0
}

String BK3254::returnBtModuleName(String receivedString) {
  #if defined DEBUG
  #ifdef BK3266
    DBG(F("Bluetooth module name: "));DBG(receivedString.substring(3) + "\n");
  #else
    DBG(F("Bluetooth module name: "));DBG(receivedString.substring(4) + "\n");
  #endif
  #endif
  #ifdef BK3266
    return receivedString.substring(3);
  #else
    return receivedString.substring(4);
  #endif
}
  
void BK3254::callAnsware() { //BK3254_CALL_ANSWARE "BT+CA" //Answare the call
  BK3254::send("BT+CA\r\n");
}

void BK3254::callReject() { //BK3254_CALL_REJECT "BT+CJ" //To reject a call
  BK3254::send("BT+CJ\r\n");
}

void BK3254::callHangUp() { //BK3254_CALL_HANGUP "BT+CE" //Hang up the phone
  BK3254::send("BT+CE\r\n");
}

void BK3254::callRedial() { //BK3254_CALL_REDIAL "BT+CR" //Last number redial
  BK3254::send("BT+CR\r\n");
}

void BK3254::musicPlay() { //BK3254_MUSIC_PLAY "COM+PA" //Music Play
 BK3254::send("COM+PA\r\n");
}

void BK3254::musicStop() { //BK3254_MUSIC_PAUSE "COM+PU" //Music Pause
  BK3254::send("COM+PU\r\n");
}

void BK3254::musicNextTrack() { //BK3254_MUSIC_NEXT_TRACK "COM+PN" //next track/ FM next station
  BK3254::send("COM+PN\r\n");
}

void BK3254::musicPreviousTrack() { //BK3254_MUSIC_PREVIOUS_TRACK "COM+PV" //previous piece/ FM On one
  BK3254::send("COM+PV\r\n");
}

void BK3254::volumeUp() { //BK3254_VOLUME_UP "COM+VP" //Volume Up
  BK3254::send("COM+VP\r\n");
}

void BK3254::volumeDown() { //BK3254_VOLUME_DOWN "COM+VD" //Volume down
  BK3254::send("COM+VD\r\n");
}

void BK3254::volumeSet(String volume) { //BK3254_VOLUME_SET "COM+VOL" //VOL+x: 0x00 - 0xAF  Set the volume  correct: VOLx\n / error: ERR\n
  BK3254::send("COM+VOL" + volume + "\r\n");
}

void BK3254::getStatusCall(void) {
  BK3254::send("AT+MY\r\n");
}

void BK3254::voicesOff() {
  BK3254::send("COM+TONEOFF\r\n");
}

void BK3254::voicesOn() {
  BK3254::send("COM+TONEON\r\n");
}

void BK3254::switchInputToBluetooth() { //BK3254_INPUT_BLUETOOTH "COM+MBT" //Bluetooth mode
  BK3254::send("COM+MBT\r\n");
}

void BK3254::switchInputToCard() { //BK3254_INPUT_TF "COM+MSD" //TF/SDcard Mode (if available)
  BK3254::send("COM+MSD\r\n");
}

void BK3254::switchInputToAux() { //BK3254_INPUT_AUX "COM+MAX" //AUX Mode (if available)
  BK3254::send("COM+MAX\r\n");
}

void BK3254::switchInputToUsb() { //BK3254_INPUT_USB "COM+MUD" //USB Disk mode (if available)
  BK3254::send("COM+MUD\r\n");
}