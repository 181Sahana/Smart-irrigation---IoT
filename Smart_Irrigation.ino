#include <WiFi.h>
#include "DHT.h"
#define BLYNK_PRINT Serial

#define BLYNK_TEMPLATE_ID "TMPL3-SNugFpO"
#define BLYNK_TEMPLATE_NAME "Smart Irrigation"
#define BLYNK_AUTH_TOKEN "_Me5EaYLSdn6jQjIaFSzYVopFpdDUUky"

#include <BlynkSimpleEsp32.h>

const char *ssid =  "12";  
const char *pass =  "123456789";

WiFiClient client;

#define DHT11_PIN 4
#define DHTTYPE DHT11
#define sensor_pin 32
#define RELAY_PIN 14

DHT dht(DHT11_PIN, DHTTYPE);

void setup() {
  Serial.begin(9600); 

  WiFi.begin(ssid, pass);
  while(WiFi.status() != WL_CONNECTED)
  {
    delay(200);
    Serial.print("..");
  }
  Serial.println();
  Serial.println("ESP32 is connected!");
  Serial.println(WiFi.localIP());

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  pinMode(RELAY_PIN, OUTPUT);
  pinMode(sensor_pin, INPUT);

  Serial.println(F("DHTxx test!"));
  dht.begin();
}

void loop() {

  delay(2000);
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  
  if (isnan(h) || isnan(t)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }
  float moisture_Value;
  float moisture_percentage;

  moisture_Value = analogRead(sensor_pin);
  moisture_percentage = map(moisture_Value, 1300, 4095, 100, 0);

  if(moisture_Value>2500)
  digitalWrite(RELAY_PIN, HIGH);
  else
  digitalWrite(RELAY_PIN, LOW);


  Serial.print(F(" Humidity: "));
  Serial.print(h);
  Serial.print(F("%  Temperature: "));
  Serial.print(t);
  Serial.print(F("C "));
  Serial.println("");
  Serial.print("Soil Moisture(in Percentage) = ");
  Serial.print(moisture_percentage);
  Serial.println("%");

  Blynk.virtualWrite(V0, moisture_percentage);
  Blynk.virtualWrite(V1, t);
  Blynk.virtualWrite(V2, h);

  Blynk.run();

  delay(2000);
}
