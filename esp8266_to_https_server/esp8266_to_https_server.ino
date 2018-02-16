#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <EEPROM.h>
#include <ArduinoJson.h>
#include "FS.h"

#define LED_RED 15
#define LED_GREEN 12
#define LED_BLUE 13
#define PUSH_BUTTON 4
#define FOTOR A0

const char* MY_HOST = "inaudio.guide";
const char* MY_URL   = "/risposta.json.php";

const int HTTPS_PORT = 443;
const char* FINGERPRINT = "75 c7 6f d2 f0 1d d8 67 c7 50 d7 13 00 50 3e 4d 0c 4d 7d 92";

ESP8266WebServer server(80);

// nome rete e password per l'access ad internet
String esid = "";
String epass = "";

bool successoAP = false;

void saveEEPROM(bool scelta) {
  // dal 60° byte parte la EEPROM riservata al setting di Witty come AP
  int k = 60;
  if (scelta) {
    // dal byte 0 parte la EEPROM riservata al setting del router internet
    k = 0;
  }
  // pulisco la porzione di EEPROM che voglio riscrivere
  for (int i = k; i < k + 60; ++i) {
    EEPROM.write(i, 0);
  }
  EEPROM.commit();

  String s = server.arg("ssid");
  for (int i = 0; i < s.length(); ++i) {
    EEPROM.write(k + i, s[i]);
  }
  s = server.arg("pwd");
  for (int i = 0; i < s.length(); ++i) {
    EEPROM.write(k + 26 + i, s[i]);
  }
  EEPROM.commit();
}

void saveSettings() {
  String s = "<!DOCTYPE html><HTML><HEAD><script>var destinazione = 'http://192.168.4.1/';var secondi = 6;var conteggio = secondi + 1;function contoallarovescia(){if (conteggio > 1){conteggio = conteggio - 1;document.getElementById('demo').innerHTML = 'Ritorno alla main page fra ' + conteggio + ' secondi';}else{window.location = destinazione;return;}setTimeout(contoallarovescia, 1000);}</script></HEAD><body><h2>Aggiornamento eseguito!</h2><h3 id='demo'></h3><script>contoallarovescia();</script></body></HTML>";
  if (server.hasArg("iap")) {
    // salva sulla EEPROM la SSID e la PASSWORD del internet wifi router
    saveEEPROM(true);
  } else if (server.hasArg("wap")) {
    saveEEPROM(false);
  } else {
    s = "<!DOCTYPE html><html><body><h2>Hello, I'm Witty</h2><ul><li><a href='/config_iap.html'>Informami sulla connessione Internet</a></li><li><a href='/config_wap.html'>Configurami come AP</a></li><li><a href='/restart'>Riavviami</a></li></ul></body></html>";
  }
  server.send(200, "text/html", s);
}

void showForm() {
  String path = server.uri();
  if (SPIFFS.exists(path)) {                            // If the file exists
    File file = SPIFFS.open(path, "r");                 // Open it
    size_t sent = server.streamFile(file, "text/html"); // And send it to the client
    file.close();
  }
}

void ESPrestart() {
  /* riavvia in 6 secondi a patto che PIN di
   *  reset (RST) e GPIO16 connessi
   */
  ESP.deepSleep(6e6);
}

void leggiInternetSettings(){
  for (int i = 0; i < 26; ++i) {
    // trasforma il byte i-esimo in un char da aggiungere alla stringa
    esid += char(EEPROM.read(i));
  }

  for (int i = 26; i < 60; ++i) {
    epass += char(EEPROM.read(i));
  }
  
}

void connectToInternetAP(){
  WiFi.persistent(false);
  WiFi.mode(WIFI_OFF);
  WiFi.mode(WIFI_STA);
  WiFi.begin(esid.c_str(), epass.c_str());
}

bool startAP() {
  WiFi.mode(WIFI_OFF);
  digitalWrite(LED_RED, LOW);
  digitalWrite(LED_BLUE, HIGH);

  WiFi.mode(WIFI_AP);
  String esid2 = "";
  for (int i = 60; i < 86; ++i) {
    // trasforma il byte i-esimo in un char da aggiungere alla stringa
    esid2 += char(EEPROM.read(i));
  }
  String epass2 = "";
  for (int i = 86; i < 120; ++i) {
    epass2 += char(EEPROM.read(i));
  }
  return WiFi.softAP(esid2.c_str(), epass2.c_str());
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  
  EEPROM.begin(128);

  bool ok = SPIFFS.begin();

  pinMode(LED_RED, OUTPUT);
  pinMode(LED_BLUE, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(PUSH_BUTTON, INPUT);

  /* circa 5 secondi per tenere premuto il pulsante
    così entriamo in modalità
  */

  for (int i = 1; i < 11 ; i ++) {
    digitalWrite(LED_RED, LOW);
    delay(250);
    digitalWrite(LED_RED, HIGH);
    delay(250);
  }

  int stato = digitalRead(PUSH_BUTTON);

  // se premuto lo stato sarà basso
  if (stato == LOW) {
    successoAP = startAP();
    if (successoAP) {
      server.on("/", saveSettings);
      // due files sono inutili, le due form differiscono per il bottone e poco altro
      server.on("/config_iap.html", showForm);
      server.on("/config_wap.html", showForm);
      server.on("/restart", ESPrestart);
      server.begin();
      return;
    }
  }

  // se non diventa AP prova a connettersi al wifi internet router
  leggiInternetSettings();

  connectToInternetAP();

}

void loop() {
  // avvio il server http dell'ESP solo se è un AP
  if (successoAP) {
    server.handleClient();
    return;
  }

  // se sono connesso invio al server qualcosa, altrimenti...
  if(WiFi.status() != WL_CONNECTED) {
    digitalWrite(LED_RED, HIGH);
    digitalWrite(LED_GREEN, LOW);
    if(WiFi.status() == WL_NO_SSID_AVAIL){
      connectToInternetAP();
    }
    return;
  }

  StaticJsonBuffer<200> jsonBuffer;
  
  digitalWrite(LED_RED, LOW);
  digitalWrite(LED_GREEN, HIGH);
  
  WiFiClientSecure client;

  if (!client.connect(MY_HOST, HTTPS_PORT)) {
    // RIGHE DI DEBUG ELIMINABILI
    Serial.println();
    Serial.println("Connessione fallita!");
    // FINE
    return;
  }

  if (!client.verify(FINGERPRINT, MY_HOST)) {
    // RIGHE DI DEBUG ELIMINABILI
    Serial.println();
    Serial.println("Certificato non verificato!");
    // FINE
    return;
  }

  int lux = analogRead(FOTOR);

  String url = MY_URL;
  url += "?idiot=";
  // id del device è ESP_secondi24bits_del_MACaddress
  url += WiFi.hostname();
  url += "&value=";
  url += lux;

  client.println(String("GET ") + url +
                 " HTTP/1.1\r\n" +
                 "Host: " + MY_HOST + "\r\n" +
                 "User-Agent: ESP8266Detector\r\n" +
                 "Connection: close\r\n\r\n");

  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 10000) {
      // RIGHE DI DEBUG ELIMINABILI
      Serial.println("Tempo Scaduto!");
      // STOP DEBUG
      client.stop();
      return;
    }
  }

  while (client.connected()) {
    String line = client.readStringUntil('\n');
    if (line == "\r") {
      // RIGHE DI DEBUG ELIMINABILI
      Serial.println();
      Serial.println("headers ricevuti!");
      // STOP DEBUG
      break;
    }
  }

  String line = client.readStringUntil('\n');

  // RIGHE DI DEBUG ELIMINABILI
  Serial.println("==========");
  Serial.println(line);
  Serial.println("==========");
  // STOP DEBUG
  
  client.stop();

  JsonObject& root = jsonBuffer.parseObject(line);

  // Test if parsing succeeds.
  if (!root.success()) {
    Serial.println("parseObject() failed");
    return;
  }

  String sensor = root["id"];
  int valore = root["valore"];
  
  if(lux == valore){
    for (int i = 1; i < 11 ; i ++) {
      digitalWrite(LED_GREEN, LOW);
      delay(150);
      digitalWrite(LED_GREEN, HIGH);
      delay(150);
    }
  }
   
  delay(60 * 1000);

}
