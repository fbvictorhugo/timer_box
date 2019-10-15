#include <MsTimer2.h>
#include <LiquidCrystal_I2C.h>

const int BTN = 8;
const int LED =  4;
const int BUZZ =  6;
const int POTENTIOMETER = A0;

volatile char seconds, minutes;

volatile boolean timerChange = true;
volatile boolean timerOn = false;

int timeLimit = 5;

LiquidCrystal_I2C lcd(0x3F, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

void setup() {

  Serial.begin(9600);

  lcd.begin(16, 2);
  MsTimer2::set(1000, onTimerTick);

  pinMode(LED, OUTPUT);
  pinMode(BUZZ, OUTPUT);
  pinMode(BTN, INPUT);


  writeLcd("Cronomometro", formatTime(timeLimit, 0));
  //ledOn();

}

void loop() {

  int btnState = digitalRead(BTN);

  if (btnState == LOW && !timerOn) {
    timerOn = true;
    ledOff();
    MsTimer2::start();

  } 

  if (timerOn) {
    if (timerChange) {

      writeLcd("TEMPO", formatTime(minutes, seconds));
      timerChange = false;

      if (minutes == timeLimit) {
        onTimerFinish();
      }
    }
  }

}

void onTimerTick() {
  seconds++;
  if (seconds == 60) {
    seconds = 0;
    minutes ++;
  }
  timerChange = true;
}

void onTimerFinish() {
  timerOn = false;
  playBuzz();
  writeLcd("Acabou! :/", "Reinicie");
  ledOn();
  delay(500);
  stopBuzz();

  MsTimer2::stop();
  seconds = 0;
  minutes = 0;
}

void writeLcd(String primary, String second) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(primary);
  lcd.setCursor(0, 1);
  lcd.print(second);
}

int getPotentiometerValue() {
  int valPotentiometer =  analogRead(POTENTIOMETER);
  return map(valPotentiometer, 0, 1023, 0, 255);
}

String toStringTime(int value) {
  if (value < 10) {
    return "0" + String(value, DEC);
  } else {
    return String(value, DEC);
  }
}

String formatTime(int minutes, int seconds) {
  return toStringTime(minutes) + ":" +  toStringTime(seconds);
}

void ledOn() {
  digitalWrite(LED, HIGH);
}

void ledOff() {
  digitalWrite(LED, LOW);
}

void playBuzz() {
  digitalWrite(BUZZ, HIGH);
}

void stopBuzz() {
  digitalWrite(BUZZ, LOW);
}

int getTimeFromPotentiometer(int value) {
  if (value <= 50) {
    return 5;
  } else if (value > 50 && value <= 100) {
    return 10;
  } else if (value > 100 && value <= 150) {
    return 15;
  } else if (value > 150 && value <= 200) {
    return 20;
  } else if (value > 200) {
    return 25;
  }

}


