//Benötigte Libraries imortieren
#include <Wire.h> //Display
#include <LiquidCrystal_I2C.h>//Display //<LiquidCrystal.h> 
#include <OneWire.h>//Temperatursensor
#include <DallasTemperature.h>//Temperatursensor
#include <ESP8266WiFi.h> //Wifi library für Esp8266
#include <PubSubClient.h> // MQTT Library
//Wlan creds angeben
const char* ssid = "Thermometer";
const char* wifi_password = "Thermometercluster";
// MQTT Einstellungen
const char* mqtt_server = "192.168.101.10";//Broker ip
const char* mqtt_topic = "Temperatur/3";//Topic
const char* mqtt_username = "mqtt";//mqtt_username
const char* mqtt_password = "Thermometercluster";//mqtt_passwort
// Mqtt Client name
const char* clientID = "Termometer_3";
//Wlan initialisieren
WiFiClient wifiClient;
// Mqtt server initialisieren
PubSubClient client(mqtt_server, 1883, wifiClient);
//Port des Temperatursensors übergeben
#define ONE_WIRE_BUS D4
//Library initialisieren
OneWire oneWire(ONE_WIRE_BUS);
//Hardware bus an Library übergeben
DallasTemperature sensors(&oneWire);
// Displayports angeben
LiquidCrystal_I2C lcd(0x3F,20,4);



void setup() {
  //LCD initialisieren
  lcd.begin();
  //LCD hintergrundlich anschalten
  lcd.backlight();
  //Cursor nach links oben setzen
  lcd.setCursor(0,0);
  //Text ausgeben
  lcd.print("Die Temp. ist :");
  // Temperatursensors starten
  sensors.begin();
  // Mit Wlan verbinden
  WiFi.begin(ssid, wifi_password);

  // warten bis verbindung mit wlan steht
  while (WiFi.status() != WL_CONNECTED) {
    char str1[10];
    //Temperatur abfragen
    sensors.requestTemperatures();
    //Cursor auf die 4. Zelle in der zweiten Reihe setzen
    lcd.setCursor(4,1);
    // Temperatur in variable speichern
    double x = (sensors.getTempCByIndex(0));
    //die Variable mit der Temperatur als Zahl in eine Variable mit der Temperatur als Text, mit nur einer Nachkommastelle, umwandeln
    dtostrf( x, 5, 1, str1);
    //Die Temperatur auf dem Display ausgeben
    lcd.print(str1);

  }
  // Mit Broker verbinden
  client.connect(clientID, mqtt_username, mqtt_password);


}

void loop() {
  //Temperatur abfragen
  sensors.requestTemperatures();
  //Cursor auf die 4. Zelle in der zweiten Reihe setzen
  lcd.setCursor(4,1);
  // Temperatur in variable speichern
  double x = (sensors.getTempCByIndex(0));
  //zwei char arrays definieren
  char str[10];
  char str1[10];
  //die Variable mit der Temperatur als Zahl in eine Variable mit der Temperatur als Text umwandeln
  dtostrf( x, 5, 2, str);
  //die Variable mit der Temperatur als Zahl in eine Variable mit der Temperatur als Text, mit nur einer Nachkommastelle, umwandeln
  dtostrf( x, 5, 1, str1);
  //Die Temperatur auf dem Display ausgeben
  lcd.print(str1);

  // die Temperatur veröffentlichen
  client.publish(mqtt_topic, str);

  //Ab hier wird der void loop() widerholt

}
