// last edit 2024/3/1 AM
// EEPROM map: 2-45 for password, 45-85 for ssid, 88-120 for sw1 and sw2
// 120-150 for button 1, 150-180 for lock button, 250-280 is for open button.
// 300-330 for back box and 335 for box's flag
// 340 is for timer state, and 345 -380 is for timer value
// 385 is for timer state (one byte).
// 391 to 395 for try address
#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <EEPROM.h>
#include <RCSwitch.h>
#include <Update.h>
#include "head.h"
#include "main.h"
#include "remote.h"
#include "settings.h"
#include "network.h"
#include "onpress.h"
#include "timer.h"
#include "update_html.h"
#include "sccess.h"
#include "fail.h"
#include "power_img_R.h"
#include "power_img_G.h"
#include "defaultNetwork.h"
RCSwitch mySwitch;

String our_default_ssid = "Smart Car";//default
const char* default_password = "";
String ssid = "";
String password="";
 
#define sensor_pin 18
#define sw1 21
#define btn3 17 
#define open_pin 27
#define lock_pin 26
#define light_pin 33
#define jam_pin 25
#define box_pin 32
#define reset_pin 19


int run_flag=0;
bool isPressedDown=false;
bool x=0;


int sw1_code;
int btn1_code;
int lock_code;
int open_code;
int box_code;

// multible tries varibles
int numOfTries=-1; //-1 neans is not activated
unsigned long tryTime=0;
int totalTries=1; // 2 means 3 tries from 0 to 2

// Define the EEPROM address to store the password at
#define PASSWORD_ADDR 2
#define SSID_ADDR 45
#define SW1_ADD 88
#define BTN1_ADD 120
#define LOCK_ADD 150
#define OPEN_ADD 250
#define BOX_ADD 300
#define TimerValue 342
#define TRY_ADD 391

#define Def_SSID 600 // default ssid name location
#define Def_Password 640 // default password location


#define SW1_F 181
#define BTN1_F 183
#define LOCK_F 185
#define OPEN_F 187

#define Net_F 179
#define BOX_F 335
#define TimerState_F 340
#define AutoStart_F 385
#define TotalTries_F 390


bool isPowerOn=false;
int receiverPin = 39; // GPIO pin to which the receiver data pin is connected

bool period=false;
bool auto_start= false;

WebServer server(80);

void handleImageR() {
  server.sendHeader("Content-Type", "image/png");
  server.sendContent_P((const char *)image_data_R, sizeof(image_data_R));
}
void handleImageG() {
  server.sendHeader("Content-Type", "image/png");
  server.sendContent_P((const char *)image_data_G, sizeof(image_data_G));
}
unsigned long start_time = 0; //*********************

//timer variables
unsigned long previousMillis = 0;  // Stores the last time the counter was updated
const unsigned long interval = 1000;  // Interval in milliseconds (1 second)
long counter = 300;  // Initial count value
long tempCounter=counter;
bool Timer=false; //current timer state
bool TimerState= true;

//jam variables
unsigned long jamPrevMillis=0;
bool jamState=false;
int jamPeriod=22000;



IPAddress local_IP(2, 2, 2, 2);
IPAddress gateway(2, 2, 2, 2);
IPAddress subnet(255, 255, 255, 0);

void setup() {
  EEPROM.begin(1024);

  WiFi.softAPConfig(local_IP, gateway, subnet);

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
  mySwitch.enableReceive(receiverPin);

  mySwitch.setProtocol(0);  // Set the protocol to 0 (Autodetect)
  mySwitch.setPulseLength(350);  // Change to the appropriate pulse length in microseconds
 
  WiFi.disconnect();

  WiFi.softAP(ssid,password);


  TimerState = EEPROM.read(TimerState_F)? true : false; //false means it's activated 0
  //counter= EEPROM.read(TimerState_F)?  counter : readNumberFromEEPROM(TimerValue);
  if(!EEPROM.read(TimerState_F)){
    unsigned long theVal=readNumberFromEEPROM(TimerValue);
    if(!theVal==0 || !theVal=='0')
      counter=theVal; 
  }
  tempCounter=counter;
//WiFi.setOutputPower(20.5);
  if(EEPROM.read(OPEN_F)){
    long number = readNumberFromEEPROM(OPEN_ADD);
  open_code = number;
  }
  if(EEPROM.read(LOCK_F)){
    long number = readNumberFromEEPROM(LOCK_ADD);
    lock_code = number;
  }
  if(EEPROM.read(SW1_F)){
    long number = readNumberFromEEPROM(SW1_ADD);
    sw1_code = number;
  }
  if(EEPROM.read(BTN1_F)){
    long number = readNumberFromEEPROM(BTN1_ADD);
    btn1_code = number;
  }
  if(EEPROM.read(BOX_F)){
    long number = readNumberFromEEPROM(BOX_ADD);
    box_code = number;
  }
  if (EEPROM.read(AutoStart_F)){
    auto_start=true;
  }

  if (EEPROM.read(AutoStart_F)){
      int16_t number = readNumberFromEEPROM(TRY_ADD);
    totalTries = number;
  }


  pinMode(sensor_pin,INPUT);
  pinMode(sw1, OUTPUT);
  pinMode(box_pin,OUTPUT); 
  pinMode(btn3,OUTPUT);   
  pinMode(open_pin,OUTPUT);   
  pinMode(lock_pin,OUTPUT);   
  pinMode(light_pin,OUTPUT);   
  pinMode(jam_pin,OUTPUT);
  pinMode(reset_pin,INPUT);   



  server.on("/switch1On",switch_1_on);  
  server.on("/switch1Off",switch_1_off);
  //server.on("/switch2On",switch_2_on);  
  //server.on("/switch2Off",switch_2_off);
  server.on("/motorOn",RunTheCar);
  server.on("/motorOff",StopTheCar);
  server.on("/state",handleState);
  server.on("/button3_pin",button3_pin_state);
  server.on("/open",open_car);
  server.on("/lock",close_car);
  server.on("/trunk",handle_openBox);
  server.on("/glassUP",handle_glassUP);
  server.on("/glassDown",handle_glassDown);
  server.on("/currentPassword",dev_password);
  server.on("/format",format);
  server.on("/dev_ver",deviceVersion);
  server.on("/dev_id",devId);
  server.on("/dev_model",devModel);
  server.on("/authOpen", authOpen);
  server.on("/authLock", authLock);
  server.on("/authBox", authBox);  
  server.on("/authPower", authPower);
  server.on("/authRun", authRun);
  server.on("/remoteCodes", codesData);
  server.on("/resetPower",resetPower);
  server.on("/resetRun",resetRun);
  server.on("/resetOpen",resetOpen);
  server.on("/resetLock",resetLock);
  server.on("/resetBox",resetBox);
  server.on("/resetAll",resetAll);
  server.on("/switch1_state",SW_state);
  server.on("/setTimer",handleSetTimer);
  server.on("/setAuto",handleAutoStart);
  server.on("/timerState",handleTimerState);
  server.on("/autoState",handleAutoState);
  server.on("/networkConfig",handleNewNetwork);
  server.on("/defaultNetwork",handleDefaultNetwork);
  server.on("/formatAll",formatAll);
  server.on("/showMemory",handleMemory);
  server.on("/image-R.png", handleImageR);
  server.on("/image-G.png", handleImageG);
  server.on("/de",[](){
      server.send(200, "text/html", defaultNet);
  });
  // get RF states
  server.on("/opn_state",handleOpenStateFlag);
  server.on("/index",[](){
      server.send(200, "text/html", index_html);
  });
  server.on("/",[](){
      server.send(200, "text/html", index_html);
  });
  server.on("/update_p",[](){  server.send(200, "text/html", update); });
  server.on("/remote",[](){ server.send(200, "text/html", remote); });
  server.on("/settings",[](){ server.send(200, "text/html", settings); });
  server.on("/network",[](){ server.send(200, "text/html", network); });
  server.on("/onepress",[](){ server.send(200, "text/html", onpress); });
  server.on("/timer",[](){ server.send(200, "text/html", timer); });
  
  
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
  } //end setup
 
void loop() {
  server.handleClient();
  theTimer();
    if (mySwitch.available()) {
      int value = mySwitch.getReceivedValue();
      if (value == open_code) {
        open_car_remote();
      }
      else if (value == lock_code){
        close_car_remote();
      }
      else if (value == box_code){
        handle_openBox_remote();
      }
      else if (value == sw1_code){
         switch_1_off_remote();
        // delay(200);
        // switch_2_off_remote();
        isPowerOn=false;
      }
      else if(value == btn1_code){
        RunTheCarRemote();
        isPressedDown=true;
        delay(100);
      }
      mySwitch.resetAvailable();
      delay(50);
    }
    else{
      if(isPressedDown){
      if(!auto_start)
        StopTheCarRemote();
      isPressedDown=false;
      //delay(100);
      }
    }

    //stop the car();
    if(digitalRead(sensor_pin)){ // if the engine is started
      if(auto_start){
        digitalWrite(btn3,LOW);
        numOfTries=-1;
    }
        // if (!TimerState)  //turn off the timer only if it is enabled by default
        //   Timer=true;
        // else
        //   Timer=false;
        // counter=tempCounter;
        }
        //**********************
  if(run_flag==1){
    if (start_time == 0)
      start_time = millis();
    
    // check if four seconds have elapsed since the timer started
    if ((millis() - start_time) >= 3500) {
        digitalWrite(btn3,LOW);
        start_time = 0; // reset the start time of the timer
        run_flag = 0; 
        if(auto_start){
          if (numOfTries<=totalTries && numOfTries>=0){
              numOfTries+=1;
              if(tryTime==0)
                tryTime=millis();
          }
          else{
            numOfTries=-1;
          }
        }
              
      }
    } else {
      start_time = 0; // reset the start time of the timer if run_flag is false
          }
if (auto_start){
  if (millis() - tryTime>=3000){
    if(numOfTries<=totalTries && numOfTries>=0){
    RunTheCarAgain();
    }
    tryTime=0;
  }  
}

if(!digitalRead(sw1)){
    if (millis()  >= 21600000) { // 6 hours  21600000
      delay(100);
      if(!digitalRead(sensor_pin)){
        ESP.restart();
    }}}
  if(!digitalRead(sensor_pin)){
      if(auto_start ){
        if (!TimerState && digitalRead(sw1))  //start the timer
          Timer=true;
        else
          Timer=false;
        counter=tempCounter;
      }
    }


  //stop the jam
  if(jamState){
    if(millis()-jamPrevMillis>=jamPeriod){
      digitalWrite(jam_pin,LOW);
      jamState=false;
  }}

if(!digitalRead(reset_pin)){ //read zero
  delay(5000);
  if(!digitalRead(reset_pin)){
    digitalWrite(open_pin,HIGH);
    delay(400);
    digitalWrite(open_pin,LOW);
    delay(100);
    digitalWrite(lock_pin,HIGH);
    delay(400);
    digitalWrite(lock_pin,LOW);
    format();
  }
}


  }//end loop




    void handleRoot() {
          server.send(200, "text/plain","developed by ACSD");
            }
                      
    void switch_1_on() {
          digitalWrite(sw1,HIGH);
          server.send(200, "text/plain","DONE");
          }
    void switch_1_on_remote() {
          digitalWrite(sw1,HIGH);
              }
    void switch_1_off() {
          digitalWrite(sw1,LOW);
          //digitalWrite(sw2,LOW);
          digitalWrite(btn3,LOW);
          Timer=false;
          counter=tempCounter;
          numOfTries=-1; //termenate the tries becaues the user stoped the motor
          server.send(200, "text/plain","DONE");
            }
    void switch_1_off_remote() {
          digitalWrite(sw1,LOW);
          //digitalWrite(sw2,LOW);
          digitalWrite(btn3,LOW);
          Timer=false;
          counter=tempCounter;
          numOfTries=-1; //termenate the tries becaues the user stoped the motor
          }
   
    void RunTheCar() {
          if(!digitalRead(sensor_pin)){
            if(!digitalRead(sw1)){
              digitalWrite(sw1,HIGH);
              //digitalWrite(sw2,HIGH);
            }
            digitalWrite(btn3,HIGH);
            run_flag=1;
            isPressedDown=false;
            if(auto_start)
               numOfTries=0;
            server.send(200, "text/plain","motor On");
          }
   }
    void RunTheCarRemote() {
        if(digitalRead(sw1)==LOW){
              digitalWrite(sw1,HIGH);
              digitalWrite(light_pin,HIGH);
              // delay(200);
              // digitalWrite(sw2,HIGH);
              delay(400);
              digitalWrite(light_pin,LOW);
            }
            
      if(!digitalRead(sensor_pin)){ // the engine is not working
            digitalWrite(btn3,HIGH);
            run_flag=1;
      }
      if(auto_start)
        numOfTries=0;
   }
    void RunTheCarAgain() {
        if(digitalRead(sw1)==LOW){
              digitalWrite(sw1,HIGH);
              digitalWrite(light_pin,HIGH);
              // delay(200);
              // digitalWrite(sw2,HIGH);
              delay(400);
              digitalWrite(light_pin,LOW);
            }
            
      if(!digitalRead(sensor_pin)){ // the engine is not working
            digitalWrite(btn3,HIGH);
            run_flag=1;
      }
   }

    void StopTheCar() {
        if(!auto_start){
            digitalWrite(btn3,LOW);
          if(!auto_start){ //only start the timer when the the manual modeF(when left up the button)
            if (!TimerState)
              Timer=true;
            else
              Timer=false;
              counter=tempCounter;
        }}
          server.send(200, "text/plain","motor off");
            }
    void StopTheCarRemote() {
      if (mySwitch.available()) {
          digitalWrite(btn3,HIGH);
          period=false;
      }
      else{
          period=true;
        if(digitalRead(btn3)) { 
          if (!mySwitch.available() && period){
            delay(10);
            if (!mySwitch.available() && period){
                delay(10);
                if (!mySwitch.available() && period){
                delay(10);
                if (!mySwitch.available() && period){
                  delay(10);
                  if (!mySwitch.available() && period){
                    delay(10);
                    if (!mySwitch.available() && period){
                        delay(10);
                      if (!mySwitch.available() && period){
                        delay(10);
                        if (!mySwitch.available() && period){
                          delay(10);
                          if (!mySwitch.available() && period){
                            delay(10);
                            if (!mySwitch.available() && period){
                              delay(10);
                              if (!mySwitch.available() && period){
                                delay(10);
                                if (!mySwitch.available() && period){
                                  delay(10);
                                  if (!mySwitch.available() && period){
                                      delay(10);
                                    if (!mySwitch.available() && period){
                                        delay(10);
                                      if (!mySwitch.available() && period){
                                        digitalWrite(btn3,LOW);
                                      }
                                      else{
                                        digitalWrite(btn3,HIGH);
                                        period=false;
                                      }
                                    }
                                    else{
                                      digitalWrite(btn3,HIGH);
                                      period=false;
                                    }
                                  }
                                  else{
                                    digitalWrite(btn3,HIGH);
                                    period=false;
                                  }
                                }
                                else{
                                  digitalWrite(btn3,HIGH);
                                  period=false;
                                }
                              }
                              else{
                                digitalWrite(btn3,HIGH);
                                period=false;
                              }
                            }
                            else{
                              digitalWrite(btn3,HIGH);
                              period=false;
                            }
                          }
                          else{
                            digitalWrite(btn3,HIGH);
                            period=false;
                          }
                        }
                        else{
                          digitalWrite(btn3,HIGH);
                          period=false;
                        }
                      }
                      else{
                        digitalWrite(btn3,HIGH);
                        period=false;
                      }
                    }
                    else{
                        digitalWrite(btn3,HIGH);
                        period=false;
                    }
                  }
                  else{
                    digitalWrite(btn3,HIGH);
                    period=false;
                  }
                }
                  else{
                    digitalWrite(btn3,HIGH);
                    period=false;
                  }
          }
          else{
            digitalWrite(btn3,HIGH);
            period=false;
          }
        }
            else{
              digitalWrite(btn3,HIGH);
              period=false;
            }
        }
        else{
          digitalWrite(btn3,HIGH);
          period=false;
        }
        }
        }
        if (!TimerState)
          Timer=true;
        else
          Timer=false;
          counter=tempCounter;
              }   
    void SW_state() {
      String sw_state=(digitalRead(sw1)==true)? "on":"off";
      server.send(200, "text/plain",sw_state);
      } 


   void button3_pin_state() {
          String btn3_st=(digitalRead(sw1)==true)? "yes":"no";
       server.send(200, "text/plain",btn3_st);
  }            

   void open_car() {
      digitalWrite(open_pin,HIGH);
      digitalWrite(light_pin,HIGH);
      delay(250);
      digitalWrite(light_pin,LOW);
      delay(250);
      digitalWrite(open_pin,LOW);
      digitalWrite(light_pin,HIGH);
      server.send(200, "text/plain","opened");
      delay(250);
      digitalWrite(light_pin,LOW);
  }
   void open_car_remote() {
      digitalWrite(open_pin,HIGH);
      digitalWrite(light_pin,HIGH);
      delay(250);
      digitalWrite(light_pin,LOW);
      delay(250);
      digitalWrite(open_pin,LOW);
      digitalWrite(light_pin,HIGH);
      delay(250);
      digitalWrite(light_pin,LOW);
  }
  void handle_openBox(){
      digitalWrite(box_pin,HIGH);
      digitalWrite(light_pin,HIGH);
      delay(500);
      digitalWrite(box_pin,LOW);
      digitalWrite(light_pin,LOW);
      server.send(200, "text/plain","box opend");
}
  void handle_openBox_remote(){
      digitalWrite(box_pin,HIGH);
      digitalWrite(light_pin,HIGH);
      delay(400);
      digitalWrite(box_pin,LOW);
      digitalWrite(light_pin,LOW);
}  
void handle_rais_jam(){
      jamState=true;
      digitalWrite(box_pin,HIGH);
      jamPrevMillis=millis();
}
 void close_car() {
      jamState=true;
      digitalWrite(jam_pin,HIGH);
      jamPrevMillis=millis();
      digitalWrite(lock_pin,HIGH);
      digitalWrite(light_pin,HIGH);
      delay(500);
      digitalWrite(lock_pin,LOW);
      digitalWrite(light_pin,LOW);
      server.send(200, "text/plain","locked");
  }
 void close_car_remote() {
      jamState=true;
      digitalWrite(jam_pin,HIGH);
      jamPrevMillis=millis();
      digitalWrite(lock_pin,HIGH);
      digitalWrite(light_pin,HIGH);
      delay(500);
      digitalWrite(lock_pin,LOW);
      digitalWrite(light_pin,LOW);
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

void authOpen() {
  int value = 0;
  int count=0;
  while (value == 0) {
    
    if (mySwitch.available()) {
      value = mySwitch.getReceivedValue();
      
      if (value != 0) {
        // Save the key code in EEPROM memory
        writeStringToEEPROM(String(value),OPEN_ADD);
        
        EEPROM.write(OPEN_F, 1);
        EEPROM.commit();
        
        open_code = value;
        server.send(200, "text/plain", String(value));
      }
      
      mySwitch.resetAvailable();
    }
    delay(20);
    count++;
    if(count >= 350)
        break;
  } //end while
}//end authopen

  void handleOpenStateFlag() {
  int openState = EEPROM.read(OPEN_F);
  server.send(200, "text/plain", String(openState));
}


void authLock() {
  int value = 0;
  int count=0;
  while (value == 0) {
    
    if (mySwitch.available()) {
      value = mySwitch.getReceivedValue();
      
      if (value != 0) {
        // Save the key code in EEPROM memory
        writeStringToEEPROM(String(value),LOCK_ADD);
        
        EEPROM.write(LOCK_F, 1);
        EEPROM.commit();
        
        lock_code = value;
        server.send(200, "text/plain", String(value));
      }
      
      mySwitch.resetAvailable();
    }
    delay(20);
    count++;
    if(count >= 350)
        break;
  } //end while
} //end authLock

void authBox() {
  int value = 0;
  int count=0;
  while (value == 0) {
    
    if (mySwitch.available()) {
      value = mySwitch.getReceivedValue();
      
      if (value != 0) {
        // Save the key code in EEPROM memory
        writeStringToEEPROM(String(value),BOX_ADD);
        
        EEPROM.write(BOX_F, 1);
        EEPROM.commit();
        
        box_code = value;
        server.send(200, "text/plain", String(value));
      }
      mySwitch.resetAvailable();
    }
    delay(20);
    count++;
    if(count >= 300) // 6 seconds
        break;
  } //end while
} //end authBox

void authPower() {
  int value = 0;
  int count=0;
  while (value == 0) {
    if (mySwitch.available()) {
      value = mySwitch.getReceivedValue();
      
      if (value != 0) {
        // Save the key code in EEPROM memory
        writeStringToEEPROM(String(value),SW1_ADD);
        EEPROM.write(SW1_F, 1);
        EEPROM.commit();
        
        sw1_code = value;
        server.send(200, "text/plain", String(value));
      }
      
      mySwitch.resetAvailable();
    }
    delay(20);
    count++;
    if(count >= 350)
        break;
  } //end while
} //end authPower
void authRun() {
  int value = 0;
  int count=0;
  while (value == 0) {
    if (mySwitch.available()) {
      value = mySwitch.getReceivedValue();

      if (value != 0) {
        writeStringToEEPROM(String(value),BTN1_ADD);
        EEPROM.write(BTN1_F, 1);
        EEPROM.commit();
        
        btn1_code = value;
        server.send(200, "text/plain", String(value));
      }
      
      mySwitch.resetAvailable();
    }
    delay(20);
    count++;
    if(count >= 350)
        break;
  } //end while
  delay(1000);
}  //end authRun

   void codesData(){
       String st1=String(sw1_code);
      String st2=String(btn1_code);
      String st3=String(open_code);
      String st4=String(lock_code);
      String st5=String(box_code);
      server.send(200, "text/plain",st1+"#"+st2+"#"+st3+"#"+st4+"#"+st5);
  }
 void handleState() {
        String st1=(digitalRead(sw1))? "on":"off";
        String st2=(digitalRead(btn3))? "on":"off";
        String sens=(digitalRead(sensor_pin))? "on":"off";
        String countVal= TimerState? "":String(counter);
        //double temp =0;// analogValueToTemperature(); 
        server.send(200, "text/plain",st1+"#"+st2+"#"+sens+"#"+countVal);//+"#"+String(temp));
      } 
      void handleTimerState(){
        String countVal= TimerState? "":String(counter);
        String timerSt=TimerState?"off":"on";
        server.send(200,"text/plain",countVal+"#"+timerSt);
      }
      void handleAutoState(){
        String autoState=auto_start?"on":"off";
        String numberOfTries=String(totalTries+1);
        server.send(200,"text/plain",autoState+"#"+numberOfTries);
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

void theTimer(){
  if(Timer){
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    if (counter > 0) {
      counter--;
    } else {
      switch_1_off_remote();
      Timer=false;
      counter=tempCounter;
    }
  }
  }
}

void handleSetTimer(){
      if (server.hasArg("value")) {
      long period1 = (server.arg("value")).toInt();
      if(period1){
        counter= period1;
        tempCounter= period1;
        writeStringToEEPROM(String(period1),TimerValue);
      }
      server.send(200, "text/plain", "Timer enabled");
  } else {
      server.send(400, "text/plain", "Bad Request");
  }
  if (server.hasArg("state")){
    if (server.arg("state") == "on"){
        TimerState= false;
         EEPROM.write(TimerState_F, 0);
        EEPROM.commit();
    }
    else{
      TimerState=true;//true means disabled in our case
      EEPROM.write(TimerState_F, 1);
      EEPROM.commit();
    }
  }
}
void handleAutoStart(){
  if (server.hasArg("state")){
    if (server.arg("state") == "on"){
        auto_start= true;
         EEPROM.write(AutoStart_F, 1);
        EEPROM.commit();
        server.send(200, "text/plain", "auto mode");
    }
    else{
      auto_start=false;//true means disabled in our case
      EEPROM.write(AutoStart_F, 0);
      EEPROM.commit();
      server.send(200, "text/plain", "manual mode");
    }
  }
     if (server.hasArg("numOfTries")) {
      int tempval = (server.arg("numOfTries")).toInt();
        totalTries= tempval-1; //to start form 0
        writeStringToEEPROM(String(totalTries),TRY_ADD);
  }
  
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


void handle_glassUP(){
  digitalWrite(jam_pin,HIGH);
}
void handle_glassDown(){
  digitalWrite(jam_pin,LOW);
}
  void resetPower(){
    sw1_code=0;
    EEPROM.write(SW1_F, 0);
    EEPROM.commit();
    server.send(200, "text/plain", "erased");
  }
  void resetRun(){
    btn1_code=0;
    EEPROM.write(BTN1_F, 0);
    EEPROM.commit();
    server.send(200, "text/plain", "erased");
  }
  void resetOpen(){
    open_code=0;
    EEPROM.write(OPEN_F, 0);
    EEPROM.commit();
    server.send(200, "text/plain", "erased");
  }
  void resetLock(){
    lock_code=0;
    EEPROM.write(LOCK_F, 0);
    EEPROM.commit();
    server.send(200, "text/plain", "erased");
  }
  void resetBox(){
    box_code=0;
    EEPROM.write(BOX_F, 0);
    EEPROM.commit();
    server.send(200, "text/plain", "erased");
  }
  void resetAll(){
    sw1_code=0;
    EEPROM.write(SW1_F, 0);
    EEPROM.commit();
    btn1_code=0;
    EEPROM.write(BTN1_F, 0);
    EEPROM.commit();
    open_code=0;
    EEPROM.write(OPEN_F, 0);
    EEPROM.commit();
    lock_code=0;
    EEPROM.write(LOCK_F, 0);
    EEPROM.commit();
    box_code=0;
    EEPROM.write(BOX_F, 0);
    EEPROM.commit();
    server.send(200, "text/plain", "erased all of them");
  }