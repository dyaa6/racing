//headers
void initEEPROM();
void initWiFi();
void setupServer();
void handleFirmwareUpload();
void setupSensors();
void handleNewNetwork();
void handleDefaultNetwork();
void handleRoot();
void dev_password();
void deviceVersion();
void devId();
void devModel();
void handleState();
void writeStringToEEPROM(String str, int address);
String readStringFromEEPROM(int address);
void readMPU6050();
void displayInfo();
void sendData();
void handleMemory();
void format();
void formatAll();

//device information
float deviceNum=2.0;
String version="1"; 
String dev_id="92uoslnw";
String dev_model="A1";
String dev_firmware="1.0";
