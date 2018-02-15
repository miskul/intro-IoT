# intro-IoT
1) caricare il firmware di scrittura sulla EEPROM di valori di default per il nome del WI-FI internet access point (con la relativa password), nonché per il nome della rete generata dall'ESP in modalità AP e la relativa password (8 caratteri per WPA2);
2) caricare il firmware di formattazione del FS (1MB SPIFFS);
3) caricare (mediante il tool di upload accessibile dal menù Strumenti dell'IDE) nel FS eventuali risorse presenti nella directory data del progetto;
4) caricare il firmware di connessione della board a un server https per inviargli le misure eseguite sul fotoresistore;

All'accensione, il led rosso lampeggerà per circa 5 secondi e se al termine il bottone risulterà premuto, il led blu indicherà l'ingresso dell'ESP in modalità AP e al cui WI-FI potremo connetterci, per modificare con il browser (http://192.168.4.1) i succitati valori di default.

Contemporaneamente alla modalità AP, sull'ESP sarà attivato un server http che darà accesso alle due risorse html (salvate nel FS) e contenenti le form da riempire con i dati scelti dall'utente. 

Per approfondimenti, sulla programmazione dell'ESP8266 mediante Arduino IDE, si può fare riferimento alla repository del progetto che è  reperibile all'indirizzo:

https://github.com/esp8266/Arduino

