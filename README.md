# intro-IoT
1) lanciare il programma di scrittura sulla EEPROM di valori di default per il nome del WI-FI internet access point (con la relativa password), nonché per il nome della rete generata dall'ESP in modalità AP e la relativa password (8 caratteri per WPA2);
2) lanciare il programma per connettere la board a un server http (o https) per inviargli le misure eseguite sul fotoresistore;

All'accensione il led rosso lampeggia per circa 5 secondi e se al termine il bottone risulterà premuto, il led blu indicherà l'ingresso dell'ESP in modalità AP e al cui WI-FI potremo connetterci per modificare, con il browser (http://192.168.4.1), i succitati valori di default.

Contemporaneamente alla modalità AP, sull'ESP sarà attivato un server http che darà accesso a due risorse html (salvate nel FS mediante il tool di upload accessibile dal menù Strumenti dell'IDE) contenenti le form da riempire con i dati scelti dall'utente. 
