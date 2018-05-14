#include <EEPROM.h>
#include <Hash.h>

const char* SS_ID_IAP   = "internetap";
const char* PASSWD_IAP = "12345678";
const char* SS_ID_WAP   = "ESP-AP";
const char* PASSWD_WAP = "12345678";

void setup() {
  // put your setup code here, to run once:
  
  EEPROM.begin(128);
  clearEEPROM();
  String s = SS_ID_IAP;
  for(int i = 0; i < s.length(); ++i){
    EEPROM.write(i, s[i]);
  }
  s = PASSWD_IAP;
  for(int i = 0; i < s.length(); ++i){
    EEPROM.write(26+i, s[i]);
  }
  s = SS_ID_WAP;
  for(int i = 0; i < s.length(); ++i){
    EEPROM.write(60+i, s[i]);
  }
  s = sha1(PASSWD_WAP);
  for(int i = 0; i < s.length(); ++i){
    EEPROM.write(86+i, s[i]);
  }
  EEPROM.commit();
}

void loop() {
  // put your main code here, to run repeatedly:

}

void clearEEPROM(){
  for (int i = 0; i < 128; ++i) {
    EEPROM.write(i, 0);
  }
  EEPROM.commit();
}

