#define BLYNK_TEMPLATE_ID    "TMPL6uYK0A8Xa"
#define BLYNK_TEMPLATE_NAME  "Weather Station"
#define BLYNK_AUTH_TOKEN     "U0DAYcdTXOgarW5J5_073X6jIc8_7f9l"
#define BLYNK_PRINT Serial
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
char ssid[] = "Wokwi-GUEST";
char pass[] = "";
BlynkTimer timer;

#include "DHTesp.h"
const int DHT_PIN = 15;
DHTesp dhtSensor;
float suhu, kelembaban;

void myTimerEvent(){

  TempAndHumidity  data = dhtSensor.getTempAndHumidity();
  suhu = data.temperature;
  kelembaban = data.humidity;
  Serial.println("Temp: " + String(suhu, 2) + "°C");
  Serial.println("Humidity: " + String(kelembaban, 1) + "%");

  if (suhu >= 32){
    Serial.println("Suhu Naik");
    Blynk.logEvent("suhu_naik");
  }

  Blynk.virtualWrite(V0, suhu);
  Blynk.virtualWrite(V1, kelembaban);
}

byte led = 5;
BLYNK_WRITE(V2)
{
  int pinValue = param.asInt();
  Serial.print("V2 Switch value is: ");
  Serial.println(pinValue);
  digitalWrite(led, pinValue);
}

void setup(){
  Serial.begin(115200);
  pinMode(led, OUTPUT);
  dhtSensor.setup(DHT_PIN, DHTesp::DHT22);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  timer.setInterval(1000L, myTimerEvent);
}

void loop(){
  Blynk.run();
  timer.run();
}