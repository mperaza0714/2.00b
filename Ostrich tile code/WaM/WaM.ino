#include <FastLED.h>
#define DATA_PIN 5
#define LED_PIN     5
#define COLOR_ORDER GRB
#define CHIPSET     WS2811
#define NUM_LEDS    60
#define BRIGHTNESS  200
#define FRAMES_PER_SECOND 60
bool gReverseDirection = false;

CRGB leds[NUM_LEDS];
//make it competitive
int ledPins[] = {2,3,4};
int checks[] = {10,11,12};

int points = 0;
int curPressed = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(ledPins[0], OUTPUT);
  pinMode(ledPins[1], OUTPUT);
  pinMode(ledPins[2], OUTPUT);
  //led strip pin
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);

  pinMode(checks[0], INPUT_PULLUP);
  pinMode(checks[1], INPUT_PULLUP);
  pinMode(checks[2], INPUT_PULLUP);
}

void loop() {
   // put your main code here, to run repeatedly:
  if (points >= 15) { //win condition
    Fire2012(); // run simulation frame
    FastLED.show(); // display this frame
    FastLED.delay(1000 / FRAMES_PER_SECOND);
   }    

  else { //if the game is still going on
    int rando = random(0, 9);   //pick a random number
    int x = 0;
    int wrong[] = {0,0,0,0,0,0,0,0};
    for (int i = 0; i < 8; i++) {
      if (x != rando) {
        wrong[x] = x;
        digitalWrite(ledPins[wrong[x]], LOW);
        x++;
      }
    }
    digitalWrite(ledPins[rando], HIGH); //turn on the random one and turn off the other ones

  //display the light for 1 second. if pressed in that one second, earn a point
    unsigned long StartTime = millis();
    unsigned long CurrentTime = millis();
    unsigned long ElapsedTime = CurrentTime - StartTime;
    
    while (ElapsedTime < 800) {
      CurrentTime = millis();
      ElapsedTime = CurrentTime - StartTime;
      
      // if none are being pressed, set the curPressed to 5 (a random number)
      if (digitalRead(checks[0]) and digitalRead(checks[1]) and digitalRead(checks[2])) {
        curPressed = 5;
      }

      //if the correct one is pressed and it was just newly pressed, you get a point and the LED strip shows it
      if (!digitalRead(checks[rando]) and curPressed != rando) {
        digitalWrite(ledPins[rando], LOW);
        points ++; //give a point
        curPressed = rando;
        for (int i = 0; i < points; i++) { //display number of points on the LED
          leds[i] = CRGB(0,255,255);
        }          
        FastLED.setBrightness(20);
        FastLED.show();
        break;
      }

      for (int i = 0; i < 7; i++) {
        if (!digitalRead(checks[wrong[i]]) and curPressed != wrong[i]) {
          leds[points - 1] = CRGB(0,0,0); //remove a point visually
          points--; 
          if (points < 0) {
            points = 0;
          }
          for (int i = 0; i < points; i++) { //update the leds for points display
            leds[i] = CRGB(0,255,255);
          }          
          FastLED.setBrightness(20);
          FastLED.show();
          curPressed = wrong[i];
          break;
        }
      }
    }
  }
}


#define COOLING  55
#define SPARKING 120

void Fire2012()
{
// Array of temperature readings at each simulation cell
  static byte heat[NUM_LEDS];

  // Step 1.  Cool down every cell a little
    for( int i = 0; i < NUM_LEDS; i++) {
      heat[i] = qsub8( heat[i],  random8(0, ((COOLING * 10) / NUM_LEDS) + 2));
    }
  
    // Step 2.  Heat from each cell drifts 'up' and diffuses a little
    for( int k= NUM_LEDS - 1; k >= 2; k--) {
      heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2] ) / 3;
    }
    
    // Step 3.  Randomly ignite new 'sparks' of heat near the bottom
    if( random8() < SPARKING ) {
      int y = random8(7);
      heat[y] = qadd8( heat[y], random8(160,255) );
    }

    // Step 4.  Map from heat cells to LED colors
    for( int j = 0; j < NUM_LEDS; j++) {
      CRGB color = HeatColor( heat[j]);
      int pixelnumber;
      if( gReverseDirection ) {
        pixelnumber = (NUM_LEDS-1) - j;
      } else {
        pixelnumber = j;
      }
      leds[pixelnumber] = color;
    }
}
