#ifndef CONFIG_ARDN_H
#define CONFIG_ARDN_H
// %c++
// Configuration pour l'arduino esclave
//=========== Inclure les bibliothèques nécessaires ===========
#include <Arduino.h>
#include <ArduinoJson.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
#include <SoftwareSerial.h>

//============================================================

// Définition des constantes et des variables globales
#define RX_ARDUINO 2  // reçoit du TX_ESP
#define TX_ARDUINO 3  // envoie vers RX_ESP
#define MAX_CODE_LENGTH 6 // Longueur maximale du code PIN

//===========================================================

// ==================Configuration pour l'ESP8266=================
unsigned long previousMillis = 0; // Pour le contrôle du temps
unsigned long Millis = 0; // Pour le contrôle du déverrouillage
const long interval = 5000; // Intervalle de temps pour l'envoi de données
char code[MAX_CODE_LENGTH] = "00000"; // Tableau pour stocker le code PIN
bool codeComplete = false; // Indicateur pour savoir si le code est complet
char verrou[20] = "fermer"; // État de la serrure, initialisé à "fermer"
bool codeSend;
SoftwareSerial espSerial(RX_ARDUINO, TX_ARDUINO); // Communication série avec l'ESP8266

// ==================Keypad configuration=================
const byte ROWS = 4; // Quatre lignes
const byte COLS = 3; // Quatre colonnes
char keys[ROWS][COLS] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};
unsigned long saisiMillis = 0; // Variable pour stocker le temps écoulé depuis la dernière saisie
unsigned long cacheInterval = 500; // Variable pour stocker le temps écoulé depuis le dernier envoi
unsigned long deverouillageMillis = 0; // Variable pour stocker le temps écoulé depuis le dernier déverrouillage
byte rowPins[ROWS] = {6, 7, 8, 9}; // Pins pour les lignes
byte colPins[COLS] = {10, 11, 12}; // Pins pour les colonnes
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS); // Initialisation du clavier

//===========================================================

// ==================LCD configuration=================
LiquidCrystal_I2C lcd(0x27, 16, 2); // Adresse I2C, 16 colonnes, 2 lignes
//===========================================================

// Les fonctions prototypes
void initLCD(); // Fonction pour initialiser l'écran LCD
void saisieClavier(); // Fonction pour gérer la saisie du clavier
bool envoiVersESP(char message[MAX_CODE_LENGTH]); // Fonction pour envoyer un message à l'ESP8266

#endif // CONFIG_ARDN_H