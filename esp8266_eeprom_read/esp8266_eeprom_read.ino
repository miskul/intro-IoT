#include <EEPROM.h>

void setup() {
  Serial.begin(9600);
  EEPROM.begin(128);

  String esid = "";
  for (int i = 0; i < 26; ++i) {
    esid += char(EEPROM.read(i));
  }
  Serial.println();
  Serial.print("SSID-INTERNET:");
  Serial.print(esid);
  
  esid = "";
  for (int i = 26; i < 60; ++i) {
    esid += char(EEPROM.read(i));
  }
  Serial.println();
  Serial.print("PASSWORD:");
  Serial.print(esid);
  
  esid = "";
  for (int i = 60; i < 86; ++i) {
    // trasforma il byte i-esimo in un char da aggiungere alla stringa
    esid += char(EEPROM.read(i));
  }
  Serial.println();
  Serial.print("SSID-AP:");
  Serial.print(esid);

  esid = "";
  for (int i = 86; i < 120; ++i) {
    esid += char(EEPROM.read(i));
  }
  Serial.println();
  Serial.print("PASSWORD:");
  Serial.print(esid);
}


void loop() {
  // put your main code here, to run repeatedly:

}
