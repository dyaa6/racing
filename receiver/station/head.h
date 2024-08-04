//headers
void handleRoot();
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
void printData();

bool connectToWiFiFromEEPROM();
void setupAP();
void handleSubmit();
void sendDataToServer();

//device information
String version="1"; 
String dev_id="1";
String dev_model="A1";
String dev_firmware="1.0";
