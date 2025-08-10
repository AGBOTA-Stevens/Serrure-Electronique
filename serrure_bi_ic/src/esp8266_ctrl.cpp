#include "config_esp.h"




// Fonction pour vérifier si un UID est autorisé
bool isUserAuthorized(const String& uid) {
  for (auto& user : utilisateurs) {
    if (user.uid == uid) return true;
  }
  return false;
}

// Fonction pour envoyer un email via une API web (exemple générique)
/*void notifierNouveauCode(const String& nouveauCode) {
  HTTPClient http;
  for (auto& user : utilisateurs) {
    String url = "https://votre-api-mail.com/send?to=" + user.email + "&msg=Nouveau code: " + nouveauCode;
    http.begin(url);
    int httpCode = http.GET();
    if (httpCode > 0) {
      Serial.println("Notification envoyée à " + user.email);
    } else {
      Serial.println("Erreur notification " + user.email);
    }
    http.end();
    delay(500); // Pour éviter de surcharger l'API
  }
}
*/

// Initialise l'EEPROM avec le code par défaut si vide
void initCodeEEPROM() {
  EEPROM.begin(EEPROM_SIZE);
  char code_lu[MAX_CODE_LENGTH];
  lireCodeEEPROM(code_lu);
  if (code_lu[0] == '\0' || code_lu[0] == 0xFF) { // EEPROM vide
    enregistrerMotDePasse(code_par_defaut);
  }
}

void initWifi() {
  // Initialisation de la connexion Wi-Fi
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password); // Remplacez par vos identifiants Wi-Fi
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connexion au Wi-Fi...");
  }
  Serial.println("Connecté au Wi-Fi");
}

// Lit le code PIN depuis l'EEPROM
void lireCodeEEPROM(char* code_lu) {
  for (int i = 0; i < MAX_CODE_LENGTH; i++) {
    code_lu[i] = EEPROM.read(addr + i);
  }
  code_lu[MAX_CODE_LENGTH - 1] = '\0';
}

void enregistrerMotDePasse(const char* mdp) {
  for (int i = 0; i < MAX_CODE_LENGTH; i++) {
    EEPROM.write(addr + i, mdp[i]);
  }
  EEPROM.commit();
}

// Compare le code reçu avec celui stocké
bool verifierCode() {
  if (Serial.available()) {
    String input = Serial.readStringUntil('\n');
    JsonDocument doc;
    DeserializationError error = deserializeJson(doc, input);
    if (error) {
      //Serial.println("Erreur de désérialisation JSON");
      //tone(BUZZER, 4000, 1000); // Buzzer pour indiquer l'erreur
      return false;
    }
    const char* code_recu = doc["code"];
    //Serial.println("Code reçu: " + String(code_recu));
    if (!code_recu) {
      //Serial.println("Code non trouvé dans le JSON");
      //tone(BUZZER, 4000, 1000); // Buzzer pour indiquer l'erreur
      return false;
    }
    char code_lu[MAX_CODE_LENGTH];
    lireCodeEEPROM(code_lu);
    if ((strncmp(code_lu, code_recu, MAX_CODE_LENGTH) == 0)) {
      //Serial.println("Code lu depuis l'EEPROM: " + String(code_lu));
      //tone(BUZZER, 1000, 500); // Buzzer pour indiquer le succès
      return true; // Code correct
    }
    return false; // Code incorrect
  }
  //Serial.println("Aucune donnée disponible sur l'ESP8266");
  //tone(BUZZER, 4000, 1000); // Buzzer pour indiquer l'échec
  return false;
}

void repondreVersArduino(bool codeOK) {
  JsonDocument doc;
  doc["isOpen"] = codeOK;
  String response;
  serializeJson(doc, response);
  Serial.print(response); // Envoie la réponse vers l'Arduino
  Serial.flush();
}

void repondreVersArduino(String msg) {
  Serial.print(msg); // Envoie le message vers l'Arduino
  Serial.flush();
}

void setup() {
  Serial.begin(9600); // UART vers l'ESP8266
  while (!Serial) {
    ; // Attendre que l'Arduino soit prêt
  }
  initCodeEEPROM(); // Initialise l'EEPROM avec le code par défaut si vide
  //initWifi(); // Initialise la connexion Wi-Fi
  SPI.begin(); // Initialise le bus SPI
  rfid.PCD_Init(); // Initialise le module MFRC522
  delay(4); // Petit délai pour la stabilité
  pinMode(BUZZER, OUTPUT);
  pinMode(RELAIS_PIN, OUTPUT); // Pin pour le relais
  tone(BUZZER, 1000, 500); // Buzzer pour indiquer que le système est prêt
  //Serial.println("Système prêt. En attente de code...");
  // Attendez que l'Arduino envoie le code PIN
}

void loop() {
  if (verifierCode()) {
    //Serial.println("Code correct, déverrouillage...");
    repondreVersArduino(true); // Envoie la réponse vers l'Arduino
    digitalWrite(RELAIS_PIN, HIGH); // Ouvre le relais
    tone(BUZZER, 1000, 5000); // Buzzer pour indiquer le succès
    delay(5000); // Buzzer actif pendant 1 seconde
    digitalWrite(RELAIS_PIN, LOW); // Referme le relais
  } else {
    //Serial.println("Code incorrect ou incomplet.");
    repondreVersArduino(false); // Envoie la réponse vers l'Arduino
    delay(2000); // Attendre avant de vérifier à nouveau
  }
}