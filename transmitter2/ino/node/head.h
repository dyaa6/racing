//headers
void handleRoot();
void handleNewNetwork();
void handleDefaultNetwork();
void dev_password();
void deviceVersion();
void devId();
void devModel();
void handleState();
void writeStringToEEPROM(String str, int address);
long readNumberFromEEPROM(int address);
String readStringFromEEPROM(int address);
void handleMemory();
void format();
void formatAll();
//-------------------
void readMPU6050();
void readGPS();
void sendData();
void displayInfo();

//device information
String version="1"; 
String dev_id="92uoslnw";
String dev_model="A1";
String dev_firmware="1.0";
