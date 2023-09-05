//RTC -------------------------------------->
#include <Wire.h>
#include <RTClib.h>
RTC_DS1307 rtc;

//LCD -------------------------------------->
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd (0x27, 16, 2);

//SERVO -------------------------------------->
#include <ESP32Servo.h>
const int servoPin = 5;
Servo mekanik;

unsigned const long interval = 1000;
unsigned long zero = 0;

void setup(){
  Serial.begin(115200);

  //LCD -------------------------------------->
  lcd.init();
  lcd.backlight();
  lcd.clear();

  //RTC -------------------------------------->
  Wire.begin();
  if(!rtc.begin()){
    Serial.println("Sambungan RTC Bermasalah");
    lcd.setCursor(0,0);
    lcd.print("RTC Bermasalah!");
    while(1);
  }
  lcd.clear();

  mekanik.attach(servoPin);
  mekanik.write(0);

}

void loop(){

  //RTC -------------------------------------->
  DateTime now = rtc.now();
  int tahun = now.year();
  int bulan = now.month();
  int tanggal = now.day();
  int jam = now.hour();
  int menit = now.minute();
  int detik = now.second();

  //JADWAL PAGI ------------------------------------------->
  if( (jam == 7) && (menit == 30) && (detik == 1) ){
    kasihpakan();
  }

  //JADWAL SIANG ------------------------------------------->
  if( (jam == 13) && (menit == 15) && (detik == 1) ){
    kasihpakan();
  }

  //JADWAL SIANG ------------------------------------------->
  if( (jam == 17) && (menit == 30) && (detik == 1) ){
    kasihpakan();
  }

  if(millis() - zero >= interval){

    Serial.println(String() + "Tanggal: " + tanggal + "-" + bulan + "-" + tahun);
    Serial.println(String() + "Jam: " + jam + ":" + menit + ":" + detik);
    Serial.println();

    lcd.setCursor(0,0);
    lcd.print(String() + "Tgl: " + tanggal + "-" + bulan + "-" + tahun + "   ");
    lcd.setCursor(0,1);
    lcd.print(String() + "Jam: " + jam + ":" + menit + ":" + detik + "   ");

    zero = millis();
  }

}

void kasihpakan(){
  mekanik.write(180);
  lcd.clear();
  lcd.print("FEEDING TIME...");
  delay(3000);
  mekanik.write(0);
  lcd.clear();
}