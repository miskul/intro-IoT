# intro-IoT
Dopo aver lanciato il programma di scrittura sulla EEPROM dei valori di default per il nome della rete WI-FI, che permetterà l'accesso ad Internet e la relativa password, nonché per il nome della rete generata dall'ESP in modalità AP e la rispettiva password, si potrà lanciare il programma per connettere la board a un server http (o https) per inviargli le misure eseguite sul fotoresistore.

All'avvio, affinché si possa entrare in modalità AP, il led rosso lampeggia per 5 secondi e se al termine il bottone risulterà premuto, allora l'accensione del led blu indicherà la creazione di una rete WI-FI da parte dell'ESP e, dopo esserci connessi, si potrà procedere con la modifica dei succitati valori di default andando con il browser all'indirizzo http://192.168.4.1
