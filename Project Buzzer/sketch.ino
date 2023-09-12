#include <WiFi.h>
#include <HTTPClient.h>
const char* ssid = "Wokwi-GUEST";
const char* password = "";
const char* SERVER = "api.thingspeak.com";
const int PORT = 80;
const String APIKEY = "IQBJ81KT5DG6KWPE";

#define PIN_TRIG 13
#define PIN_ECHO 14
int tinggiMaksimal = 500;
int tinggi, jarak = 0;
byte pinBuzzer = 5;

unsigned const long interval = 10000;
unsigned long zero = 0;

void setup() {
  Serial.begin(115200);
  pinMode(PIN_TRIG, OUTPUT);
  pinMode(PIN_ECHO, INPUT);

  // Menghubungkan ke jaringan WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
}

void loop() {

  digitalWrite(PIN_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(PIN_TRIG, LOW);
  int duration = pulseIn(PIN_ECHO, HIGH);
  jarak = duration / 58;
  tinggi = tinggiMaksimal - jarak;
  Serial.println(String() + "Tinggi: " + tinggi + " cm");

  int jeda = 1000;
  if(tinggi >= 300){
    jeda = jeda - tinggi;
    Serial.println(jeda);
    tone(pinBuzzer, 262, jeda);
    noTone(pinBuzzer);
    tone(pinBuzzer, 349, jeda);
    noTone(pinBuzzer);
  }

  if(millis() - zero >= interval){

if (WiFi.status() == WL_CONNECTED) {
    Serial.println();
    Serial.println("Menghubungkan dengan server...");
    HTTPClient http;
    String url = "http://" + String(SERVER) + "/update";
    String konten = String() + "field1=" + tinggi;
    http.begin(url);
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    http.addHeader("X-THINGSPEAKAPIKEY", APIKEY);
    int httpCode = http.POST(konten);

    if (httpCode > 0) {
      if (httpCode == HTTP_CODE_OK) {
        String response = http.getString();
        Serial.println("Terhubung dengan server.");
        Serial.print("Respon dari server: ");
        Serial.println(response);
      } else {
        Serial.printf("Request gagal dengan kode status HTTP: %d\n", httpCode);
      }
    } else {
      Serial.println("Koneksi gagal.");
    }

    http.end();

  } else {
    Serial.println("WiFi disconnected");
  }

    zero = millis();
  }

  

}