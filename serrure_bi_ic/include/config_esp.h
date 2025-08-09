#ifndef CONFIG_ESP_H
#define CONFIG_ESP_H
// %c++
// Configuration pour l'ESP8266 maître
//=========== Inclure les bibliothèques nécessaires ===========

#include <Arduino.h>
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <ESPAsyncWebServer.h>
#include <EEPROM.h>
#include <Servo.h>
#include <SoftwareSerial.h>
#include <MFRC522.h>
#include <SPI.h>
#include <vector>
#include <ESP8266HTTPClient.h>

//============================================================
// Définition des constantes et des variables globales

#define MAX_CODE_LENGTH 6 // Longueur maximale du code PIN
#define EEPROM_SIZE 512 // Taille de l'EEPROM
#define addr 0 // Adresse de début de l'EEPROM
#define BUZZER D5 // Pin pour le buzzer
#define RELAIS_PIN D6 // Pin pour le servo moteur
#define RX_ESP D7 // Pin pour la réception de l'ESP8266
#define TX_ESP D8 // Pin pour l'envoi vers l'ESP8266

//===========================================================
// Fonctions pour gestion du code PIN
void initCodeEEPROM(); // Initialise l'EEPROM avec le code par défaut si vide
void lireCodeEEPROM(char* code_lu); // Lit le code PIN depuis l'EEPROM
bool verifierCode(); // Compare le code reçu avec celui stocké
void enregistrerMotDePasse(const char* mdp); // Enregistre le mot de passe dans l'EEPROM
void initWifi(); // Initialise la connexion Wi-Fi

// ==================Configuration pour l'ESP8266=================
unsigned long previousMillis = 0; // Pour le contrôle du temps
const long interval = 1000; // Intervalle de temps pour l'envoi de données
String msg = ""; // Message à envoyer à l'Arduino
const char code_par_defaut[MAX_CODE_LENGTH] = "12345";
SoftwareSerial ardnSerial(RX_ESP, TX_ESP); // Communication série avec l'ESP8266
const char* ssid = "youpilab_fibre"; // Nom du réseau Wi-Fi
const char* password = "i_l@v3_yl2021Fibre"; // Mot de passe du réseau Wi-Fi
AsyncWebServer server(80); // Serveur web asynchrone sur le port
//===========================================================


// ==================Configuration pour l'arduino===============


// ==================RFID configuration=================
#define SS_PIN D1 // Pin pour le Slave Select du RFID
#define RST_PIN D2 // Pin pour le Reset du RFID
MFRC522 rfid(SS_PIN, RST_PIN); // Initialisation du lecteur RFID



#endif // CONFIG_ESP_H