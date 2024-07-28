// last edit 2024/7/25 
// EEPROM map: 2-45 for password, 45-85 for ssid,

#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <EEPROM.h>
#include <Update.h>
#include "head.h"
#include "main.h"
#include "settings.h"
#include "network.h"
#include "update_html.h"
#include "sccess.h"
#include "fail.h"
#include "defaultNetwork.h"
//---------------
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
//-------------------
String our_default_ssid = "Receiver";//default
const char* default_password = "";
String ssid = "";
String password="";

// Define the EEPROM address to store the password at
#define PASSWORD_ADDR 2
#define SSID_ADDR 45

#define Def_SSID 600 // default ssid name location
#define Def_Password 640 // default password location
#define Net_F 179
WebServer server(80);

//---------------
RF24 radio(4, 5); // CE, CSN
//RF24 radio(D2, D1); // CE, CSN //fpr nodemcu
const byte address[6] = "00001";

struct SensorData {
  float accX, accY, accZ;
  float gyroX, gyroY, gyroZ;
  float lat, lon;
} __attribute__((packed));

SensorData receivedData;

unsigned long lastReceivedTime = 0;
const unsigned long timeoutDuration = 5000; // 5 seconds timeout


void setup() {
  EEPROM.begin(1024);
  Serial.begin(115200);
  if(EEPROM.read(Net_F)){
    password=readStringFromEEPROM(PASSWORD_ADDR);
    ssid = readStringFromEEPROM(SSID_ADDR);
  }
  else{
    password=readStringFromEEPROM(Def_Password);
    ssid = readStringFromEEPROM(Def_SSID);
  }
  if (password.isEmpty() || ssid.isEmpty() || password.length()>30 || ssid.length()>30){
    ssid=our_default_ssid;
    password= default_password;
  }
  WiFi.disconnect();

  WiFi.softAP(ssid,password);
  server.on("/state",handleState);
  server.on("/currentPassword",dev_password);
  server.on("/format",format);
  server.on("/dev_ver",deviceVersion);
  server.on("/dev_id",devId);
  server.on("/dev_model",devModel);
  server.on("/firmware",firmware);
  server.on("/networkConfig",handleNewNetwork);
  server.on("/defaultNetwork",handleDefaultNetwork);
  server.on("/formatAll",formatAll);
  server.on("/showMemory",handleMemory);
  server.on("/de",[](){
  server.send(200, "text/html", defaultNet);});
  server.on("/index.html",[](){ server.send(200, "text/html", index_html); });
  server.on("/index",[](){ server.send(200, "text/html", index_html); });
  server.on("/",[](){  server.send(200, "text/html", index_html); });
  server.on("/update_p.html",[](){  server.send(200, "text/html", update); });
  server.on("/settings.html",[](){ server.send(200, "text/html", settings); });
  server.on("/network.html",[](){ server.send(200, "text/html", network); });
  server.on("/up", HTTP_GET, []() {
    server.send(200, "text/html", 
      "<form method='POST' action=' ' enctype='multipart/form-data'>"
      "<input type='file' name='update'>"
      "<input type='submit' value='Update'>"
      "</form>");
  });
  server.on("/update", HTTP_POST, []() {
    server.sendHeader("Connection", "close");
        //server.send_P(200, "text/html", html); // Use send_P to access PROGMEM content
    server.send(200, "text/html", (Update.hasError()) ? fail_m : sccess_m);
    delay(100);
    ESP.restart();
  }, []() {
    HTTPUpload& upload = server.upload();
    if (upload.status == UPLOAD_FILE_START) {
      Serial.printf("Update: %s\n", upload.filename.c_str());
      if (!Update.begin(UPDATE_SIZE_UNKNOWN)) { // start with max available size
        Update.printError(Serial);
      }
    } else if (upload.status == UPLOAD_FILE_WRITE) {
      if (Update.write(upload.buf, upload.currentSize) != upload.currentSize) {
        Update.printError(Serial);
      }
    } else if (upload.status == UPLOAD_FILE_END) {
      if (Update.end(true)) { // true to set the size to the current progress
        Serial.printf("Update Success: %u bytes\nRebooting...\n", upload.totalSize);
      } else {
        Update.printError(Serial);
      }
    } else if (upload.status == UPLOAD_FILE_ABORTED) {
      Update.end();
      Serial.println("Update Aborted");
    }
  });
  server.onNotFound([]() {
    server.send(404, "text/plain", "Not Found");
  });

  server.enableCORS(true);
  server.begin();

  //---------------------
  
  Serial.println("nRF24L01 Receiver Starting");
  
  if (!radio.begin()) {
    Serial.println("Radio hardware not responding!");
    while (1) {} // hold in infinite loop
  }
  
  // Set the NRF24L01 radio configuration
  radio.setPALevel(RF24_PA_LOW);
  radio.setChannel(108);
  radio.openReadingPipe(1, 0xF0F0F0F0E1LL);
  radio.startListening();
  Serial.println("Receiver initialized and listening...");

  } //end setup
 
void loop() {
  server.handleClient();
   //--------------
   if (radio.available()) {
    radio.read(&receivedData, sizeof(receivedData));
    lastReceivedTime = millis();
    printData();
  } else if (millis() - lastReceivedTime > timeoutDuration) {
    Serial.println("No data received yet");
    delay(50);  // Wait a second before checking again
  }

  
  }//end loop
void printData() {
  Serial.println("Data received:");
  Serial.print("Acc X: "); Serial.print(receivedData.accX);
  Serial.print(" Y: "); Serial.print(receivedData.accY);
  Serial.print(" Z: "); Serial.println(receivedData.accZ);
  Serial.print("Gyro X: "); Serial.print(receivedData.gyroX);
  Serial.print(" Y: "); Serial.print(receivedData.gyroX);
  Serial.print(" Z: "); Serial.println(receivedData.gyroZ);
  Serial.print("Lat: "); Serial.print(receivedData.lat, 6);
  Serial.print(" Lon: "); Serial.println(receivedData.lon, 6);
  Serial.println();
}





void handleRoot() {
      server.send(200, "text/plain","developed by ACSD");
    }

void handleNewNetwork() {
  if (server.hasArg("pass") && server.hasArg("ssid")) {
    String newPassword = server.arg("pass");
    String newSSID = server.arg("ssid");

    if (!newPassword.isEmpty() && newPassword.length() >= 8 && !newSSID.isEmpty()) {
      password = newPassword;
      writeStringToEEPROM(password, PASSWORD_ADDR);
      writeStringToEEPROM(newSSID, SSID_ADDR);
      EEPROM.write(Net_F, 1);
      EEPROM.commit();
      delay(1000);
      ESP.restart();
      server.send(200, "text/plain", "New network credentials have been updated");
    } else {
      server.send(400, "text/plain", "Invalid password or SSID details");
    }
  } else {
    server.send(400, "text/plain", "SSID and password are required");
  }
}


void handleDefaultNetwork() {
  if (server.hasArg("defPass") && server.hasArg("ssid")) {
    String defaultPassword = server.arg("defPass");
    String defSSID = server.arg("ssid");

    if (!defaultPassword.isEmpty() && !defSSID.isEmpty()) {
      password = defaultPassword;
      writeStringToEEPROM(password, Def_Password);
      writeStringToEEPROM(defSSID, Def_SSID);
      // EEPROM.write(PASS_F, 1);
      // EEPROM.write(SSID_F, 1);
      // EEPROM.commit();
      server.send(200, "text/plain", "New Network credentials have been updated");
      delay(1000);
      ESP.restart();
      return;
    }
  }

  server.send(400, "text/plain", "Incorrect or incomplete request details!");
}
                             


void dev_password(){
         server.send(200, "text/plain",password);
  }
void deviceVersion(){
         server.send(200, "text/plain",version);

  }
  void devId(){
         server.send(200, "text/plain",dev_id);
  }
  void devModel(){
         server.send(200, "text/plain",dev_model);
  }
  void firmware(){
         server.send(200, "text/plain",dev_firmware);
  }




void handleState() {
    String recievedData = 
    String(receivedData.accX) + "#" + String(receivedData.accY) + "#" + String(receivedData.accZ) + "#" +
    String(receivedData.gyroX) + "#" + String(receivedData.gyroY) + "#" + String(receivedData.gyroZ) + "#" +
    String(receivedData.lat, 6) + "#" + String(receivedData.lon, 6);
    server.send(200, "text/plain", recievedData);
  } 
  void writeStringToEEPROM(String str, int address) {
          int length = str.length();
          for (int i = 0; i < length; i++) {
            EEPROM.write(address + i, str.charAt(i));
          }
          EEPROM.write(address + length, '\0'); // Null-terminate the string
          EEPROM.commit();
        }

long readNumberFromEEPROM(int address) {
    String str;
    char character;
    int i = 0;
    while (true) {
      character = EEPROM.read(address + i);
      if (character == '\0') {
        break;
      }
      str += character;
      i++;
    }
    long number = str.toInt();
  return number;
  }


String readStringFromEEPROM(int address) {
    String str;
    char character;
    int i = 0;
    while (true) {
      character = EEPROM.read(address + i);
      if (character == '\0') {
        break;
      }
      str += character;
      i++;
    }
  return str;
  }






void handleMemory() {
    String eepromContent = "";
    // Read and display the entire EEPROM content
    for (int i = 0; i < EEPROM.length(); i++) {
        byte value = EEPROM.read(i);
        eepromContent += String(value) + "  ";
    }
    server.send(200, "text/plain", eepromContent);
}
void format() {
  for (int i = 0; i < 590; i++) {
    EEPROM.write(i, 0); // Set each byte in EEPROM to 0
  }
  EEPROM.commit(); 
  server.send(200, "text/plain", "erased");
  delay(800);
  //ESP.reset();
  ESP.restart();
}
void formatAll() {
    for (int i = 0; i < EEPROM.length(); i++) {
        EEPROM.write(i, 0); // Set each byte in EEPROM to 0
    }
    EEPROM.commit();
    server.send(200, "text/plain", "Memory will be formatted");
    delay(800);
    ESP.restart();
}

