#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <EEPROM.h>
#include <Update.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_MPU6050.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <TinyGPS++.h>
#include <HardwareSerial.h>

// Include the relevant headers
#include "head.h"
#include "main.h"
#include "settings.h"
#include "network.h"
#include "update_html.h"
#include "sccess.h"
#include "fail.h"
#include "defaultNetwork.h"

#define PASSWORD_ADDR 2
#define SSID_ADDR 45
#define Def_SSID 600
#define Def_Password 640
#define Net_F 179

String our_default_ssid = "node 2"; // default
const char* default_password = "";
String ssid = "";
String password = "";

WebServer server(80);

Adafruit_MPU6050 mpu;
RF24 radio(4, 5); // CE, CSN
TinyGPSPlus gps;
HardwareSerial SerialGPS(1);

const byte address[6] = "00001";

struct SensorData1 {
  float accX, accY, accZ;
  float gyroX, gyroY, gyroZ;
} __attribute__((packed));

struct SensorData2 {
  char deviceName[8];
  float lat, lon;
} __attribute__((packed));

SensorData1 sensorData1;
SensorData2 sensorData2;

void setup() {
  EEPROM.begin(1024);
  Serial.begin(115200);
  
  if(EEPROM.read(Net_F)){
    password = readStringFromEEPROM(PASSWORD_ADDR);
    ssid = readStringFromEEPROM(SSID_ADDR);
  } else {
    password = readStringFromEEPROM(Def_Password);
    ssid = readStringFromEEPROM(Def_SSID);
  }
  
  if (password.isEmpty() || ssid.isEmpty() || password.length() > 30 || ssid.length() > 30){
    ssid = our_default_ssid;
    password = default_password;
  }
    strncpy(sensorData2.deviceName, ssid.c_str(), sizeof(sensorData2.deviceName) - 1);
  sensorData2.deviceName[sizeof(sensorData2.deviceName) - 1] = '\0'; // Null-terminate
  

  WiFi.disconnect();
  WiFi.softAP(ssid, password);
  
  setupServer();
  setupSensors();
  
  Serial.println("Initialization complete");
}

void loop() {
  server.handleClient();
  readMPU6050();
  readGPS();
  sendData();
  displayInfo();
  delay(50);
}

void setupServer() {
  server.on("/state", handleState);
  server.on("/currentPassword", dev_password);
  server.on("/format", format);
  server.on("/dev_ver", deviceVersion);
  server.on("/dev_id", devId);
  server.on("/dev_model", devModel);
  server.on("/firmware", firmware);
  server.on("/networkConfig", handleNewNetwork);
  server.on("/defaultNetwork", handleDefaultNetwork);
  server.on("/formatAll", formatAll);
  server.on("/showMemory", handleMemory);
  server.on("/de", []() { server.send(200, "text/html", defaultNet); });
  server.on("/index.html", []() { server.send(200, "text/html", index_html); });
  server.on("/index", []() { server.send(200, "text/html", index_html); });
  server.on("/", []() { server.send(200, "text/html", index_html); });
  server.on("/update_p.html", []() { server.send(200, "text/html", update); });
  server.on("/settings.html", []() { server.send(200, "text/html", settings); });
  server.on("/network.html", []() { server.send(200, "text/html", network); });
  server.on("/up", HTTP_GET, []() {
    server.send(200, "text/html", 
      "<form method='POST' action='/update' enctype='multipart/form-data'>"
      "<input type='file' name='update'>"
      "<input type='submit' value='Update'>"
      "</form>");
  });
  server.on("/update", HTTP_POST, []() {
    server.sendHeader("Connection", "close");
    server.send(200, "text/html", (Update.hasError()) ? fail_m : sccess_m);
    delay(100);
    ESP.restart();
  }, []() {
    HTTPUpload& upload = server.upload();
    if (upload.status == UPLOAD_FILE_START) {
      Serial.printf("Update: %s\n", upload.filename.c_str());
      if (!Update.begin(UPDATE_SIZE_UNKNOWN)) {
        Update.printError(Serial);
      }
    } else if (upload.status == UPLOAD_FILE_WRITE) {
      if (Update.write(upload.buf, upload.currentSize) != upload.currentSize) {
        Update.printError(Serial);
      }
    } else if (upload.status == UPLOAD_FILE_END) {
      if (Update.end(true)) {
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
}

void setupSensors() {
  SerialGPS.begin(9600, SERIAL_8N1, 16, 17);
  Wire.begin(21, 22); // SDA, SCL

  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    //while (1) { delay(10); }
  }
 
  mpu.setAccelerometerRange(MPU6050_RANGE_16_G);//less sensitive
  mpu.setGyroRange(MPU6050_RANGE_1000_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);

  if (!radio.begin()) {
    Serial.println("Failed to initialize radio");
    while (1) { delay(10); }
  }

  radio.setPALevel(RF24_PA_LOW);
  radio.setChannel(108);
  radio.openWritingPipe(0xF0F0F0F0E1LL);
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
      server.send(200, "text/plain", "New Network credentials have been updated");
      delay(1000);
      ESP.restart();
      return;
    }
  }

  server.send(400, "text/plain", "Incorrect or incomplete request details!");
}

void dev_password() {
  server.send(200, "text/plain", password);
}

void deviceVersion() {
  server.send(200, "text/plain", version);
}

void devId() {
  server.send(200, "text/plain", dev_id);
}

void devModel() {
  server.send(200, "text/plain", dev_model);
}

void firmware() {
  server.send(200, "text/plain", dev_firmware);
}



void writeStringToEEPROM(String str, int address) {
  int length = str.length();
  for (int i = 0; i < length; i++) {
    EEPROM.write(address + i, str.charAt(i));
  }
  EEPROM.write(address + length, '\0');
  EEPROM.commit();
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
  for (int i = 0; i < EEPROM.length(); i++) {
    byte value = EEPROM.read(i);
    eepromContent += String(value) + "  ";
  }
  server.send(200, "text/plain", eepromContent);
}

void format() {
  for (int i = 0; i < 590; i++) {
    EEPROM.write(i, 0);
  }
  EEPROM.commit(); 
  server.send(200, "text/plain", "erased");
  delay(800);
  ESP.restart();
}

void formatAll() {
  for (int i = 0; i < EEPROM.length(); i++) {
    EEPROM.write(i, 0);
  }
  EEPROM.commit();
  server.send(200, "text/plain", "Memory will be formatted");
  delay(800);
  ESP.restart();
}


void readMPU6050() {
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  // Apply offsets to the accelerometer readings
  sensorData1.accX = a.acceleration.x;
  sensorData1.accY = a.acceleration.y;
  sensorData1.accZ = a.acceleration.z;

  // Apply offsets to the gyroscope readings
  sensorData1.gyroX = g.gyro.x;
  sensorData1.gyroY = g.gyro.y;
  sensorData1.gyroZ = g.gyro.z;

  //  // Apply offsets to the accelerometer readings
  // sensorData1.accX = a.acceleration.x + 0.18;
  // sensorData1.accY = a.acceleration.y - 0.12;
  // sensorData1.accZ = a.acceleration.z - 10.58;

  // // Apply offsets to the gyroscope readings
  // sensorData1.gyroX = g.gyro.x + 0.04;
  // sensorData1.gyroY = g.gyro.y - 0.07;
  // sensorData1.gyroZ = g.gyro.z - 0.01;
}


void readGPS() {
  while (SerialGPS.available() > 0) {
    if (gps.encode(SerialGPS.read())) {
      if (gps.location.isValid()) {
        sensorData2.lat = gps.location.lat();
        sensorData2.lon = gps.location.lng();
      }
    }
  }
}
void sendData() {
  radio.stopListening();
  radio.openWritingPipe(0xF0F0F0F0E1LL);
  bool report1 = radio.write(&sensorData1, sizeof(sensorData1));
  delay(10); // Small delay between transmissions
  
  radio.openWritingPipe(0xF0F0F0F0E2LL);
  bool report2 = radio.write(&sensorData2, sizeof(sensorData2));
  
  if (!report1 || !report2) {
    Serial.println("Failed to send data");
  } else {
    Serial.println("Data sent successfully");
  }

  // Debug: Print the data being sent
  Serial.print("Sending: ");
  Serial.print("AccX: "); Serial.print(sensorData1.accX);
  Serial.print(", AccY: "); Serial.print(sensorData1.accY);
  Serial.print(", AccZ: "); Serial.print(sensorData1.accZ);
  Serial.print(", GyroX: "); Serial.print(sensorData1.gyroX);
  Serial.print(", GyroY: "); Serial.print(sensorData1.gyroY);
  Serial.print(", GyroZ: "); Serial.print(sensorData1.gyroZ);
  Serial.print(", Lat: "); Serial.print(sensorData2.lat, 6);
  Serial.print(", Lon: "); Serial.print(sensorData2.lon, 6);
  Serial.print(", Device Name: "); Serial.println(sensorData2.deviceName);
}
void displayInfo() {
  Serial.print("Acc X: "); Serial.print(sensorData1.accX);
  Serial.print(" Y: "); Serial.print(sensorData1.accY);
  Serial.print(" Z: "); Serial.print(sensorData1.accZ);
  Serial.print(" | Gyro X: "); Serial.print(sensorData1.gyroX);
  Serial.print(" Y: "); Serial.print(sensorData1.gyroY);
  Serial.print(" Z: "); Serial.print(sensorData1.gyroZ);
  Serial.print(" | Lat: "); Serial.print(sensorData2.lat, 6);
  Serial.print(" Lon: "); Serial.print(sensorData2.lon, 6);
  Serial.print(" | Device Name: "); Serial.println(sensorData2.deviceName);
}

void handleState() {
  String storedData = 
    String(sensorData1.accX) + "#" + String(sensorData1.accY) + "#" + String(sensorData1.accZ) + "#" +
    String(sensorData1.gyroX) + "#" + String(sensorData1.gyroY) + "#" + String(sensorData1.gyroZ) + "#" +
    String(sensorData2.lat, 6) + "#" + String(sensorData2.lon, 6) + "#" + sensorData2.deviceName;
  server.send(200, "text/plain", storedData);
}
