/* PowerTimer
 * Controls power to two devices, such as a soldering iron and a hot glue gun.
 * 
 * Circuit:  Buttons hooked to pins 10 and 3, active low.
 *           Active-low relay controllers on pins 9 and 4.
 *           Built-in LED's on Arduino Nano on pins 1 (active low) and 13 (active high).
 *           Buzzer hooked between pins 5 and 8.
 *           Pin 12 set LOW to act like a ground.  Can only handle 20 mA, but it's only used for one button.
 */

#define PSEUDOGROUND 12
#define BUZZER       5
#define BUZGND       8
unsigned long onTime = 5 * 60  * 1000UL;

#define NRELAYS 2
uint8_t button[] = { 10, 3 };
uint8_t relay[] = { 9, 4 };
uint8_t light[] = { 1, 13 };
uint8_t relayOn[] = { LOW, LOW };
uint8_t lightOn [] = { LOW, HIGH };
unsigned int lastState[] = { 0, 0 };
unsigned int tones[] = { 2000, 500 };
unsigned long starttime[] = {0, 0};
unsigned long timer[NRELAYS] = {0, 0};

void setup() {
  // Set my pseudoground pin to low output.
  pinMode(PSEUDOGROUND, OUTPUT);
  digitalWrite(PSEUDOGROUND, LOW);
  pinMode(BUZGND, OUTPUT);
  digitalWrite(BUZGND, LOW);

  // Set each relay to high (off), each button to active low input, and wipe each timer.
  for (int i = 0; i < NRELAYS; i++) {
    pinMode(relay[i], OUTPUT);
    digitalWrite(relay[i], !relayOn[i]);
    pinMode(light[i], OUTPUT);
    digitalWrite(light[i], !lightOn[i]);
    pinMode(button[i], INPUT_PULLUP);
    starttime[i] = 0;
    timer[i] = 0;
  }

  tone(BUZZER, 1000, 200);
}

void loop() {
  int curstate;
  unsigned long curtime;
  static unsigned long presstime[] = { 0, 0 };  // Last time each button was pressed.

  for (int i = 0; i < NRELAYS; i++) {
    curstate = getButton(button[i]);
    curtime = millis();

    // Four conditions are recognized.
    //  HIGH:  Button is not being pressed.
    //  LOW, initial detection:  Short press.
    //  LOW, short time:  Still in a short press.
    //  LOW, long time:   Long press.
    
    if (curstate == LOW) {
      if (presstime[i] == 0) {
        // This is a short press.  Start and/or increment the timer.
        presstime[i] = curtime;
        if (starttime[i] == 0) {
          // Set the timer, turn on the relay.
          starttime[i] = curtime; timer[i] = 0;
          digitalWrite(relay[i], relayOn[i]);
          digitalWrite(light[i], lightOn[i]);
        }

        // Increment the timer.  Beep to confirm.
        timer[i] += onTime;
        tone(BUZZER, tones[i], 200);
      } else if (curtime - presstime[i] > 1000) {
        // This is a long press.  Wipe out the timer.  Beep to confirm.
        digitalWrite(relay[i], !relayOn[i]);
        digitalWrite(light[i], !lightOn[i]);
        tone(BUZZER, tones[i], 200); delay(300);
        tone(BUZZER, tones[i], 200);
        starttime[i] = 0; timer[i] = 0;
      } else {
        // Middle of a short press.  Do nothing.
      }
    } else {
      // Button was released.  Reset presstime.
      presstime[i] = 0;
    }

    // See if the timer has expired.
    if (starttime[i] > 0 && curtime - starttime[i] >= timer[i]) {
      // Turn off and beep.
      digitalWrite(relay[i], !relayOn[i]);
      digitalWrite(light[i], !lightOn[i]);
      tone(BUZZER, tones[i], 200); delay(300);
      tone(BUZZER, tones[i], 200);
      starttime[i] = 0;
      timer[i] = 0;
    }

  }
}

/*
   getButton(pin) - returns the digitalRead value of the pin with debouncing.
 *  * Edge detection changes the state of the button.
 *  * Any change within 50 ms is ignored.
   This version is "twitchy".  Any noise will trigger a button state change.
*/
int getButton(uint8_t pin) {
  const unsigned long debounceDelay = 50;
  static int lockval[] = { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 };
  static int lastval[] = { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 };
  static unsigned long lasttime[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

  if (pin > 13) return LOW;

  int curval = digitalRead(pin);
  unsigned long curtime = millis();

  // Decide what to do based on when the last button state change occurred.
  if (lasttime[pin] == 0) {
    // There was never a previous button state.  Save it now.
    lastval[pin] = curval;
    lasttime[pin] = curtime;
  } else if (curtime - lasttime[pin] < debounceDelay) {
    // The last button state change was very recent.  Ignore this reading.
  } else if (curval != lastval[pin]) {
    // Edge detected after the delay.  This is a real button state change.
    lastval[pin] = curval;
    lasttime[pin] = curtime;
  }

  return lastval[pin];
}
