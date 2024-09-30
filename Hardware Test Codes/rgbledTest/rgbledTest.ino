#include <Adafruit_NeoPixel.h>
#define PIN 7  
#define NUMPIXELS 2 
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
#define DELAYVAL 500  

void setup() {
  pixels.begin(); 
}

void loop() {
  pixels.clear();  
  pixels.setPixelColor(1, pixels.Color(150, 150, 0));
  pixels.setPixelColor(0, pixels.Color(150, 150, 0));
  pixels.show(); 
  delay(DELAYVAL);  
}
