#include <LiquidCrystal_I2C.h>
#include <RTClib.h>

RTC_DS1307 rtc;

#define BACKLIGHT 7
#define TEST 9
#define ALTERNATE_DAYS 8
// update: alternate_days is triggerable trough pin 8!
#define PUMP1 2
#define PUMP2 3
#define PUMP3 4
#define PUMP4 5
#define LED 13

LiquidCrystal_I2C lcd(0x27,20,4);

DateTime p1Start(0,0,0, 2,00,0); DateTime p1End(0,0,0, 2,15,0);
DateTime p2Start(0,0,0, 2,30,0); DateTime p2End(0,0,0, 3,00,0);
DateTime p3Start(0,0,0, 3,15,0); DateTime p3End(0,0,0, 3,45,0);
DateTime p4Start(0,0,0, 4,00,0); DateTime p4End(0,0,0, 4,30,0);

bool alternate_days = false;
bool work = true;
void setup() {
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("turning on...");

  // set pins
  pinMode(BACKLIGHT, INPUT);
  pinMode(TEST, INPUT);

  pinMode(PUMP1, OUTPUT);
  pinMode(PUMP2, OUTPUT);
  pinMode(PUMP3, OUTPUT);
  pinMode(PUMP4, OUTPUT);
  pinMode(LED, OUTPUT);

  // spegni le pompe!
  digitalWrite(PUMP1, HIGH);
  digitalWrite(PUMP2, HIGH);
  digitalWrite(PUMP3, HIGH);
  digitalWrite(PUMP4, HIGH);
  
  if(!rtc.begin()){
    lcd.setCursor(1,0);
    lcd.print("ERR: no rtc!");
    while(true);
  }

  if(!rtc.isrunning()){
    lcd.setCursor(1,0);
    lcd.print("INF: time set");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }

  if(digitalRead(TEST)){
    lcd.setCursor(0,2);
    lcd.print("INF: test");
    DateTime now = rtc.now();
    
    p1Start = now + TimeSpan(0,0, 0,0); p1End = now + TimeSpan(0,0, 1,0);
    p2Start = now + TimeSpan(0,0, 1,0); p2End = now + TimeSpan(0,0, 2,0);
    p3Start = now + TimeSpan(0,0, 2,0); p3End = now + TimeSpan(0,0, 3,0);
    p4Start = now + TimeSpan(0,0, 3,0); p4End = now + TimeSpan(0,0, 4,0);
  }else if(digitalRead(ALTERNATE_DAYS)){
    lcd.setCursor(0,2);
    lcd.print("INF: alternate days");
    alternate_days=true;
    work=false; // so that on midnight will be set to true and pump will work during the night
  }

  delay(2000);

  lcd.clear();
  lcd.setCursor(0,2);
  lcd.print("1:O  2:O    3:O  4:O");
}

DateTime now; int nm; int nh; int ns; int ny; int nM; int nd; 
bool backlight = true;
bool pump1 = false, pump2 = false, pump3 = false, pump4 = false;

void loop() {
  // get time
  now = rtc.now();
  nm = now.minute(); nh = now.hour(); ns = now.second();
  ny = now.year(); nM = now.month(); nd = now.day();

  lcd.setCursor(0,0);
  lcd.print(String() + format(nh) + ":" + format(nm) + ":" + format(ns) + "  " + 
              format(nd) + "/" + format(nM) + "/" + ny);

  if(digitalRead(BACKLIGHT)){
    backlight = !backlight;
    if(backlight) lcd.backlight();
    else lcd.noBacklight();
    delay(500);
  }

  if(work){
    if(nh==p1Start.hour() && nm==p1Start.minute() && !pump1) turnOn(PUMP1);
    else if(nh==p1End.hour() && nm==p1End.minute() && pump1) turnOff(PUMP1);

    if(nh==p2Start.hour() && nm==p2Start.minute() && !pump2) turnOn(PUMP2);
    else if(nh==p2End.hour() && nm==p2End.minute() && pump2) turnOff(PUMP2);

    if(nh==p3Start.hour() && nm==p3Start.minute() && !pump3) turnOn(PUMP3);
    else if(nh==p3End.hour() && nm==p3End.minute() && pump3) turnOff(PUMP3);

    if(nh==p4Start.hour() && nm==p4Start.minute() && !pump4) turnOn(PUMP4);
    else if(nh==p4End.hour() && nm==p4End.minute() && pump4) turnOff(PUMP4);
  }

  // This might bring some errors!!! if the next update second is still 0, work will change!
  // but thanks to delay(2000) this shouldn't happen.
  if (alternate_days && nm==0 && nh==0 && ns == 0){
    work = !work;
    delay(2000);
  }

  delay(100);
}

void turnOn(int pump){
  digitalWrite(pump, LOW);
  switch(pump){
    case PUMP1:
      lcd.setCursor(2,2);
      lcd.print("I");
      pump1 = true;
      break;
    case PUMP2:
      lcd.setCursor(7,2);
      lcd.print("I");
      pump2 = true;
      break;
    case PUMP3:
      lcd.setCursor(14,2);
      lcd.print("I");
      pump3 = true;
      break;
    case PUMP4:
      lcd.setCursor(19,2);
      lcd.print("I");
      pump4 = true;
      break;
  }
}

void turnOff(int pump){
  digitalWrite(pump, HIGH);
  
  switch(pump){
    case PUMP1:
      lcd.setCursor(2,2);
      lcd.print("O");
      pump1 = false;
      break;
    case PUMP2:
      lcd.setCursor(7,2);
      lcd.print("O");
      pump2 = false;
      break;
    case PUMP3:
      lcd.setCursor(14,2);
      lcd.print("O");
      pump3 = false;
      break;
    case PUMP4:
      lcd.setCursor(19,2);
      lcd.print("O");
      pump4 = false;
      break;
  }
}

String format(int n){
  if (n<10) return String("0") + n;

  return String(n);
}
