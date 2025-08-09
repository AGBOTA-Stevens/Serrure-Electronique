#include "config_ardn.h"


void initLCD() {
  lcd.begin(16, 2);
  lcd.backlight();
  lcd.clear();
  lcd.scrollDisplayRight();
  lcd.setCursor(0, 0);
}

void saisieClavier() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Entrez le code:");
  lcd.setCursor(0, 1);
  uint8_t index = 0;
  memset(code, 0, MAX_CODE_LENGTH); // Réinitialiser le code
  while (index <= MAX_CODE_LENGTH - 2) { // -2 pour laisser de la place pour le caractère de fin
    char key = keypad.getKey();
    if (key) {
      if (key == '#') {
        codeComplete = false; // Fin de la saisie
        code[index] = '\0'; // Terminer la chaîne
        break;
      } else if (key == '*') {
        if (index > 0) {
          index--; // Supprimer le dernier caractère
          lcd.setCursor(index, 1);
          lcd.print(' '); // Effacer le dernier caractère affiché
          lcd.setCursor(index, 1);
        }
        continue; // Ignorer l'astérisque
      } else {
        code[index] = key;
        lcd.print(key);
        delay(200); // Anti-rebond
        lcd.setCursor(index, 1);
        lcd.print('*');
        index++;
      }
      delayMicroseconds(200); // Anti-rebond
    }
  }
  if (index == MAX_CODE_LENGTH-1) {
    code[index] = '\0'; // Terminer la chaîne
    codeComplete = true;
  }
}

bool envoiVersESP(char message[MAX_CODE_LENGTH]) {
  JsonDocument doc;
  doc["code"] = message;
  String output;
  serializeJson(doc, output);
  if (espSerial.print(output)!=0) {
    espSerial.flush(); // Assurez-vous que les données sont envoyées
    espSerial.setTimeout(1000); // Définir un délai d'attente pour la réception
    delay(100); // Attendre un peu pour s'assurer que les données sont envoyées
    // Vérifier si des données sont 
    return true; // Envoi réussi
  }
  return false; // Envoi échoué
}

void receptionDepuisESP() {
  if (espSerial.available()) {
    String input = espSerial.readStringUntil('\n'); // Lire jusqu'à la fin de ligne
    JsonDocument doc;
    DeserializationError error = deserializeJson(doc, input);
    if (error) {
      //Serial.println("Erreur de désérialisation JSON");
      return; // Erreur de désérialisation, ne pas continuer  
    }
    const bool isOpen = doc["isOpen"];
    if (isOpen) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Serrure ouverte");
      // Ici, vous pouvez ajouter le code pour ouvrir la serrure
      delay(2000); // Afficher le message pendant 2 secondes
    } else {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Code incorrect");
      delay(2000); // Afficher le message pendant 2 secondes  
    }
  }
}

void setup() {
  espSerial.begin(9600); // Initialisation de la communication série
  initLCD();
}

void loop() {
  if (strcmp(verrou, "fermer") == 0) {
    saisieClavier();
    if (codeComplete) {
      if (envoiVersESP(code)) 
        receptionDepuisESP();
      codeComplete = false; 
      delay(1000); // Attendre un peu avant de réinitialiser l'affichage
    } else {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Code incomplet");
      delay(2000); // Afficher le message pendant 2 secondes
    }
  }
}