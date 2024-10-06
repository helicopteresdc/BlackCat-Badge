#include <Arduino.h>
#include <U8g2lib.h>
#include <Adafruit_NeoPixel.h>
#include <Wire.h>
#include "pitches.h"
int melody[] = {
  NOTE_E4, NOTE_G4, NOTE_A4, NOTE_A4, 0,
  NOTE_A4, NOTE_B4, NOTE_C5, NOTE_C5, 0,
  NOTE_C5, NOTE_D5, NOTE_B4, NOTE_B4, 0,
  NOTE_A4, NOTE_G4, NOTE_A4, 0,

  NOTE_E4, NOTE_G4, NOTE_A4, NOTE_A4, 0,
  NOTE_A4, NOTE_B4, NOTE_C5, NOTE_C5, 0,
  NOTE_C5, NOTE_D5, NOTE_B4, NOTE_B4, 0,
  NOTE_A4, NOTE_G4, NOTE_A4, 0,

  NOTE_E4, NOTE_G4, NOTE_A4, NOTE_A4, 0,
  NOTE_A4, NOTE_C5, NOTE_D5, NOTE_D5, 0,
  NOTE_D5, NOTE_E5, NOTE_F5, NOTE_F5, 0,
  NOTE_E5, NOTE_D5, NOTE_E5, NOTE_A4, 0,

  NOTE_A4, NOTE_B4, NOTE_C5, NOTE_C5, 0,
  NOTE_D5, NOTE_E5, NOTE_A4, 0,
  NOTE_A4, NOTE_C5, NOTE_B4, NOTE_B4, 0,
  NOTE_C5, NOTE_A4, NOTE_B4, 0,

  NOTE_A4, NOTE_A4
};

int noteDurations[] = {
  125, 125, 250, 125, 125,
  125, 125, 250, 125, 125,
  125, 125, 250, 125, 125,
  125, 125, 375, 125,

  125, 125, 250, 125, 125,
  125, 125, 250, 125, 125,
  125, 125, 250, 125, 125,
  125, 125, 375, 125,

  125, 125, 250, 125, 125,
  125, 125, 250, 125, 125,
  125, 125, 250, 125, 125,
  125, 125, 125, 250, 125,

  125, 125, 250, 125, 125,
  250, 125, 250, 125,
  125, 125, 250, 125, 125,
  125, 125, 375, 375,

  250, 125
};
const int songspeed = 1.5;
#define PIN 7
#define NUMPIXELS 2

#define DELAYVAL 500
#define U8X8_PIN_NONE -1
#define jsupPin 20
#define jsdownPin 2
#define jsrightPin 10
#define jsleftPin 4
#define jsswitchPin 3
#define reedPin 21
#define buttonAPin 5
#define buttonBPin 6

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* clock=*/9, /* data=*/8, /* reset=*/U8X8_PIN_NONE);
//u8g2.begin(/*Select=*/ 5, /*Right/Next=*/ 10, /*Left/Prev=*/ 4, /*Up=*/ 20, /*Down=*/ 2, /*Home/Cancel=*/ 6);

char modeState = 0;

bool jsupVal = 0;
bool jsdownVal = 0;
bool jsrightVal = 0;
bool jsleftVal = 0;
bool jsswitchVal = 0;

bool buttonAVal = 0;
bool buttonBVal = 0;

bool ledVal = 0;
unsigned long prevtimer = 0;
int interval = 1000;
int count = 0;
bool reedVal = 0;

void setup() {
  u8g2.begin();
  pixels.begin();
  pinMode(jsupPin, INPUT_PULLUP);
  pinMode(jsdownPin, INPUT_PULLUP);
  pinMode(jsrightPin, INPUT_PULLUP);
  pinMode(jsleftPin, INPUT_PULLUP);
  pinMode(jsswitchPin, INPUT_PULLUP);
  pinMode(buttonAPin, INPUT_PULLUP);
  pinMode(buttonBPin, INPUT_PULLUP);
  pinMode(reedPin, INPUT_PULLUP);
  u8g2.setFont(u8g2_font_timR12_tr);
  modeState = 1;
}

void loop() {
  unsigned long timer = millis();
  switch (modeState) {
    case 1:
      u8g2.clearBuffer();
      u8g2.drawStr(0, 20, "HARUUL ZANGI");  //Byte4Bait
      u8g2.drawStr(40, 40, "2024");
      u8g2.drawStr(20, 60, "#1 Byte4Bait");
      u8g2.sendBuffer();
      for (int thisNote = 0; thisNote < 78; thisNote++) {
        int wait = noteDurations[thisNote] * songspeed;
        tone(1, melody[thisNote], wait);
        delay(wait);
        noTone(1);
        modeState = 2;
        prevtimer = timer;
      }
      break;
    case 2:
      jsupVal = digitalRead(jsupPin);
      jsdownVal = digitalRead(jsdownPin);
      jsrightVal = digitalRead(jsrightPin);
      jsleftVal = digitalRead(jsleftPin);
      u8g2.clearBuffer();
      u8g2.drawStr(20, 20, "1. JOYSTICK");
      if (jsupVal == LOW) {
        u8g2.drawStr(55, 50, "UP");
      }
      if (jsdownVal == LOW) {
        u8g2.drawStr(45, 50, "DOWN");
      }
      if (jsrightVal == LOW) {
        u8g2.drawStr(40, 50, "RIGHT");
      }
      if (jsleftVal == LOW) {
        u8g2.drawStr(45, 50, "LEFT");
      }
      u8g2.sendBuffer();
      if (count == 10) {
        modeState = 3;
        count = 0;
      }
      if (timer - prevtimer > interval) {
        count++;
        prevtimer = timer;
      }
      break;
    case 3:
      buttonAVal = digitalRead(buttonAPin);
      buttonBVal = digitalRead(buttonBPin);
      jsswitchVal = digitalRead(jsswitchPin);
      u8g2.clearBuffer();
      u8g2.drawStr(20, 20, "2. BUTTON");
      if (buttonAVal == LOW) {
        u8g2.drawStr(20, 50, "BUTTON A");
      }
      if (buttonBVal == LOW) {
        u8g2.drawStr(20, 50, "BUTTON B");
      }
      if (jsswitchVal == LOW) {
        u8g2.drawStr(35, 50, "SWITCH");
      }
      u8g2.sendBuffer();
      if (count == 5) {
        modeState = 4;
        count = 0;
      }
      if (timer - prevtimer > interval) {
        count++;
        prevtimer = timer;
      }
      break;
    case 4:
      u8g2.clearBuffer();
      u8g2.drawStr(20, 20, "4.RGB LED");
      pixels.clear();
      if (count % 3 == 0) {
        u8g2.drawStr(45, 50, "RED");
        pixels.setPixelColor(1, pixels.Color(150, 0, 0));
        pixels.setPixelColor(0, pixels.Color(150, 0, 0));
      }
      if (count % 3 == 1) {
        u8g2.drawStr(45, 50, "GREEN");
        pixels.setPixelColor(1, pixels.Color(0, 150, 0));
        pixels.setPixelColor(0, pixels.Color(0, 150, 0));
      }
      if (count % 3 == 2) {
        u8g2.drawStr(45, 50, "BLUE");
        pixels.setPixelColor(1, pixels.Color(0, 0, 150));
        pixels.setPixelColor(0, pixels.Color(0, 0, 150));
      }
      if (count == 4) {
        pixels.clear();
        modeState = 5;
        count = 0;
      }
      pixels.show();
      u8g2.sendBuffer();
      if (timer - prevtimer > interval) {
        count++;
        prevtimer = timer;
      }
      break;
    case 5:
      u8g2.clearBuffer();
      u8g2.drawStr(0, 20, "5.REED SWITCH");
      reedVal = digitalRead(reedPin);
      if (reedVal == LOW) {
        u8g2.drawStr(45, 50, "REED");
      }
      u8g2.sendBuffer();
      if (count == 5) {
        modeState = 6;
        count = 0;
      }
      if (timer - prevtimer > interval) {
        count++;
        prevtimer = timer;
      }
      break;
    case 6:
      u8g2.clearBuffer();
      u8g2.drawStr(0, 20, "6.BUZZER");
      u8g2.drawStr(45, 50, "PLAYING");
      u8g2.sendBuffer();
      for (int thisNote = 0; thisNote < 78; thisNote++) {
        int wait = noteDurations[thisNote] * songspeed;
        tone(1, melody[thisNote], wait);
        delay(wait);
        noTone(1);
        modeState = 7;
        prevtimer = timer;
      }
      break;
    case 7:
      u8g2.clearBuffer();
      u8g2.drawStr(0, 20, "FINISH");
      u8g2.drawStr(45, 50, "TESTING");
      u8g2.sendBuffer();
      if (count == 5) {
        modeState = 1;
        count = 0;
        prevtimer = timer;
      }
      if (timer - prevtimer > interval) {
        count++;
        prevtimer = timer;
      }
      break;
  }
}
