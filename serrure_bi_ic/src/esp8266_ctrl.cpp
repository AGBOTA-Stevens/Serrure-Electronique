#include "config_esp.h"

bool writeBlock(byte blockAddr, byte *data) {
  MFRC522::StatusCode status;
  status = rfid.MIFARE_Write(blockAddr, data, 50);

  return (status == MFRC522::STATUS_OK);
}

// Transforme une chaîne en tableau de bytes
size_t toBytes(const char* str, byte* buffer, size_t bufsize) {
    size_t len = strlen(str);

    // Sécurité : ne pas dépasser la taille du buffer
    if (len > bufsize) {
        len = bufsize;
    }

    for (size_t i = 0; i < len; i++) {
        buffer[i] = (byte)str[i];
    }

    return len;  // renvoie le nombre de bytes copiés
}

bool readBlock(byte blockAddr) {
  byte buffer[50];
  byte size = sizeof(buffer);
  
  MFRC522::StatusCode status = rfid.MIFARE_Read(blockAddr, buffer, &size);
  if (status == MFRC522::STATUS_OK) {
    for (byte i = 0; i < 50; i++) {
      badge[i] = buffer[i];
    }
    return true;
  }
  return false;
}

// Fonction pour vérifier si un UID est autorisé
bool isUserAuthorized(const String& uid) {
  for (auto& user : utilisateurs) {
    if (user.uid == uid) return true;
  }
  return false;
}

bool postBadge() {
  if (WiFi.status() != WL_CONNECTED) return false;

  HTTPClient http;
  String url = "http://192.168.100.7:8000/api/verifier_badge_pass/"; //http://192.168.100.7:8000/api/verifier_badge_pass/
  http.begin(client, url); // pour HTTPS: http.begin(client, url)
  http.addHeader("Content-Type", "application/json");
  Serial.println("URL: " + url);
  
  if (!readBlock(1))
    return false; // Lire le bloc 1 où le badge est stocké
  // construire JSON
  JsonDocument doc;
  doc["badge_pass"] = badge;
  String payload;
  serializeJson(doc, payload);

  int httpCode = http.POST(payload);
  if (httpCode > 0) {
    const String resp_body = http.getString();
    http.end();
    JsonDocument d;
    DeserializationError err = deserializeJson(d, resp_body);
    if (err) {
      return false;
    } else if (d["status"]) {
      const char* badgeStr = d["badge_pass"];
      
      int len = strlen(badgeStr);
      byte badgeBytes[len];   // buffer
      toBytes(badgeStr, badgeBytes, sizeof(badgeBytes));
      return writeBlock(1, badgeBytes);
    }else {
      return false;
    }
  } else {
    http.end();
    return false;
  }
}

bool activerBadge() {
  if (WiFi.status() != WL_CONNECTED) return false;

  HTTPClient http;
  String url = String(serverName) + "/api/activer_badge_pass/"; //http://192.168.100.7:8000/api/activer_badge_pass/
  http.begin(client, url); // pour HTTPS: http.begin(client, url)
  http.addHeader("Content-Type", "application/json");
  if (!readBlock(1))
    return false; // Lire le bloc 1 où le badge est stocké
  JsonDocument doc;
  doc["code"] = code_recu;
  String payload;
  serializeJson(doc, payload);

  int httpCode = http.POST(payload);
  if (httpCode > 0) {
    const String resp_body = http.getString();
    http.end();
    JsonDocument d;
    DeserializationError err = deserializeJson(d, resp_body);
    if (err) {
      return false;
    } else if (d["status"]) {
      const char* badgeStr = d["badge_pass"];
      int len = strlen(badgeStr);
      byte badgeBytes[len];   // buffer
      toBytes(badgeStr, badgeBytes, sizeof(badgeBytes));
      return writeBlock(1, badgeBytes);
    }else {
      return false;
    }
  } else {
    http.end();
    return false;
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
      Serial.println("Erreur de désérialisation JSON");
      //tone(BUZZER, 4000, 1000); // Buzzer pour indiquer l'erreur
      return false;
    }
    const char* code = doc["code"];
    int len = strlen(code);
    for (int i = 0; i<len; i++) {
      code_recu[i] = code[i];
    }
    Serial.println("Code reçu: " + String(code_recu));
    if (!code_recu) {
      Serial.println("Code non trouvé dans le JSON");
      //tone(BUZZER, 4000, 1000); // Buzzer pour indiquer l'erreur
      return false;
    }
    return true;
  }
  Serial.println("Aucun code reçu");
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
  initWifi(); // Initialise la connexion Wi-Fi
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
    activerBadge();
  } else {
    //Serial.println("Code incorrect ou incomplet.");
    repondreVersArduino("Non"); // Envoie la réponse vers l'Arduino
    delay(2000); // Attendre avant de vérifier à nouveau
  }
  if (postBadge()) {
    //Serial.println("Badge mis à jour avec succès.");
    repondreVersArduino("Oui"); // Envoie la réponse vers l'Arduino
    tone(BUZZER, 2000, 500); // Buzzer pour indiquer le succès
    digitalWrite(RELAIS_PIN, HIGH); // Active le relais pour ouvrir la porte
    delay(5000); // Garde la porte ouverte pendant 5 secondes
    digitalWrite(RELAIS_PIN, LOW); // Désactive le relais pour fermer la porte
  } else {
    //Serial.println("Échec de la mise à jour du badge.");
    repondreVersArduino("Non"); // Envoie la réponse vers l'Arduino
    tone(BUZZER, 4000, 1000); // Buzzer pour indiquer l'erreur
    delay(2000); // Attendre avant de vérifier à nouveau
  }
}