// last edit 2024/7/23 
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
//------------------------
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_MPU6050.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <TinyGPS++.h>
#include <HardwareSerial.h>

String our_default_ssid = "node 1";//default
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

//-------------------------

Adafruit_MPU6050 mpu;
RF24 radio(4, 5); // CE, CSN
TinyGPSPlus gps;
HardwareSerial SerialGPS(1);

const byte address[6] = "00001";

struct SensorData {
  float accX, accY, accZ;
  float gyroX, gyroY, gyroZ;
  float lat, lon;
};

SensorData sensorData;


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
  //----------------------------
   Serial.begin(115200);
  SerialGPS.begin(9600, SERIAL_8N1, 16, 17);
  Wire.begin(21, 22); // SDA, SCL

  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) { delay(10); }
  }
 
  mpu.setAccelerometerRange(MPU6050_RANGE_2_G);
  mpu.setGyroRange(MPU6050_RANGE_250_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);

  if (!radio.begin()) {
    Serial.println("Failed to initialize radio");
    while (1) { delay(10); }
  }

  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_HIGH);
  radio.setDataRate(RF24_250KBPS);
  radio.setChannel(76);
  radio.stopListening();

  Serial.println("Initialization complete");
  } //end setup
 
void loop() {
  server.handleClient();
   
   //-----------------------------
  readMPU6050();
  readGPS();
  sendData();
  displayInfo();
  delay(50);

  }//end loop




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
    String storedData = 
    String(sensorData.accX) + "#" + String(sensorData.accY) + "#" + String(sensorData.accZ) + "#" +
    String(sensorData.gyroX) + "#" + String(sensorData.gyroY) + "#" + String(sensorData.gyroZ) + "#" +
    String(sensorData.lat, 6) + "#" + String(sensorData.lon, 6);
    server.send(200, "text/plain", storedData);
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


//----------------

void readMPU6050() {
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);
  sensorData.accX = a.acceleration.x;
  sensorData.accY = a.acceleration.y;
  sensorData.accZ = a.acceleration.z;
  sensorData.gyroX = g.gyro.x;
  sensorData.gyroY = g.gyro.y;
  sensorData.gyroZ = g.gyro.z;
}

void readGPS() {
  while (SerialGPS.available() > 0) {
    if (gps.encode(SerialGPS.read())) {
      if (gps.location.isValid()) {
        sensorData.lat = gps.location.lat();
        sensorData.lon = gps.location.lng();
      }
    }
  }
}
void sendData() {
  radio.powerUp();
  delay(5);

  if (gps.location.isValid()) {
    bool report = radio.write(&sensorData, sizeof(sensorData));
    if (!report) {
      Serial.println("Failed to send data");
      // Attempt to diagnose the problem
      if (!radio.isChipConnected()) {
        Serial.println("Radio hardware not responding");
      } else {
        Serial.println("Radio hardware is responding, but transmission failed");
        Serial.print("Channel: "); Serial.println(radio.getChannel());
        Serial.print("Data Rate: "); Serial.println(radio.getDataRate());
        Serial.print("PA Level: "); Serial.println(radio.getPALevel());
      }
    } else {
      Serial.println("Data sent successfully");
    }
  } else {
    Serial.println("Waiting for valid GPS data");
  }

  radio.powerDown();
}

void displayInfo() {
  //accx="Acc X: "+sensorData.accX;
  Serial.print("Acc X: "); Serial.print(sensorData.accX);
  Serial.print(" Y: "); Serial.print(sensorData.accY);
  Serial.print(" Z: "); Serial.print(sensorData.accZ);
  Serial.print(" | Gyro X: "); Serial.print(sensorData.gyroX);
  Serial.print(" Y: "); Serial.print(sensorData.gyroY);
  Serial.print(" Z: "); Serial.print(sensorData.gyroZ);
  Serial.print(" | Lat: "); Serial.print(sensorData.lat, 6);
  Serial.print(" Lon: "); Serial.println(sensorData.lon, 6);
}
