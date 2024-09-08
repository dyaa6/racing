#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <EEPROM.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_MPU6050.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>


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

String our_default_ssid = "node 3"; // default
const char* default_password = "";
String ssid = "";
String password = "";

ESP8266WebServer server(80);

Adafruit_MPU6050 mpu;
RF24 radio(4, 16); // CE to GPIO 4, CSN to GPIO 16

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

unsigned long previousMillis = 0;
bool radioFlag=false;
const unsigned long sensorInterval = 50; // 50 milliseconds interval for sensor readings
const int totalRAM = 81920;
void setup() {
  EEPROM.begin(1024);
  Serial.begin(115200);
  // Initialize EEPROM and WiFi
  initEEPROM();
  initWiFi();

  // Initialize the server and sensors
  server.begin();

  setupServer();
  setupSensors();

  Serial.println("Initialization complete");
}

void loop() {
  server.handleClient();
  unsigned long currentMillis = millis();
   if (currentMillis - previousMillis >= sensorInterval) {
    previousMillis = currentMillis;
    readMPU6050();  // Read data from the MPU6050 sensor
    sendData();
  }
}

void initEEPROM() {
  if (EEPROM.read(Net_F)) {
    password = readStringFromEEPROM(PASSWORD_ADDR);
    ssid = readStringFromEEPROM(SSID_ADDR);
  } else {
    password = readStringFromEEPROM(Def_Password);
    ssid = readStringFromEEPROM(Def_SSID);
  }

  if (password.isEmpty() || ssid.isEmpty() || password.length() > 30 || ssid.length() > 30) {
    ssid = our_default_ssid;
    password = default_password;
  }

  strncpy(sensorData2.deviceName, ssid.c_str(), sizeof(sensorData2.deviceName) - 1);
  sensorData2.deviceName[sizeof(sensorData2.deviceName) - 1] = '\0'; // Null-terminate
}

void initWiFi() {
  WiFi.disconnect(true);
  delay(100);
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid.c_str(), password.c_str());

  Serial.print("Access Point started with SSID: ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.softAPIP());
}

void setupServer() {
  // Define server routes
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

  // Handle file upload for firmware update
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
  }, handleFirmwareUpload);

  server.onNotFound([]() {
    server.send(404, "text/plain", "Not Found");
  });

  // Start the server
  server.enableCORS(true);
  server.begin();
  Serial.println("Web server started");
}

void handleFirmwareUpload() {
  HTTPUpload& upload = server.upload();
  if (upload.status == UPLOAD_FILE_START) {
    Serial.printf("Update: %s\n", upload.filename.c_str());
    if (!Update.begin(1024 * 1024)) { // 1 MB max size
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
}

void setupSensors() {
  Wire.begin(10, 5); // SDA to GPIO 10, SCL to GPIO 5

  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    //while (1) { delay(10); }
  }

  mpu.setAccelerometerRange(MPU6050_RANGE_16_G); // Less sensitive
  mpu.setGyroRange(MPU6050_RANGE_1000_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);

  if (!radio.begin()) {
    Serial.println("Failed to initialize radio");
    //while (1) { delay(10); }
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
  String value;
  char data = EEPROM.read(address);
  while (data != '\0' && address < EEPROM.length()) {
    value += data;
    data = EEPROM.read(++address);
  }
  return value;
}

void readMPU6050() {
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  sensorData1.accX = a.acceleration.x;
  sensorData1.accY = a.acceleration.y;
  sensorData1.accZ = a.acceleration.z;
  sensorData1.gyroX = g.gyro.x;
  sensorData1.gyroY = g.gyro.y;
  sensorData1.gyroZ = g.gyro.z;
}

void sendData() {
  if(radioFlag){
  radio.write(&sensorData1, sizeof(sensorData1));
  radioFlag=!radioFlag;
  }
  else{
  radio.write(&sensorData2, sizeof(sensorData2));
  radioFlag=!radioFlag;
  }
}


void handleState() {
  
  int freeHeap = ESP.getFreeHeap();
  int usedHeap = totalRAM - freeHeap;

  String RAM = "Used ";
  RAM += usedHeap;
  RAM += " from ";
  RAM += totalRAM;
  RAM += " bytes";

  String storedData = 
    String(sensorData1.accX) + "#" + String(sensorData1.accY) + "#" + String(sensorData1.accZ) + "#" +
    String(sensorData1.gyroX) + "#" + String(sensorData1.gyroY) + "#" + String(sensorData1.gyroZ) + "#" +
    "null" + "#" + "null" + "#" + sensorData2.deviceName + "#" + RAM;
  server.send(200, "text/plain", storedData);
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
  server.send(200, "text/plain", "Cleared");
}

void formatAll() {
  for (int i = 0; i < EEPROM.length(); i++) {
    EEPROM.write(i, 0);
  }
  EEPROM.commit();
  server.send(200, "text/plain", "Fully Cleared");
}
