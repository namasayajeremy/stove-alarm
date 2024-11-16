#include "FlameSensor.h"
#include "KeypadHandler.h"
#include <TM1637Display.h>


const int CLK_PIN = D3;
const int DIO_PIN = D4;
const int piezoPin = D0;
FlameSensor flameSensor(A0, 800, 5000);
KeypadHandler keypadHandler;
TM1637Display display(CLK_PIN, DIO_PIN);


// Timer variables
unsigned long countdownTime = 0;
unsigned long flameStartTime = 0;

// Define modes
enum Mode {
  STANDBY,
  RUNNING,
  BEEPING
};
Mode mode = STANDBY;


void setup() {
  display.setBrightness(0);
  pinMode(piezoPin, OUTPUT);
  Serial.begin(9600);
  Serial.println("Setup begins");
}

void loop() {
  // Mode selection
  switch (mode) {
    case STANDBY:
      //Serial.println("Stand by");
      if (flameSensor.flameDetected()) {
        //Serial.println("flame detected");
        adjustCountdownTime();
        if (flameStartTime == 0) {
          flameStartTime = millis() / 1000;
        }
        if (millis() / 1000 - flameStartTime >= 15) {
          countdownTime = 180;
          startBeep();
          mode = RUNNING;
        }
      } else {
        display.clear();
        countdownTime = 0;
        flameStartTime = 0;
      }
      break;

    case RUNNING:
      Serial.println("Running");
      {
        unsigned char key = keypadHandler.getKey();
        unsigned long remainingTimeValue = remainingTime();
        Serial.print("Remaining Time: ");
        Serial.println(remainingTimeValue);
        if (!flameSensor.flameDetected() || remainingTimeValue <= 0 || key == '0') {
          mode = BEEPING;
        } else {
          displayFormatted(remainingTimeValue);
        }
      }
      break;

    case BEEPING:
      {
        Serial.println("Beeping");
        playBeep();  // Continue beeping //add counter
        char key = keypadHandler.getKey();
        if (key == '0' || !flameSensor.flameDetected()) {  // Stop beeping if OK or flame is out
          mode = STANDBY;
        }
      }
      break;
  }
}

void adjustCountdownTime() {
  char key = keypadHandler.getKey();
  if (key) {
    if (key == '-') {
      countdownTime = (countdownTime >= 60) ? countdownTime - 60 : 0;
    } else if (key == '1') {
      countdownTime += 60;
    } else if (key == '5') {
      countdownTime += 300;
    } else if (key == '0') {
      if (countdownTime > 0) {
        startBeep();
        mode = RUNNING;  // Switch to RUNNING mode
      }
    }
  }
  displayFormatted(countdownTime);
}

unsigned long remainingTime() {
  unsigned long currentTime = millis() / 1000;
  unsigned long ellapsedTime = currentTime - flameStartTime;  // Get the current time in seconds
  return countdownTime - ellapsedTime;                        // Return the remaining time
}

void displayFormatted(unsigned long countdownTime) {  //seconds
  int minutes = countdownTime / 60;
  int seconds = countdownTime % 60;
  // Display MM:SS format on the 7-segment display
  display.showNumberDecEx(minutes * 100 + seconds, 0b01000000, true);
}


void playBeep() {
  for (int i = 0; i < 3; i++) {
    tone(piezoPin, 4978, 100);
    display.showNumberDec(0, true);
    delay(100 + 100);
    display.clear();
  }
  tone(piezoPin, 4978, 300);
  display.showNumberDec(0, true);
  delay(300 + 100);
  noTone(piezoPin);  // Stop the tone
  display.clear();
  delay(500);
  display.showNumberDec(0, true);
  delay(500);
  display.clear();
  delay(500);
};

void startBeep() {
  for (int i = 0; i < 3; i++) {
    tone(piezoPin, 4978, 100);
    displayFormatted(countdownTime);
    delay(100 + 100);
    display.clear();
  }
}