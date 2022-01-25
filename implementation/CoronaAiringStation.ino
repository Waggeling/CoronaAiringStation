/* pitch values for notes */
#include "pitches.h"

/* melody of Mario Theme */
#include "melodyMarioTheme.h"

/* DHT definitions & declaration */
#include "DHT.h"

#define DHTPIN 2
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

/* port definitions */
#define melodyPin 8
#define btnPin 3

/* global variable declarations*/
volatile int oldState = -1;
volatile int state = -1;
volatile bool interrupted = false;

float startHumidity;
float currentHumidity;
float diffHumidity;
float deltaHumidity = 10.0;    /* max humidity difference */

unsigned long startTime = 0;
unsigned long windowOpening = 1200000;    /* timer 1 till next airing; 20 minutes */
unsigned long windowClosing = 300000;    /* timer 2 till window closing; 5 minutes */

void setup() {
  Serial.begin(9600);
  
  dht.begin();
  
  pinMode(melodyPin, OUTPUT);
  pinMode(btnPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(btnPin), stopMelody, RISING);
}

void loop() {
  switch(state) {
    case -1: /* initial state: delay for humidity sensor */
      if (windowOpening < 2000) {
        int delayTime = 2000 - windowOpening;
        delay(delayTime);
      }
      state = 0;
      break;
      
    case 0: /* start state or timer 2 ended */
      interrupted = false;
      startTime = millis();
      Serial.print("Timer 1 is running...\n");
      state = 1;
      break;
      
    case 1: /* timer 1 till next airing */
      if (millis() - startTime >= windowOpening) {
        showTime(startTime);
        Serial.print("Timer 1 ended.\nPlease open your window.\n");
        oldState = 1;
        state = 4;
      }
      break;
      
    case 2: /* timer 1 ended */
      interrupted = false;
      setStartHumidity();
      startTime = millis();
      Serial.print("Timer 2 is running...\n");
      state = 3;
      break;
      
    case 3: /* timer 2 till window closing */
      if (millis() - startTime >= windowClosing) {
        showTime(startTime);
        Serial.print("Timer 2 ended.\nYou can close your window now.\n\n");
        oldState = 3;
        state = 4;
      }
      else {
        currentHumidity = dht.readHumidity();
        diffHumidity = abs(startHumidity - currentHumidity);
        if (diffHumidity >= deltaHumidity) {
          Serial.print("Humidity delta achieved.\nYou can close your window now.\n\n");
          oldState = 3;
          state = 4;
        }
      }
      break;
      
    case 4: /* play melody */
      playMelody();
      delay(1000);
      break;
  }
}

void playMelody() {
  int size = sizeof(melody) / sizeof(int);
  for (int thisNote = 0; thisNote < size; thisNote++) {
    if (!interrupted) {
      int noteDuration = 1000 / noteDurations[thisNote];
      tone(melodyPin, melody[thisNote], noteDuration);
      int pauseBetweenNotes = noteDuration * 1.70;
      delay(pauseBetweenNotes);
      noTone(melodyPin);
    }
  }
}

void stopMelody() {
  if (state == 4) {
    interrupted = true;
    state = (oldState + 1) % 4;
  }
}

void setStartHumidity() {
  startHumidity = dht.readHumidity();
  Serial.print("\nStart humidity: ");
  Serial.print(startHumidity);
  Serial.println(" %");
}

void showTime(unsigned long startTime) {
  Serial.print("Start time: ");
  Serial.print(startTime);
  Serial.print("\nEnd time: ");
  Serial.print(millis());
  Serial.print("\n");
}
