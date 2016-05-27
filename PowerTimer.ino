/* PowerTimer
 *  
 */


#define PSEUDOGROUND 12
#define BUTTON1      10
#define BUTTON2      3
#define RELAY1       9
#define RELAY2       4
#define LIGHT2       13
#define LIGHT1       1
#define MODE2        HIGH
#define MODE1        LOW
#define NRELAYS 2
#define BUZZER       5
#define BUZGND       8
#define TONE1        2000
#define TONE2        500

uint8_t button[] = { BUTTON1, BUTTON2 };
uint8_t relay[] = { RELAY1, RELAY2 };
uint8_t light[] = { LIGHT1, LIGHT2 };
uint8_t mode[] = { MODE1, MODE2 };
unsigned int tones[] = { TONE1, TONE2 };
unsigned long onTime = 5 * 60 * 1000UL;
unsigned long timer[NRELAYS];

void setup() {
  // Set my pseudoground pin to low output.
  pinMode(PSEUDOGROUND, OUTPUT);
  digitalWrite(PSEUDOGROUND, LOW);
  pinMode(BUZGND, OUTPUT);
  digitalWrite(BUZGND, LOW);

  // Set each relay to high, each button to active low input, and wipe each timer.
  for (int i = 0; i < NRELAYS; i++) {
    pinMode(relay[i], OUTPUT);
    digitalWrite(relay[i], HIGH);
    pinMode(light[i], OUTPUT);
    digitalWrite(light[i], !mode[i]);
    pinMode(button[i], INPUT_PULLUP);
    timer[i] = 0;
  }

  tone(BUZZER, 1000, 200);
}

void loop() {
  unsigned long curtime = millis();

  for (int i = 0; i < NRELAYS; i++) {
    if (digitalRead(button[i]) == LOW) {
      timer[i] = max(curtime,timer[i]) + onTime;
      digitalWrite(relay[i], LOW);
      digitalWrite(light[i], mode[i]);
      tone(BUZZER, tones[i], 200);
      delay(500);
      continue;
    }
    if (timer[i] != 0 && (long)(curtime-timer[i]) > 0) {
      timer[i] = 0;
      digitalWrite(relay[i], HIGH);
      digitalWrite(light[i], !mode[i]);
      tone(BUZZER, tones[i], 200); delay(300);
      tone(BUZZER, tones[i], 200);
      continue;
    }
  }

}
