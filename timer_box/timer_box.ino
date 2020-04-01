#include <MsTimer2.h>
#include <LiquidCrystal_I2C.h>

const int BTN = 10;
const int LED =  6;
const int BUZZ =  2;
const int POTENTIOMETER = A2;

volatile char seconds, minutes;

volatile boolean timerChange = true;
volatile boolean timerOn = false;

int timeLimit = 1;

bool control = true;

LiquidCrystal_I2C lcd(0x3F, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

void setup() {

  Serial.begin(9600);

  lcd.begin(16, 2);
  MsTimer2::set(1000, onTimerTick);

  pinMode(LED, OUTPUT);
  pinMode(BUZZ, OUTPUT);
  pinMode(BTN, INPUT);

  writeLcd("Cronomometro", formatTime(timeLimit, 0));
  ledOn();

  minutes = timeLimit;

}

void loop() {

  int btnState = digitalRead(BTN);

  Serial.println(String(btnState , DEC));

  if (btnState == LOW && !timerOn) {
    playBuzzOnStart();
    timerOn = true;
    ledOff();
    MsTimer2::start();

  } else if (!timerOn) {
    timeLimit = getTimeFromPotentiometer(getPotentiometerValue());
    minutes = timeLimit;
    writeLcdSecond(formatTime(timeLimit, 0));
  }

  if (timerOn) {
    if (timerChange) {

      writeLcd("TEMPO", formatTime(minutes, seconds));
      timerChange = false;

      if (minutes == 0 && seconds == 0 ) {
        onTimerFinish();
      }
    }
  }

}

void onTimerTick() {
  if (seconds == 0) {
    minutes--;
    seconds = 59;
  } else {
    seconds--;
  }
  timerChange = true;

  static boolean output = HIGH;
  digitalWrite(LED, output);
  output = !output;
}

void onTimerFinish() {
  MsTimer2::stop();
  timerOn = false;
  ledOn();

  playBuzzOnFinish();

  seconds = 0;
  minutes = timeLimit;
}

void writeLcd(String primary, String second) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(primary);
  lcd.setCursor(0, 1);
  lcd.print(second);
}

void writeLcdSecond(String second) {
  lcd.print("          ");
  lcd.setCursor(0, 1);
  lcd.print(second);
}

void ledOn() {
  digitalWrite(LED, HIGH);
}

void ledOff() {
  digitalWrite(LED, LOW);
}

void playBuzzOnFinish() {
  digitalWrite(BUZZ, HIGH);
  delay(500);
  digitalWrite(BUZZ, LOW);
}

void playBuzzOnStart() {
  digitalWrite(BUZZ, HIGH);
  delay(200);
  digitalWrite(BUZZ, LOW);
}

String formatTime(int minutes, int seconds) {
  return toStringTime(minutes) + ":" +  toStringTime(seconds);
}

String toStringTime(int value) {
  if (value < 10) {
    return "0" + String(value, DEC);
  } else {
    return String(value, DEC);
  }
}

int getPotentiometerValue() {
  int valPotentiometer =  analogRead(POTENTIOMETER);
  return map(valPotentiometer, 0, 1023, 0, 255);
}

int getTimeFromPotentiometer(int valPotentiometer) {
  if (valPotentiometer <= 42) {
    return 10;
  } else if (valPotentiometer > 42 && valPotentiometer <= 84) {
    return 9;
  } else if (valPotentiometer > 84 && valPotentiometer <= 126) {
    return 8;
  } else if (valPotentiometer > 126 && valPotentiometer <= 168) {
    return 7;
  } else if (valPotentiometer > 168 && valPotentiometer <= 210) {
    return 6;
  } else if (valPotentiometer > 210) {
    return 5;
  }

}


