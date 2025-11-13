#ifndef CONFIG_ESP_H
#define CONFIG_ESP_H
// %c++
// Configuration pour l'ESP8266 maître
//=========== Inclure les bibliothèques nécessaires ===========

#include <Arduino.h>
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <ESPAsyncWebServer.h>
#include <ESP8266HTTPClient.h>
#include <EEPROM.h>
#include <Servo.h>
//#include <SoftwareSerial.h>
#include <MFRC522.h>
#include <SPI.h>
#include <vector>
#include <ESP8266HTTPClient.h>
// =================== Structure utilisateur ===================
// Structure utilisateur
struct Utilisateur {
  String uid;
  String email;
  String mdp_perso;
};

// Exemple de base d'utilisateurs (à remplacer par stockage persistant ou serveur)
std::vector<Utilisateur> utilisateurs = {
  {"A1B2C3D4", "alice@email.com", "azerty"},
  {"B2C3D4E5", "bob@email.com", "123456"}
};

//============================================================
// Définition des constantes et des variables globales

#define MAX_CODE_LENGTH 6 // Longueur maximale du code PIN
#define EEPROM_SIZE 512 // Taille de l'EEPROM
#define addr 0 // Adresse de début de l'EEPROM
#define BUZZER D3 // Pin pour le buzzer
#define RELAIS_PIN D8 // Pin pour le servo moteur
//#define RX_ESP D7 // Pin pour la réception de l'ESP8266
//#define TX_ESP D8 // Pin pour l'envoi vers l'ESP8266

//===========================================================
// Fonctions pour gestion du code PIN
bool verifierCode(); // Compare le code reçu avec celui stocké
void enregistrerMotDePasse(const char* mdp); // Enregistre le mot de passe dans l'EEPROM
void initWifi(); // Initialise la connexion Wi-Fi
bool isUserAuthorized(const String& uid); // Vérifie si l'utilisateur est autorisé
void repondreVersArduino(bool codeOK); // Envoie la réponse vers l'Arduino
void repondreVersArduino(String msg);
bool writeBlock(byte blockAddr, byte *data);
bool readBlock(byte blockAddr);
bool postBadge();
bool activerBadge();
size_t toBytes(const char* str, byte* buffer, size_t bufsize);

// ==================Configuration pour l'ESP8266=================
unsigned long previousMillis = 0; // Pour le contrôle du temps
const long interval = 1000; // Intervalle de temps pour l'envoi de données
String msg = ""; // Message à envoyer à l'Arduino

char badge[50];
//SoftwareSerial ardnSerial(RX_ESP, TX_ESP); // Communication série avec l'ESP8266
const char* ssid = "HUAWEI-2.4G-KdbN"; // Nom du réseau Wi-Fi
const char* password = "V7Uhv9gX"; // Mot de passe du réseau Wi-Fi
const char* serverName = "http://192.168.100.7:8000";
char* code_recu;
WiFiClient client;

//===========================================================


// ==================Configuration pour l'arduino===============


// ==================RFID configuration=================
#define SS_PIN D2 // Pin pour le Slave Select du RFID
#define RST_PIN D1 // Pin pour le Reset du RFID
MFRC522 rfid(SS_PIN, RST_PIN); // Initialisation du lecteur RFID
/*  
   SCK	SPI Clock	D5 (GPIO14)
  MOSI	SPI MOSI	D7 (GPIO13)
  MISO	SPI MISO	D6 (GPIO12)
  IRQ	Interruption	Non utilisé (laisser libre)
  GND	Masse	GND
  RST	Reset	D1 (GPIO5)
*/



#endif // CONFIG_ESP_H