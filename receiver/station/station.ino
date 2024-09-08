#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <EEPROM.h>
#include <Update.h>
#include <HTTPClient.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
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

// Define constants
#define PASSWORD_ADDR 2
#define SSID_ADDR 45
#define Def_SSID 600
#define Def_Password 640
#define Net_F 179

String our_default_ssid = "Receiver";
const char* default_password = "";
String ssid = "";
String password = "";

// Define nRF24L01 radio module
RF24 radio(4, 5);
const byte address[6] = "00001";


struct SensorData1 {
  float accX, accY, accZ;
  float gyroX, gyroY, gyroZ;
} __attribute__((packed));

struct SensorData2 {
  char deviceName[8];
  float lat, lon;
} __attribute__((packed));

SensorData1 receivedData1;
SensorData2 receivedData2;
unsigned long lastReceivedTime = 0;
const unsigned long timeoutDuration = 5000;

const char* apSSID = "ESP32_Config";
const char* apPassword = "12345678";

// Define web server
WebServer server(80);
Ticker ticker1;

void setup() {
  EEPROM.begin(1024);
  Serial.begin(115200);
  ticker1.attach(5, sendDataToServer);
  
  // Attempt to connect to WiFi using stored credentials
  if (!connectToWiFiFromEEPROM()) {
    setupAP();  // Setup access point if WiFi connection fails
  }

  // Define server routes
  server.on("/state", handleState);
  server.on("/currentPassword", dev_password);
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

  Serial.println("nRF24L01 Receiver Starting");
  if (!radio.begin()) {
    Serial.println("Radio hardware not responding!");
  }

  radio.setPALevel(RF24_PA_LOW);
  radio.setChannel(108);
  radio.openReadingPipe(1, 0xF0F0F0F0E1LL);
  radio.openReadingPipe(2, 0xF0F0F0F0E2LL);  // Add this line
  radio.startListening();
  Serial.println("Receiver initialized and listening...");
}

void loop() {
  server.handleClient();
  if (radio.available()) {
    uint8_t pipeNum;
    if (radio.available(&pipeNum)) {
      if (pipeNum == 1) {
        radio.read(&receivedData1, sizeof(receivedData1));
        Serial.println("Received data part 1");
      } else if (pipeNum == 2) {
        radio.read(&receivedData2, sizeof(receivedData2));
        Serial.println("Received data part 2");
        lastReceivedTime = millis();
        printData();  // Print data after receiving both parts
      }
    }
  } else if (millis() - lastReceivedTime > timeoutDuration) {
    delay(50);
  }
}
void printData() {
  Serial.println("Data received:");
  Serial.print("Acc X: "); Serial.print(receivedData1.accX);
  Serial.print(" Y: "); Serial.print(receivedData1.accY);
  Serial.print(" Z: "); Serial.println(receivedData1.accZ);
  Serial.print("Gyro X: "); Serial.print(receivedData1.gyroX);
  Serial.print(" Y: "); Serial.print(receivedData1.gyroY);
  Serial.print(" Z: "); Serial.println(receivedData1.gyroZ);
  Serial.print("Lat: "); Serial.print(receivedData2.lat, 6);
  Serial.print(" Lon: "); Serial.println(receivedData2.lon, 6);
  Serial.print("Device Name: "); Serial.println(receivedData2.deviceName);
  Serial.println();
}

void handleRoot() {
  String html = "<html><body><h1>ESP32 Network Configuration</h1><form action='/submit' method='post'>"
                "SSID: <input type='text' name='ssid'><br>"
                "Password: <input type='password' name='password'><br>"
                "<input type='submit' value='Connect'>"
                "</form></body></html>";
  server.send(200, "text/html", html);
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

void handleState() {
  String receivedData =
    String(receivedData1.accX) + "#" + String(receivedData1.accY) + "#" + String(receivedData1.accZ) + "#" +
    String(receivedData1.gyroX) + "#" + String(receivedData1.gyroY) + "#" + String(receivedData1.gyroZ) + "#" +
    String(receivedData2.lat, 6) + "#" + String(receivedData2.lon, 6) + "#" + receivedData2.deviceName;
  server.send(200, "text/plain", receivedData);
}

void writeStringToEEPROM(String str, int address) {
  Serial.print("Writing to EEPROM at address ");
  Serial.print(address);
  Serial.print(": ");
  Serial.println(str);
  
  int length = str.length();
  for (int i = 0; i < length; i++) {
    EEPROM.write(address + i, str[i]);
  }
  EEPROM.write(address + length, '\0');
  EEPROM.commit();
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



void handleMemory() {
    String eepromContent = "";
    // Read and display the entire EEPROM content
    for (int i = 0; i < EEPROM.length(); i++) {
        byte value = EEPROM.read(i);
        eepromContent += String(value) + "  ";
    }
    server.send(200, "text/plain", eepromContent);
}

void sendDataToServer() {
  printData();

  // Check if data is zero
  if (receivedData1.accX == 0 || receivedData1.accY == 0 || receivedData1.accZ == 0 ||
      receivedData1.gyroX == 0 || receivedData1.gyroY == 0 || receivedData1.gyroZ == 0) {
    Serial.println("Some data values are zero, not sending to server.");
    return;
  }

  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;

    // Construct the URL with the query parameters
    String url = "https://nodemcu.hyantalm.com/save_data.php";
    url += "?accX=" + String(receivedData1.accX, 6) +
           "&accY=" + String(receivedData1.accY, 6) +
           "&accZ=" + String(receivedData1.accZ, 6) +
           "&gyroX=" + String(receivedData1.gyroX, 6) +
           "&gyroY=" + String(receivedData1.gyroY, 6) +
           "&gyroZ=" + String(receivedData1.gyroZ, 6) +
           "&lat=" + String(receivedData2.lat, 6) +
           "&lon=" + String(receivedData2.lon, 6) +
           "&deviceName=" + urlencode(receivedData2.deviceName);

    // Print the URL for debugging
    Serial.println("URL: " + url);

    // Begin the request
    http.begin(url);
    int httpResponseCode = http.GET();

    if (httpResponseCode > 0) {
      String response = http.getString();
      receivedData1.accX=0;
      receivedData1.accY=0;
      receivedData1.accZ=0;
      receivedData1.gyroX=0;
      receivedData1.gyroY=0;
      receivedData1.gyroZ=0;
      receivedData2.lat=0;
      receivedData2.lon=0;
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

  server.on("/", handleRoot);
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

String readStringFromEEPROM(int address) {
  char data[100];
  int length = 0;
  for (int i = address; i < address + 100; i++) {
    data[length] = EEPROM.read(i);
    if (data[length] == '\0') {
      break;
    }
    length++;
  }
  String str = String(data);
  Serial.print("Read from EEPROM at address ");
  Serial.print(address);
  Serial.print(": ");
  Serial.println(str);
  return str;
}
