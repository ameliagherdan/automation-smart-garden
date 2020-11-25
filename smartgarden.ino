#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>
#include "DHT.h"

#define DHTPIN 4
#define DHTTYPE DHT11   // DHT 11

const int waterLevelSensor = D1;
const int moistureLevelSensor = A0;
const int relay = D4;

int waterLevel;
int moistureLevel;
unsigned int notified = 0;
void resetNotified() {
  notified = 0;
}
DHT dht(DHTPIN, DHTTYPE);
BlynkTimer timer;

// Go to the Project Settings (nut icon).
char auth[] = "xxxxxxxxxxxxxxxxxxx";
char server[] = "89.39.209.33"; 
int port = 8080;
// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "xxxxxxx";
char pass[] = "xxxxxxxx";


void sendSensor1()
{
  float h = dht.readHumidity();
  float t = dht.readTemperature(); 
    
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  Blynk.virtualWrite(V5, h);
  Blynk.virtualWrite(V6, t);

}

void sendSensor2()
{
  waterLevel = analogRead(waterLevelSensor);
  waterLevel = map(waterLevel,0,2000,0,20);
  moistureLevel = analogRead(moistureLevelSensor);
  moistureLevel = map(moistureLevel,1023, 165, 0, 100);
  
  Blynk.virtualWrite(V7, waterLevel);
  Blynk.virtualWrite(V8, moistureLevel);
  Blynk.virtualWrite(V9, relay);

   if(moistureLevel<45&&notified ==0){
//Blynk API call for EMAIL sending
notified =1;
Blynk.email("ameliagherdan@gmail.com", "Umiditate sol scazuta", "Umiditatea solului a scazut!");
timer.setTimeout(90000L, resetNotified); // 15 min between emails
//Blynk API call for NOTIFICATION pushing
//Blynk.notify("Atentie - Umiditatea solului a scazut!");
}
}
  
 
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass, server, port );
    Blynk.email("ameliagherdan@gmail.com", "Subject", "Smart Garden is running!."); 
  dht.begin();

    // Setup a function to be called every second
  timer.setInterval(2000L, sendSensor1);
  timer.setInterval(5000L, sendSensor2);

}


void loop() {
  // put your main code here, to run repeatedly:
  Blynk.run();
  timer.run();  
 
}
