#include <LiquidCrystal_I2C.h>
#include <RTClib.h>

RTC_DS1307 rtc;

#define BACKLIGHT 7
#define TEST 8
#define PUMP1 2
#define PUMP2 3
#define PUMP3 4
#define PUMP4 5
#define LED 13

LiquidCrystal_I2C lcd(0x27,20,4);

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

  delay(1000);
}

DateTime now;
void loop() {
  // get time
  now = rtc.now();

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(String() + format(now.hour()) + ":" + format(now.minute()) + "     " + 
              format(now.day()) + "/" + format(now.month()) + "/" + now.year());

  // #define BACKLIGHT 7
  // #define TEST 8
  // lcd.setCursor(0,1);
  // lcd.print(String("BKL: ") + digitalRead(BACKLIGHT) + " | TS: " + digitalRead(TEST));

  lcd.setCursor(0,2);
  lcd.print("1:X  2:X    3:X  4:X")

  delay(500);
}

String format(int n){
  if (n<10) return String("0") + n;

  return String(n);
}
