#include <SPI.h>
#include <RH_RF95.h>

//-----------------------
#include <FastLED.h>
#define NUM_LEDS 65
#define LED_PIN A1 //*****************************
//-----------------------

//---------------------- win condition variables
#define BRIGHTNESS  200
#define FRAMES_PER_SECOND 60
bool gReverseDirection = false;
#define NUM_LEDS    65
CRGB ledss[NUM_LEDS];


// for feather m0 RFM9x
#define RFM95_CS 8
#define RFM95_RST 4
#define RFM95_INT 3

// Change to 434.0 or other frequency, must match RX's freq!
#define RF95_FREQ 915.0

// Singleton instance of the radio driver
RH_RF95 rf95(RFM95_CS, RFM95_INT);
bool leds = false;
bool prevPressure = false;
bool prevButton = false;

//int vv = 13; //***************************** CHANGE THIS TO WHATEVER PIN THE LEDS ARE CONNECTED TO
//int button = 10;

//-----------------------
int analogPin = A3; //CHANAFE TO A4 FOR TILE 9 !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//int analogPin = A4; //for tile 9 only
int powerPin = A7;
int val = 0;

int R = 0;
int G = 0;
int B = 0;
CRGB led[NUM_LEDS];
//-----------------------

void setup() {
  // put your setup code here, to run once:
//-----------------------
  pinMode(12, OUTPUT);
  pinMode(analogPin, INPUT);
  digitalWrite(12, HIGH);
  pinMode(powerPin, OUTPUT);
  digitalWrite(powerPin, HIGH);
  
  FastLED.addLeds<NEOPIXEL, LED_PIN>(led, NUM_LEDS);
  for(int i = 2;i<NUM_LEDS;i++)
  {
    led[i] = CRGB(0,255,255);
  }
  FastLED.setBrightness(0);
  FastLED.show();
//-----------------------
  
//  pinMode(vv, OUTPUT);
//  pinMode(button, INPUT_PULLUP);

  pinMode(RFM95_RST, OUTPUT);
  digitalWrite(RFM95_RST, HIGH);

  Serial.begin(115200);
//  while (!Serial) {
//    delay(1);
//  }
    // manual reset
  digitalWrite(RFM95_RST, LOW);
//  delay(10);
  digitalWrite(RFM95_RST, HIGH);
//  delay(10);

  while (!rf95.init()) {
    Serial.println("LoRa radio init failed");

  }
  Serial.println("LoRa radio init OK!");

  // Defaults after init are 434.0MHz, modulation GFSK_Rb250Fd250, +13dbM
  if (!rf95.setFrequency(RF95_FREQ)) {
    Serial.println("setFrequency failed");
    while (1);
  }
  Serial.print("Set Freq to: "); Serial.println(RF95_FREQ);
  
  // Defaults after init are 434.0MHz, 13dBm, Bw = 125 kHz, Cr = 4/5, Sf = 128chips/symbol, CRC on

  // The default transmitter power is 13dBm, using PA_BOOST.
  // If you are using RFM95/96/97/98 modules which uses the PA_BOOST transmitter pin, then 
  // you can set transmitter powers from 5 to 23 dBm:
  rf95.setTxPower(23, false);
//  Serial.println("power");
}


void loop() {
  uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
  uint8_t len = sizeof(buf);
  if (rf95.recv(buf, &len)) {
    if (String((char*)buf) == ("Win")) { //if a player won, display. this is same for all tiles
//      Fire2012(); // run simulation frame
      FastLED.setBrightness(175);
      FastLED.show(); // display this frame
//      FastLED.delay(1000 / FRAMES_PER_SECOND);
    }
//    if (String((char*)buf) == ("Win1")) { //if a player won, display only for certain tiles  CHANGE FOR EACH TILE
//      Fire2012(); // run simulation frame
//     FastLED.setBrightness(175);
//      FastLED.show(); // display this frame
//      FastLED.delay(1000 / FRAMES_PER_SECOND);
//    }
//    if (String((char*)buf) == ("Off")) { //if a player won, display only for certain tiles  CHANGE FOR EACH TILE
//      Serial.println("LEDS OFF");
//      FastLED.setBrightness(0);
//      FastLED.show();
//    }
  

    //CHANGE FOR EACH TILE
    if (String((char*)buf) == ("L9")) { //qrs*****************************this will only run if it receives a message for this specific tile, change 1 to diff int for diff tiles
//      leds = true; //turn on LEDs if receives L1
      int R = random(255);
      int G = random(255);
      int B = random(255);
      FastLED.setBrightness(175);
      for(int i = 2; i < NUM_LEDS; i++) {
        led[i] = CRGB(R,G,B);
      }
      FastLED.show();
      Serial.println("changing LEDs");
    } 

    else{//qrs***********
//      leds = false;
      Serial.println("LEDS OFF");
      FastLED.setBrightness(0);
      FastLED.show();

//    }
}}

//  digitalWrite(vv, leds); //leds tells it to go HIGH or LOW

  //if tile is being stepped on and it wasn't already
  //Serial.println(testPressure());
//  if(testPressure() and !prevPressure or !digitalRead(button) and prevButton)
  if(testPressure() and !prevPressure) { //does this test if it's being pressed or not being pressed??????????    
    Serial.println("tile pressed, not prev pressed");
    uint8_t data[] = "B9"; //qrs*****************************change based on tile number CHANGE FOR EACH TILE
    rf95.send(data, sizeof(data));
    rf95.waitPacketSent();
    Serial.println("sent");
  }
  prevPressure = testPressure();
//  prevButton = digitalRead(button);
//  FastLED.show();
}


//returns if being pressed
boolean testPressure() {
  val = analogRead(analogPin);
  Serial.println(val);
  if(val>600) {
    return(true);
  }
  else {
    return(false);
  }
}



//-------------------------------------------------------------------------------------------------------------------------------------

//#define COOLING  55
//#define SPARKING 120
//
//void Fire2012() {
//// Array of temperature readings at each simulation cell
//  static byte heat[NUM_LEDS];
//
//  // Step 1.  Cool down every cell a little
//    for( int i = 0; i < NUM_LEDS; i++) {
//      heat[i] = qsub8( heat[i],  random8(0, ((COOLING * 10) / NUM_LEDS) + 2));
//    }
//  
//    // Step 2.  Heat from each cell drifts 'up' and diffuses a little
//    for( int k= NUM_LEDS - 1; k >= 2; k--) {
//      heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2] ) / 3;
//    }
//    
//    // Step 3.  Randomly ignite new 'sparks' of heat near the bottom
//    if( random8() < SPARKING ) {
//      int y = random8(7);
//      heat[y] = qadd8( heat[y], random8(160,255) );
//    }
//
//    // Step 4.  Map from heat cells to LED colors
//    for( int j = 0; j < NUM_LEDS; j++) {
//      CRGB color = HeatColor( heat[j]);
//      int pixelnumber;
//      if( gReverseDirection ) {
//        pixelnumber = (NUM_LEDS-1) - j;
//      } else {
//        pixelnumber = j;
//      }
//      ledss[pixelnumber] = color;
//    }
//}
