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
#include <HTTPClient.h>
#include <Ticker.h>
// Include the relevant headers
#include "head.h"
#include "main.h"
#include "settings.h"
#include "network.h"
#include "update_html.h"
#include "sccess.h"
#include "fail.h"
#include "defaultNetwork.h"
#include "mode.h"

#define PASSWORD_ADDR 2
#define SSID_ADDR 45
#define MODE_ADDR 90
#define Def_SSID 600
#define Def_Password 640
#define Net_F 179

const char* apSSID = "ESP32_Config";
const char* apPassword = "";

String ssid = "";
String password = "";

const char* deviceName = "node D";
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

bool mode=false; //http send mode

Ticker ticker;
void setup() {
  EEPROM.begin(1024);
  EEPROM.read(MODE_ADDR)? mode=true:false;
  
  Serial.begin(115200);
  Serial.println("Starting setup...");
    strncpy(sensorData2.deviceName, deviceName, sizeof(sensorData2.deviceName) - 1);
    // Ensure the string is null-terminated
    sensorData2.deviceName[sizeof(sensorData2.deviceName) - 1] = '\0';

  if (!connectToWiFiFromEEPROM()) {
    setupAP();  // Setup access point if WiFi connection fails
  }
  
  setupServer();
  setupSensors();
  ticker.attach(2, sendDataMode);
  Serial.println("Initialization complete");
}

void loop() {
  server.handleClient();
  readMPU6050();
  readGPS();
  //displayInfo();
  //delay(50);
}

void setupServer() {
  server.on("/state", handleState);
  server.on("/format", format);
  server.on("/dev_ver", deviceVersion);
  server.on("/dev_id", devId);
  server.on("/dev_model", devModel);
  server.on("/firmware", firmware);
  server.on("/formatAll", formatAll);
  server.on("/showMemory", handleMemory);
  server.on("/de", []() { server.send(200, "text/html", defaultNet); });
  server.on("/index.html", []() { server.send(200, "text/html", index_html); });
  server.on("/index", []() { server.send(200, "text/html", index_html); });
  server.on("/", []() { server.send(200, "text/html", index_html); });
  server.on("/update_p.html", []() { server.send(200, "text/html", update); });
  server.on("/settings.html", []() { server.send(200, "text/html", settings); });
  server.on("/connect", []() { server.send(200, "text/html", network); });
  server.on("/setmode", handleSetMode);
  server.on("/mode", []() { server.send(200, "text/html", mode_page); });
  //server.on("/connect", []() { server.send(200, "text/html", handleConnect); });
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
    //while (1) { delay(10); }
  }

  radio.setPALevel(RF24_PA_LOW);
  radio.setChannel(108);
  radio.openWritingPipe(0xF0F0F0F0E1LL);
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
  
  // if (!report1 || !report2) {
  //   Serial.println("Failed to send data");
  // } else {
  //   Serial.println("Data sent successfully");
  // }

  // Debug: Print the data being sent
  // Serial.print("Sending: ");
  // Serial.print("AccX: "); Serial.print(sensorData1.accX);
  // Serial.print(", AccY: "); Serial.print(sensorData1.accY);
  // Serial.print(", AccZ: "); Serial.print(sensorData1.accZ);
  // Serial.print(", GyroX: "); Serial.print(sensorData1.gyroX);
  // Serial.print(", GyroY: "); Serial.print(sensorData1.gyroY);
  // Serial.print(", GyroZ: "); Serial.print(sensorData1.gyroZ);
  // Serial.print(", Lat: "); Serial.print(sensorData2.lat, 6);
  // Serial.print(", Lon: "); Serial.print(sensorData2.lon, 6);
  // Serial.print(", Device Name: "); Serial.println(sensorData2.deviceName);
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


bool connectToWiFiFromEEPROM() {
  String ssid = readStringFromEEPROM(SSID_ADDR);
  String password = readStringFromEEPROM(PASSWORD_ADDR);

  Serial.print("Read SSID from EEPROM: ");
  Serial.println(ssid);
  Serial.print("Read Password from EEPROM: ");
  Serial.println(password);

  if (ssid.isEmpty()) {
    Serial.println("SSID is empty, cannot connect to WiFi.");
    return false;
  }

  WiFi.begin(ssid.c_str(), password.isEmpty() ? NULL : password.c_str());
  Serial.print("Connecting to ");
  Serial.println(ssid);

  unsigned long startTime = millis();
  while (WiFi.status() != WL_CONNECTED) {
    if (millis() - startTime > 10000) {
      Serial.println("Failed to connect to WiFi within timeout.");
      return false;
    }
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  return true;
}


void setupAP() {
  WiFi.mode(WIFI_AP);
  WiFi.softAP(apSSID, apPassword);

  //server.on("/", handleRoot);
  server.on("/submit", HTTP_POST, []() {
    ssid = server.arg("ssid");
    password = server.arg("password");
    if (ssid.length() > 0) {
      writeStringToEEPROM(ssid, SSID_ADDR);
      writeStringToEEPROM(password, PASSWORD_ADDR);
      EEPROM.commit();
      server.send(200, "text/html", "<html><body><h1>Credentials saved!</h1><p>ESP32 will now restart.</p></body></html>");
      delay(1000);
      ESP.restart();
    } else {
      server.send(400, "text/html", "<html><body><h1>Error</h1><p>SSID too short.</p></body></html>");
    }
  });

  server.begin();

  Serial.println();
  Serial.println("WiFi AP started");
  Serial.print("SSID: ");
  Serial.println(apSSID);
  Serial.print("Password: ");
  Serial.println(apPassword);
  Serial.print("IP Address: ");
  Serial.println(WiFi.softAPIP());
}

void handleConnect() {
  String html = "<html><body><h1>ESP32 Network Configuration</h1><form action='/submit' method='post'>"
                "SSID: <input type='text' name='ssid'><br>"
                "Password: <input type='password' name='password'><br>"
                "<input type='submit' value='Connect'>"
                "</form></body></html>";
  server.send(200, "text/html", html);
}



void sendDataToServer() {
  //printData();

  // Check if data is zero
  if (sensorData1.accX == 0 || sensorData1.accY == 0 || sensorData1.accZ == 0 ||
      sensorData1.gyroX == 0 || sensorData1.gyroY == 0 || sensorData1.gyroZ == 0) {
    Serial.println("Some data values are zero, not sending to server.");
    return;
  }

  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;

    // Construct the URL with the query parameters
    String url = "https://nodemcu.hyantalm.com/save_data.php";
    url += "?accX=" + String(sensorData1.accX, 6) +
           "&accY=" + String(sensorData1.accY, 6) +
           "&accZ=" + String(sensorData1.accZ, 6) +
           "&gyroX=" + String(sensorData1.gyroX, 6) +
           "&gyroY=" + String(sensorData1.gyroY, 6) +
           "&gyroZ=" + String(sensorData1.gyroZ, 6) +
           "&lat=" + String(sensorData2.lat, 6) +
           "&lon=" + String(sensorData2.lon, 6) +
           "&deviceName=" + urlencode(sensorData2.deviceName);

    // Print the URL for debugging
    Serial.println("URL: " + url);

    // Begin the request
    http.begin(url);
    int httpResponseCode = http.GET();

    if (httpResponseCode > 0) {
      String response = http.getString();
      sensorData1.accX=0;
      sensorData1.accY=0;
      sensorData1.accZ=0;
      sensorData1.gyroX=0;
      sensorData1.gyroY=0;
      sensorData1.gyroZ=0;
      sensorData2.lat=0;
      sensorData2.lon=0;
      Serial.println("HTTP Response code: " + String(httpResponseCode));
      Serial.println("Server response: " + response);
      Serial.println("Data successfully sent to the server.");
    } else {
      Serial.print("Error on sending GET: ");
      Serial.println(httpResponseCode);
      Serial.println("URL: " + url);
      Serial.println("WiFi Status: " + String(WiFi.status()));
      Serial.println("HTTP Error: " + String(http.errorToString(httpResponseCode).c_str()));
    }

    http.end();
  } else {
    Serial.println("WiFi Disconnected");
    Serial.println("WiFi Status: " + String(WiFi.status()));
  }
}

void handleSetMode() {
  if (server.hasArg("mode")) {
    // Get the mode from the request (expects "true" or "false")
    String modeValue = server.arg("mode");

    // Update the mode variable
    if (modeValue == "true") {
      mode = true;
    } else if (modeValue == "false") {
      mode = false;
    } else {
      server.send(400, "text/plain", "Invalid mode value. Use 'true' or 'false'");
      return;
    }

    // Save the mode to EEPROM (store as 1 for true, 0 for false)
    EEPROM.write(MODE_ADDR, mode ? 1 : 0);
    EEPROM.commit();  // Make sure to commit the change

    // Send a response back to the client
    server.send(200, "text/plain", String("Mode updated successfully to: ") + (mode ? "true" : "false"));
    Serial.println(String("Mode updated to: ") + (mode ? "true" : "false"));
  } else {
    server.send(400, "text/plain", "Mode not provided. Use 'true' or 'false'");
  }
}

String urlencode(String str) {
  String encodedString = "";
  char c;
  char code0;
  char code1;
  char code2;
  for (int i = 0; i < str.length(); i++) {
    c = str.charAt(i);
    if (c == ' ') {
      encodedString += '+';
    } else if (isalnum(c)) {
      encodedString += c;
    } else {
      code1 = (c & 0xf) + '0';
      if ((c & 0xf) > 9) {
        code1 = (c & 0xf) - 10 + 'A';
      }
      c = (c >> 4) & 0xf;
      code0 = c + '0';
      if (c > 9) {
        code0 = c - 10 + 'A';
      }
      code2 = '\0';
      encodedString += '%';
      encodedString += code0;
      encodedString += code1;
    }
    yield();
  }
  return encodedString;
}

void sendDataMode() {
  mode ? sendDataToServer() : sendData();
}