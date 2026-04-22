#define LED_1 5
#define LED_2 6
#define LED_3 7
#define BUTTON_1 12
#define BUTTON_2 3
#define BEEPER 11
#define LED_P1 4
#define LED_P2 13
#define WIN_SCORE 3

bool beginCountdown = true;
bool waitInput = false;
int p1Score = 0;
int p2Score = 0;

void beep(int ms, int freq = 1000) {
  int halfPeriod = 1000000 / (freq * 2);
  long cycles = (long)ms * freq / 1000;
  for (long i = 0; i < cycles; i++) {
    digitalWrite(BEEPER, HIGH);
    delayMicroseconds(halfPeriod);
    digitalWrite(BEEPER, LOW);
    delayMicroseconds(halfPeriod);
  }
}

void waitForRelease() {
  while (digitalRead(BUTTON_1) == LOW || digitalRead(BUTTON_2) == LOW) {
    delay(10);
  }
  delay(40);
}

void resetRound() {
  beginCountdown = true;
  waitInput = false;
}

void setup() {
  pinMode(BUTTON_1, INPUT_PULLUP);
  pinMode(BUTTON_2, INPUT_PULLUP);
  pinMode(LED_1, OUTPUT);
  pinMode(LED_2, OUTPUT);
  pinMode(LED_3, OUTPUT);
  pinMode(LED_P1, OUTPUT);
  pinMode(LED_P2, OUTPUT);
  pinMode(BEEPER, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  if (beginCountdown) {
    beginCountdown = false;

    digitalWrite(LED_1, HIGH); beep(1000, 700);
    digitalWrite(LED_2, HIGH); beep(1000, 850);
    digitalWrite(LED_3, HIGH); beep(1000, 1000);
    digitalWrite(LED_1, LOW);
    digitalWrite(LED_2, LOW);
    digitalWrite(LED_3, LOW);

    waitForRelease();
    waitInput = true;
  }

  if (waitInput) {
    bool b1 = digitalRead(BUTTON_1) == LOW;
    bool b2 = digitalRead(BUTTON_2) == LOW;

    if (b1 || b2) {
      delay(20);
      b1 = digitalRead(BUTTON_1) == LOW;
      b2 = digitalRead(BUTTON_2) == LOW;

      if (b1 && b2) {
        beep(150, 500);
        waitForRelease();
        resetRound();

      } else if (b1) {
        beep(80, 1200);
        p1Score++;
        Serial.print("P1: "); Serial.print(p1Score);
        Serial.print(" | P2: "); Serial.println(p2Score);
        digitalWrite(LED_P1, HIGH);
        delay(2000);
        digitalWrite(LED_P1, LOW);
        if (p1Score >= WIN_SCORE) {
          for (int i = 0; i < 5; i++) {
            digitalWrite(LED_P1, HIGH);
            beep(100, 1500);
            delay(100);
            digitalWrite(LED_P1, LOW);
            delay(100);
          }
          p1Score = 0;
          p2Score = 0;
        }
        waitForRelease();
        resetRound();

      } else if (b2) {
        beep(80, 1200);
        p2Score++;
        Serial.print("P1: "); Serial.print(p1Score);
        Serial.print(" | P2: "); Serial.println(p2Score);
        digitalWrite(LED_P2, HIGH);
        delay(2000);
        digitalWrite(LED_P2, LOW);
        if (p2Score >= WIN_SCORE) {
          for (int i = 0; i < 5; i++) {
            digitalWrite(LED_P2, HIGH);
            beep(100, 1500);
            delay(100);
            digitalWrite(LED_P2, LOW);
            delay(100);
          }
          p1Score = 0;
          p2Score = 0;
        }
        waitForRelease();
        resetRound();
      }
    }
  }
}
