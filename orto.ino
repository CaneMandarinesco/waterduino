// features
// uusare un giorno si e un giorno no
// orario

#DEFINE BACKLIGHT A0
#DEFINE TEST A1
#DEFINE PUMP1 D2
#DEFINE PUMP2 D3
#DEFINE PUMP3 D4
#DEFINE PUMP4 D5


void setup() {
  // pinMode(13, OUTPUT);
  
  pinMode(BACKLIGHT, INPUT);
  pinMode(TEST, INPUT);

  pinMode(PUMP1, OUTPUT);
  pinMode(PUMP2, OUTPUT);
  pinMode(PUMP3, OUTPUT);
  pinMode(PUMP4, OUTPUT);

  // spegni gli interruttori
}

void loop() {
  digitalWrite(13, HIGH);
  delay(500);
  digitalWrite(13, LOW);
  delay(500);
}
