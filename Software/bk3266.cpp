#include "bk3266.h"
#include "cdc_function.h"
#include <SoftwareSerial.h>
#include <Arduino.h>

SoftwareSerial BtSerial(3, 4); // RX, TX

String bt_serial_data = "";
bool bt_serial_data_ready = false;
unsigned long c = 0;

uint8_t bt_module_status = 1;   //disconnected

void InitBK3266()  {
  BtSerial.begin(9600);
}


void CheckStateBK3254() {
  c++;
  delay(10);
  if(c >= 100)  {
    //SendBK3266("COM+MY\r\n"); //Bluetooth inquiry HFP status  disconnect: M0\r\n / connection: M1\r\n / Caller: M2\r\n / Outgoing: M3\r\n / calling: M4\r\n
    //Serial.println("Send MY command");
    c = 0;
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
  if(bt_serial_data_ready == true)  {
            
      Serial.print("Received from bt module: ");
      Serial.println(bt_serial_data);

      if(bt_serial_data == "M4\r\n")  { 
        Serial.println("Incoming call");
        bt_module_status = INCOMING_CALL;
      }
      if(bt_serial_data == "M3\r\n")  { 
        Serial.println("Outgoing call");
        bt_module_status = OUTGOING_CALL;
      }
      if(bt_serial_data == "M2\r\n")  { 
        Serial.println("Active call");
        bt_module_status = CALL_IN_PROGRESS;
      }
      if(bt_serial_data == "M1\r\n") { 
        Serial.println("Connected, idle");
        bt_module_status = CONNECTED_IDLE;
      }
      if(bt_serial_data == "M0\r\n") { 
        Serial.println("No conected, paring mode");
        bt_module_status = DISCONNECTED;
      }
      
      bt_serial_data_ready = false; //reset flag
      bt_serial_data = "";  //clear data
    }  
}

void SendBK3266(String data)  {  
  BtSerial.println(data);
}

uint8_t GetStatusBK3266 (void)  {
  return  bt_module_status;
}

void CheckStateBK3266() {

  c++;
  delay(10);
  if(c == 300)  {
    SendBK3266("COM+CALLON\r\n");
    Serial.println("wykonano");
    c = 0;
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
  if(bt_serial_data_ready == true)  {
            
      Serial.print("Received from bt module: ");
      Serial.println(bt_serial_data);
      
      if(bt_serial_data == "BT_IC\n")  { 
        Serial.println("Incoming call");
        bt_module_status = INCOMING_CALL;
      }
      if(bt_serial_data == "BT_OC\n")  { 
        Serial.println("Outgoing call");
        bt_module_status = OUTGOING_CALL;
      }
      if(bt_serial_data == "BT_EC\n")  { 
        Serial.println("Call in progress");
        bt_module_status = CALL_IN_PROGRESS;
      }
      if((bt_serial_data == "BT_CN\n") && (bt_module_status = CALL_IN_PROGRESS)) { 
        Serial.println("Connected, idle");
        bt_module_status = CONNECTED_IDLE;
      }
      if(bt_serial_data == "BT_CN\n") { 
        Serial.println("Connected, idle");
        bt_module_status = CONNECTED_IDLE;
      }
      if(bt_serial_data == "BT_WP\n") { 
        Serial.println("No conected, paring mode");
        bt_module_status = DISCONNECTED;
      }
        
      bt_serial_data_ready = false; //reset flag
      bt_serial_data = "";  //clear data
    }  
}
