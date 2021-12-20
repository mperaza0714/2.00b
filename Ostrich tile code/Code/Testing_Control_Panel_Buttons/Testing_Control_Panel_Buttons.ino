#include <SPI.h>
#include <RH_RF95.h>
#include <FastLED.h>
#include <TM1637Display.h>

//---------------------- for feather m0 RFM9x
#define RFM95_CS 8
#define RFM95_RST 4
#define RFM95_INT 3

//---------------------- Change to 434.0 or other frequency, must match RX's freq!
#define RF95_FREQ 915.0
// Singleton instance of the radio driver
RH_RF95 rf95(RFM95_CS, RFM95_INT);

int controlButton1 = 15; //music
int controlButton2 = 16; //multi
int controlButton3 = 17; //single
int winner = 0;
int score = 0;
int score1 = 0;
int score2 = 0;
int correct1 = 0;
int correct2 = 0;
int correct = 0;
bool loops = true; //basically a break but ryan hates breaks
uint8_t data[] = "1";

String state = "Setup";
int prevControlPressed = 0;


void setup() {
  // put your setup code here, to run once:
  pinMode(controlButton1, INPUT_PULLUP);
  pinMode(controlButton2, INPUT_PULLUP);
  pinMode(controlButton3, INPUT_PULLUP);
}


void loop() {

//  Serial.println(state);
  if (!digitalRead(controlButton1) and prevControlPressed != 1) {
    Serial.println("button Rando music pressed");
    reset();
    state = "RandoMusic";
    prevControlPressed = 1;
  }
  if (!digitalRead(controlButton2) and prevControlPressed != 2) {
    reset();
    state = "MultiColorStomp";
    prevControlPressed = 2;
  }
  if (!digitalRead(controlButton3) and prevControlPressed != 3) {
    reset();
    state = "SingleColorStomp";
    prevControlPressed = 3;
  }
 
  if (state == "SingleColorStomp") { 
    singleColorStomp();
  }
  else if (state == "MultiColorStomp") {
    multiColorStomp();
  }
  else if (state == "RandoMusic") {
    randoMusic();
  }
  
//  multiColorStomp();
//  turnAllOn();

  if (state == "Won") {
    Serial.println("won");
    Serial.println(winner);
     //if someone won, send that they won
    if (winner == 3) { //single player, no winner, all tiles will light up 
      uint8_t data[] = "W";
      rf95.send(data, sizeof(data));
      Serial.println("midlde of sending");
      rf95.waitPacketSent();
      Serial.println("sent Win");
    }
    if (winner == 1) { //player 1 won in multiplayer
      uint8_t data[] = "W1";
      Serial.println("boutta send bc win1");
      rf95.send(data, sizeof(data));
      Serial.println("midlde of sending");
      rf95.waitPacketSent();
      Serial.println("sent Win1");
    }
    if (winner == 2) {
      uint8_t data[] = "W2";
      rf95.send(data, sizeof(data));
      Serial.println("midlde of sending");
      rf95.waitPacketSent();
      Serial.println("sent Win2");
    }
    state = "Setup"; ///????????? IMPLEMENT
  }
}

void singleColorStomp() {
  Serial.print("this is single color stomp");
  Serial.println(score);
  score ++;
  if (score >= 50) {
    Serial.println("Wonnnnn colorstom");
    winner = 3;
    state = "Won";
  }
}

void multiColorStomp() {
  Serial.print("this is multi color stomp");
  Serial.println(score);
  score ++;
  if (score >= 50) {
    Serial.println("Wonnnnn multi comlor stomp");
    winner = 1;
    state = "Won";
  }
}

void randoMusic() {
  Serial.print("this is rando music");
  Serial.println(score);  
  score ++;
  if (score >= 50) {
    Serial.println("Wonnnnn rando musc");
    winner = 0;
    state = "Won";
  }
}

void reset() {
  score = 0;
  correct = 0;
  score1 = 0;
  score2 = 0;
  correct1 = 0;
  correct2 = 0;
  winner = 0;
  loops = true;
  prevControlPressed = 0;
}
