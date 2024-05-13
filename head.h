//headers
void writeStringToEEPROM(String str, int address);
void saveFloatToEEPROM(float value, int location);
float readFloatFromEEPROM(int address);
String readStringFromEEPROM(int address);
long readNumberFromEEPROM(int address);
void resetEsp();
void handleState();
void handleRoot();
void new_password();
void handleNewSSID();
void dev_password();
void deviceVersion();
void devId();
void devModel();
void format();
void handleRoot();
void switch_1_on();
void switch_1_on_remote();
void switch_1_off();
void switch_1_off_remote();
void switch_2_on();
void switch_2_off();
void switch_2_on_remote();
void switch_2_off_remote();
void RunTheCar();
void RunTheCarRemote();
void StopTheCar();
void StopTheCarRemote();
void StopTheCarDirectly();
void SW_state();
void button3_pin_state();
void open_car();
void close_car();
void dev_password();
void authOpen();
void handleOpenStateFlag();
void authLock();
void authPower();
void authRun();
void codesData();
void handleState();
void resetPower();
void resetRun();
void resetOpen();
void resetLock();
void resetBox();
void resetAll();
void handle_openBox();
void authBox();
void open_car_remote();
void handle_openBox_remote();
void close_car_remote();
void handleSetTimer();
void handleAutoStart();
void theTimer();
void handleTimerState();
void RunTheCarAgain();
void handleSetNumOfTries();
void handleMemory();
void formatAll();
void handleDefaultNetwork();
void handleNewNetwork();
//device information
String version="5"; 
String dev_id="92uoslnw";
String dev_model="A5";