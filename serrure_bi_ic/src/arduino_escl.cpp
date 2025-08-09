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
  if (espSerial.println(output)!=0) {
    espSerial.flush(); // Assurez-vous que les données sont envoyées
    espSerial.setTimeout(1000); // Définir un délai d'attente pour la réception
    delay(100); // Attendre un peu pour s'assurer que les données sont envoyées
    // Vérifier si des données sont 
    return true; // Envoi réussi
  }
  return false; // Envoi échoué
}


void setup() {
  espSerial.begin(9600); // Initialisation de la communication série
  initLCD();
}

void loop() {
  if (strcmp(verrou, "fermer") == 0) {
    saisieClavier();
    if (codeComplete) {
      envoiVersESP(code);
      codeComplete = false;
      
    }
  }
}