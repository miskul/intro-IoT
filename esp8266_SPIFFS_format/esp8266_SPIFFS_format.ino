#include "FS.h"

boolean fs_format = false;
void setup() {
  delay(500);
  // put your setup code here, to run once:
  Serial.begin(9600);
  fs_format = SPIFFS.format();
  if(fs_format == true){
    Serial.print("Formattato!");
    Serial.println();
  }
  bool ok = SPIFFS.begin();
  if(ok){
    Serial.print("FS al lavoro");
  }
  SPIFFS.end();
}

void loop() {
  // put your main code here, to run repeatedly:

}
