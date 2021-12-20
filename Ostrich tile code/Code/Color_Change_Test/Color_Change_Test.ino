
#include <FastLED.h>

#define NUM_LEDS 65
#define LED_PIN A1

int analogPin = A3;
int powerPin = A7;
int val = 0;
boolean wasOnLast = false;
int R = 0;
int G = 0;
int B = 0;

CRGB led[NUM_LEDS];
void setup() {
  // put your setup code here, to run once:
  pinMode(13, OUTPUT);
  pinMode(analogPin, INPUT);
  digitalWrite(13, HIGH);
  pinMode(powerPin, OUTPUT);
  digitalWrite(powerPin, HIGH);
  
  FastLED.addLeds<NEOPIXEL, LED_PIN>(led, NUM_LEDS);
  //just initializing
  for(int i = 2;i<NUM_LEDS;i++) {
    led[i] = CRGB(0,255,255);
  }
  FastLED.setBrightness(0);
  FastLED.show();
  
}

void loop() {

  
  if(testPressure()) {
    FastLED.setBrightness(175);
    wasOnLast = true;
    }
  else {
    FastLED.setBrightness(0);
    if(wasOnLast) {
      int R = random(255);
      int G = random(255);
      int B = random(255);
      for(int i = 2;i<NUM_LEDS;i++) {
        led[i] = CRGB(R,G,B);
      }
    }
    wasOnLast = false;
  }
   FastLED.show();
  delay(75);
}

boolean testPressure() {
  val = analogRead(analogPin);
  if(val<400) {
    return(true);
  }
  else {
    return(false);
  }
}
