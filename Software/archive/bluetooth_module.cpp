#include "bluetooth_module.h"
#include "cdc_function.h"
#include <SoftwareSerial.h>
#include <Arduino.h>

SoftwareSerial BtSerial(3, 4); // RX, TX

String bt_serial_data = "";
String bt_last_serial_data = "";
uint8_t bt_recived_data = 0;
uint8_t bt_last_recived_data = 0;
bool bt_serial_data_ready = false;

unsigned long c = 0;

uint8_t bt_module_status = 1;   //disconnected
uint8_t bt_last_module_status = 1;

void InitBtModule()  {
  BtSerial.begin(9600);
  delay(50);
  SendBt("COM+VOLC\r\n"); // from 0 to F
  delay(50);
  SendBt("COM+TONEOFF\r\n");
  delay(50);
  /*
  SendBt("COM+SNAME+AUDI A6\r\n");
  delay(50);
  SendBt("COM+REBOOT\r\n");
  delay(2000);
  Serial.println("init done");
  */
}


void CheckStateBt() {  

  #ifdef CHIP_BK3254

    if(GetFlag500ms() == true) {  //every 500ms
      SendBt("AT+MY\r\n");
      ClearFlag500ms();
    }
    
    if (BtSerial.available() > 0) { // if we have something
      while (BtSerial.available())  {
        char sign2 = (char)BtSerial.read();
        bt_serial_data += sign2;
        if(sign2 == '\n')  {
          bt_serial_data_ready = true;
        }
      }
    }

    if((bt_serial_data_ready == true) && (bt_serial_data != bt_last_serial_data))  {
  
      if(bt_serial_data == "M4\r\n")  { 
        bt_module_status = CALL_IN_PROGRESS;
      }
      if(bt_serial_data == "M3\r\n")  { 
        bt_module_status = OUTGOING_CALL;
      }
      if(bt_serial_data == "M2\r\n")  { 
        bt_module_status = INCOMING_CALL;
      }
      if(bt_serial_data == "M1\r\n") { 
        bt_module_status = CONNECTED_IDLE;
      }
      if(bt_serial_data == "M0\r\n") { 
        bt_module_status = DISCONNECTED;
      }

      bt_last_serial_data = bt_serial_data; 
      bt_serial_data_ready = false; //reset flag
      bt_serial_data = "";  //clear data
      }
  #endif

  #ifdef DEBUG
    if(bt_module_status != bt_last_module_status)
    {
      switch(bt_module_status)
      {
        case INCOMING_CALL: 
          Serial.println("Incoming call");
          break;
        case OUTGOING_CALL:
          Serial.println("Outgoing call");
          break;
        case CALL_IN_PROGRESS:
          Serial.println("Active call");
          break;
        case CONNECTED_IDLE:
          Serial.println("Connected, idle");
          break;
        case DISCONNECTED:
          Serial.println("Not connected, paring mode");
          break;
      }
      bt_last_module_status = bt_module_status;
    }
  #endif
}

void SendBt(String data)  {  
  BtSerial.println(data);
}

uint8_t GetStatusBt (void)  {
  return  bt_module_status;
}

uint8_t pairingState() {
  #ifdef CHIP_HY5066SR
  return senddata(BK3266SR_CMD_PAIRING_STATE);
  #else
  SendBt("BT+PR\r\n");
  #endif
}
uint8_t connect() {
  #ifdef CHIP_HY5066SR
  return senddata(BK3266SR_CMD_CONNECTED);
  #endif
}
uint8_t disconnect() {
  #ifdef CHIP_HY5066SR
  return senddata(BK3266SR_CMD_DISCONNECTED);
  #endif
}
uint8_t pickupCall() {
  #ifdef CHIP_HY5066SR
  return senddata(BK3266SR_CMD_PICKUP_CALL);
  #else
  SendBt("BT+CA\r\n");  //Answare the call
  #endif
}
uint8_t rejectcall() {
  #ifdef CHIP_HY5066SR
  return senddata(BK3266SR_CMD_REJECT_CALL);
  #else
  SendBt("BT+CJ\r\n");      //Refuse to call
  #endif
}
uint8_t hangUpCall() {
  #ifdef CHIP_HY5066SR
  return senddata(BK3266SR_CMD_HANGUP_CALL);
  #else
  SendBt("BT+CE\r\n");  //Hang up the phone
  #endif
}
uint8_t redialLastCall() {
  #ifdef CHIP_HY5066SR
  return senddata(BK3266SR_CMD_REDIAL_LAST);
  #else
  SendBt("BT+CR\r\n");  //Last number redial
  #endif
}
uint8_t volumeUp() {
  #ifdef CHIP_HY5066SR
  return senddata(BK3266SR_CMD_VOLUME_UP);
  #endif
}
uint8_t volumeDown() {
  #ifdef CHIP_HY5066SR
  return senddata(BK3266SR_CMD_VOLUME_DOWN);
  #endif
}

uint8_t play() {
  #ifdef CHIP_HY5066SR
  return senddata(BK3266SR_CMD_PLAYPAUSE);
  #else
  SendBt("COM+PA\r\n");  //play
  #endif
}
uint8_t pause() {
  #ifdef CHIP_HY5066SR
  return senddata(BK3266SR_CMD_PLAYPAUSE);
  #else
  SendBt("COM+PU\r\n");  //pause
  #endif
}
uint8_t nextTrack() {
  #ifdef CHIP_HY5066SR
  return senddata(BK3266SR_CMD_NEXT);
  #else
  SendBt("COM+PN\r\n");  //next
  #endif
}
uint8_t previousTrack() {
  #ifdef CHIP_HY5066SR
  return senddata(BK3266SR_CMD_PREV);
  #else
  SendBt("COM+PV\r\n");  //previous
  #endif
}
uint8_t getModuleName() {
  #ifdef CHIP_HY5066SR
  return senddata(BK3266SR_CMD_DEVICE_NAME);
  #endif
}
uint8_t getModuleAddr() {
  #ifdef CHIP_HY5066SR
  return senddata(BK3266SR_CMD_DEVICE_ADDR);
  #endif
}
uint8_t phoneName() {
  #ifdef CHIP_HY5066SR
  return senddata(BK3266SR_CMD_PHONE_NAME);
  #endif
}
uint8_t isAclLinked() {
  #ifdef CHIP_HY5066SR
  return senddata(BK3266SR_CMD_ACL);
  #endif
}
uint8_t callStatus() {
  #ifdef CHIP_HY5066SR
  return senddata(BK3266SR_CMD_CALLSTATUS);
  #endif
}
uint8_t songStatus() {
  #ifdef CHIP_HY5066SR
  return senddata(BK3266SR_CMD_SONGSTATUS);
  #endif
}
uint8_t getVolume() {
  #ifdef CHIP_HY5066SR
  return senddata(BK3266SR_CMD_VOL);
  #endif
}
uint8_t auxSearch() {
  #ifdef CHIP_HY5066SR
  return senddata(BK3266SR_CMD_AUX_SEARCH);
  #endif
}
uint8_t auxDisable() {
  #ifdef CHIP_HY5066SR
  return senddata(BK3266SR_CMD_AUX_DIS);
  #endif
}
uint8_t reset() {
  #ifdef CHIP_HY5066SR
  return senddata(BK3266SR_CMD_RESET);
  #endif
}
uint8_t idleMode() {
  #ifdef CHIP_HY5066SR
  return senddata(BK3266SR_CMD_IDLEMODE);
  #endif
}
uint8_t mute() {
  #ifdef CHIP_HY5066SR
  return senddata(BK3266SR_CMD_MUTE);
  #endif
}
uint8_t unmute() {
  #ifdef CHIP_HY5066SR
  return senddata(BK3266SR_CMD_UNMUTE);
  #endif
}
uint8_t remoteAddr() {
  #ifdef CHIP_HY5066SR
  return senddata(BK3266SR_CMD_REMOTE_ADDR);
  #endif
}
uint8_t transmit() {
  #ifdef CHIP_HY5066SR
  return senddata(BK3266SR_CMD_TRANSMITMODE);
  #endif
}
uint8_t receiver() {
  #ifdef CHIP_HY5066SR
  return senddata(BK3266SR_CMD_RECEIVEMODE);
  #else
  SendBt("COM+MBT\r\n");  //bluetooth mode
  #endif
}
uint8_t setName(uint8_t data[]) {
  #ifdef CHIP_HY5066SR
  return senddata(data);
  #endif
}
