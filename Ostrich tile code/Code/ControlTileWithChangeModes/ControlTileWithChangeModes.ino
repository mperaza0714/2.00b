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

//---------------------- FOR SCOREBOARD
const int CLK = 13; //Set the CLK pin connection to the display (4th one, blue new is green
const int DIO = 12; //Set the DIO pin connection to the display (3rd
const int CLK1 = 11; //Set the CLK pin for display 2 connection (4th one, blue new is green
const int DIO1 = 10; //Set the DIO pin connection for display 2

TM1637Display display(CLK, DIO);  //set up the 4-Digit Display.
TM1637Display display1(CLK1, DIO1); //set up 4-digit display player 2

//---------------------- control panel stuff (dont know which pins yet so change later!!!!!!!!!!!!!!!!)
int controlButton1 = 15; //music
int controlButton2 = 16; //multi
int controlButton3 = 17; //single

String state = "Setup";
int prevControlPressed = 0;

//---------------------- initialize variables for Color Stomp
//int prevTime = 0;
int score = 0;
int correct = 0;
uint8_t data[] = "1";
bool loops = true; //basically a break but ryan hates breaks

int score1 = 0;
int score2 = 0;
int correct1 = 0;
int correct2 = 0;
int winner = 0; //no winner. 1 means player 1 won, 2 means player 2 won

//-------------------------------------------------------------------------------------------------------------------------------------
void setup() {
  // put your setup code here, to run once:
  pinMode(controlButton1, INPUT_PULLUP);
  pinMode(controlButton2, INPUT_PULLUP);
  pinMode(controlButton3, INPUT_PULLUP);

  
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
  
  if (!rf95.setFrequency(RF95_FREQ)) { // Defaults after init are 434.0MHz, modulation GFSK_Rb250Fd250, +13dbM
    Serial.println("setFrequency failed");
    while (1);
  }
  Serial.print("Set Freq to: "); Serial.println(RF95_FREQ);
  // Defaults after init are 434.0MHz, 13dBm, Bw = 125 kHz, Cr = 4/5, Sf = 128chips/symbol, CRC on. The default transmitter power is 13dBm, using PA_BOOST. If you are using RFM95/96/97/98 modules which uses the PA_BOOST transmitter pin, then you can set transmitter powers from 5 to 23 dBm:
  rf95.setTxPower(23, false);

  display.setBrightness(0x0a);  //set the diplay to maximum brightness
  display1.setBrightness(0x0a); // set display 2 to max bight
}
//-------------------------------------------------------------------------------------------------------------------------------------
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
    turnAllOn();
//    randoMusic();
  }
//  singleColorStomp();
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
    state = "Setup";
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

//-------------------------------------------------------------------------------------------------------------------------------------
void turnAllOn() {
      Serial.println("running turnAllOn");
      uint8_t data[] = "L1";
      rf95.send(data, sizeof(data));
      rf95.waitPacketSent(); 
      Serial.println("sentttttt L1");
      
      uint8_t data2[] = "L2";
      rf95.send(data2, sizeof(data));
      rf95.waitPacketSent(); 
      Serial.println("sentttttt L2");
    
      uint8_t data3[] = "L3";
      rf95.send(data3, sizeof(data));
      rf95.waitPacketSent(); 
      Serial.println("sentttttt L3");
      
      uint8_t data4[] = "L4";
      rf95.send(data4, sizeof(data));
      rf95.waitPacketSent(); 
      Serial.println("sentttttt L4"); 
      
      uint8_t data5[] = "L5";
      rf95.send(data5, sizeof(data));
      rf95.waitPacketSent(); 
      Serial.println("sentttttt L5");

      uint8_t data6[] = "L6";
      rf95.send(data6, sizeof(data));
      rf95.waitPacketSent(); 
      Serial.println("sentttttt L6");

      uint8_t data7[] = "L7";
      rf95.send(data7, sizeof(data));
      rf95.waitPacketSent(); 
      Serial.println("sentttttt L7");

      uint8_t data8[] = "L8";
      rf95.send(data8, sizeof(data));
      rf95.waitPacketSent(); 
      Serial.println("sentttttt L8");

      uint8_t data9[] = "L9";
      rf95.send(data9, sizeof(data));
      rf95.waitPacketSent(); 
      Serial.println("sentttttt L9");
}

//-------------------------------------------------------------------------------------------------------------------------------------
void singleColorStomp() {
    Serial.println("running singlecolorstomp");
    correct = (random(1,10)); //Generates the correct tile to be lit and converts it to a string to be sent
//  correct = 9;
    Serial.print("the random number is: ");
    Serial.println(correct);
        
  //uint8_t data = random(1,9); //Generates the correct tile to be lit and converts it to a string to be sent
  loops = true; // resets looping to true at the beginning of each game cycle (basically break)
//  if (rf95.available()) { //rf95.available()
    //Broadcasts the number of the tile that needs to light up
    if (correct == 1) {
      uint8_t data[] = "L1";
      rf95.send(data, sizeof(data));
      rf95.waitPacketSent(); 
//      Serial.println("sentttttt L1");
    }
    if (correct == 2) {
      uint8_t data[] = "L2";
      rf95.send(data, sizeof(data));
      rf95.waitPacketSent(); 
//      Serial.println("sentttttt L2");
    }
    if (correct == 1) {
      uint8_t data[] = "L1";
      rf95.send(data, sizeof(data));
      rf95.waitPacketSent(); 
      Serial.println("sentttttt L1");
    }
    if (correct == 2) {
      uint8_t data[] = "L2";
      rf95.send(data, sizeof(data));
      rf95.waitPacketSent(); 
      Serial.println("sentttttt L2");
    }
    if (correct == 3) {
      uint8_t data[] = "L3";
      rf95.send(data, sizeof(data));
      rf95.waitPacketSent(); 
      Serial.println("sentttttt L3");
    }
    if (correct == 4) {
      uint8_t data[] = "L4";
      rf95.send(data, sizeof(data));
      rf95.waitPacketSent(); 
      Serial.println("sentttttt L4");
    }     
    if (correct == 5) {
      uint8_t data[] = "L5";
      rf95.send(data, sizeof(data));
      rf95.waitPacketSent(); 
      Serial.println("sentttttt L5");
    }
    if (correct == 6) {
      uint8_t data[] = "L6";
      rf95.send(data, sizeof(data));
      rf95.waitPacketSent(); 
      Serial.println("sentttttt L6");
    }    
    if (correct == 7) {
      uint8_t data[] = "L7";
      rf95.send(data, sizeof(data));
      rf95.waitPacketSent(); 
      Serial.println("sentttttt L7");
    }
    if (correct == 8) {
      uint8_t data[] = "L8";
      rf95.send(data, sizeof(data));
      rf95.waitPacketSent(); 
      Serial.println("sentttttt L8");
    }   
    if (correct == 9) {
      uint8_t data[] = "L9";
      rf95.send(data, sizeof(data));
      rf95.waitPacketSent(); 
      Serial.println("sentttttt L9");
    }  
    int sendTime = millis(); //keeps track of how long game has gone on to be able to decide when to take point for no response
    
    while ((millis() < 2000 + sendTime) && loops) { //check for 5 seconds whether a button was pressed before changing to a new tile
      uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
      uint8_t len = sizeof(buf);



        if (!digitalRead(controlButton1) and prevControlPressed != 1) {
          Serial.println("button Rando music pressed");
          reset();
          state = "RandoMusic";
          prevControlPressed = 1;
          loops = false;
        }
        if (!digitalRead(controlButton2) and prevControlPressed != 2) {
          reset();
          state = "MultiColorStomp";
          prevControlPressed = 2;
          loops = false;
        }
        if (!digitalRead(controlButton3) and prevControlPressed != 3) {
          reset();
          state = "SingleColorStomp";
          prevControlPressed = 3;
          loops = false;
        }

      
      if (rf95.recv(buf, &len)) { //if receive something
//-----------------------------
        if (String((char*)buf) == ("B1") and correct == 1) { //if we receive a signal that the correct one was jumped on 
          Serial.println("got b1");
          score++;
          //happy beep
          loops = false; // breaks this game cycle
          //prevTime = millis(); //makes prevTime into curr time
        }
        else if (String((char*)buf) != ("B1") and correct == 1) { //if wrong pressed //take some points
          score = max(0, score - 1);
          Serial.println("score decreased bc wrong one pressed 1");
          //sad beep
          loops = false; //break
          //prevTime = millis(); //makes the prevTime into the current time 
        }
        if (String((char*)buf) == ("B2") and correct == 2) { //if we receive a signal that the correct one was jumped on 
          Serial.println("got b2");
          score++;
          //happy beep
          loops = false; // breaks this game cycle
          //prevTime = millis(); //makes prevTime into curr time
        }
        else if (String((char*)buf) != ("B2") and correct == 2) { //if wrong pressed //take some points
          score = max(0, score - 1);
          Serial.println("score decreased bc wrong one pressed 2");
          //sad beep
          loops = false; //break
          //prevTime = millis(); //makes the prevTime into the current time 
        }
         if (String((char*)buf) == ("B3") and correct == 3) { //if we receive a signal that the correct one was jumped on 
          Serial.println("got b3");
          score++;
          //happy beep
          loops = false; // breaks this game cycle
          //prevTime = millis(); //makes prevTime into curr time
        }
        else if (String((char*)buf) != ("B3") and correct == 3) { //if wrong pressed //take some points
          score = max(0, score - 1);
          Serial.println("score decreased bc wrong one pressed 3");
          //sad beep
          loops = false; //break
          //prevTime = millis(); //makes the prevTime into the current time 
        }
         if (String((char*)buf) == ("B4") and correct == 4) { //if we receive a signal that the correct one was jumped on 
          Serial.println("got b4");
          score++;
          //happy beep
          loops = false; // breaks this game cycle
          //prevTime = millis(); //makes prevTime into curr time
        }
        else if (String((char*)buf) != ("B4") and correct == 4) { //if wrong pressed //take some points
          score = max(0, score - 1);
          Serial.println("score decreased bc wrong one pressed 4");
          //sad beep
          loops = false; //break
          //prevTime = millis(); //makes the prevTime into the current time 
        }
         if (String((char*)buf) == ("B5") and correct == 5) { //if we receive a signal that the correct one was jumped on 
          Serial.println("got b5");
          score++;
          //happy beep
          loops = false; // breaks this game cycle
          //prevTime = millis(); //makes prevTime into curr time
        }
        else if (String((char*)buf) != ("B5") and correct == 5) { //if wrong pressed //take some points
          score = max(0, score - 1);
          Serial.println("score decreased bc wrong one pressed 5");
          //sad beep
          loops = false; //break
          //prevTime = millis(); //makes the prevTime into the current time 
        }
         if (String((char*)buf) == ("B6") and correct == 6) { //if we receive a signal that the correct one was jumped on 
          Serial.println("got b6");
          score++;
          //happy beep
          loops = false; // breaks this game cycle
          //prevTime = millis(); //makes prevTime into curr time
        }
        else if (String((char*)buf) != ("B6") and correct == 6) { //if wrong pressed //take some points
          score = max(0, score - 1);
          Serial.println("score decreased bc wrong one pressed 6");
          //sad beep
          loops = false; //break
          //prevTime = millis(); //makes the prevTime into the current time 
        }
         if (String((char*)buf) == ("B7") and correct == 7) { //if we receive a signal that the correct one was jumped on 
          Serial.println("got b7");
          score++;
          //happy beep
          loops = false; // breaks this game cycle
          //prevTime = millis(); //makes prevTime into curr time
        }
        else if (String((char*)buf) != ("B7") and correct == 7) { //if wrong pressed //take some points
          score = max(0, score - 1);
          Serial.println("score decreased bc wrong one pressed 7");
          //sad beep
          loops = false; //break
          //prevTime = millis(); //makes the prevTime into the current time 
        }
         if (String((char*)buf) == ("B8") and correct == 8) { //if we receive a signal that the correct one was jumped on 
          Serial.println("got b8");
          score++;
          //happy beep
          loops = false; // breaks this game cycle
          //prevTime = millis(); //makes prevTime into curr time
        }
        else if (String((char*)buf) != ("B8") and correct == 8) { //if wrong pressed //take some points
          score = max(0, score - 1);
          Serial.println("score decreased bc wrong one pressed 8");
          //sad beep
          loops = false; //break
          //prevTime = millis(); //makes the prevTime into the current time 
        }
            //-----------------------------        
         if (String((char*)buf) == ("B9") and correct == 9) { //if we receive a signal that the correct one was jumped on 
          Serial.println("got b8");
          score++;
          //happy beep
          loops = false; // breaks this game cycle
          //prevTime = millis(); //makes prevTime into curr time
        }
        else if (String((char*)buf) != ("B9") and correct == 9) { //if wrong pressed //take some points
          score = max(0, score - 1);
          Serial.println("score decreased bc wrong one pressed 9");
          //sad beep
          loops = false; //break
          //prevTime = millis(); //makes the prevTime into the current time 
        }
      }
    }// end of while loop
    //tell all tiles to turn off; test this later?
//    uint8_t data10[] = "Off";
//    rf95.send(data10, sizeof(data));
//    rf95.waitPacketSent(); 
//    Serial.println("sentttttt off/ end of round");
  
  //Serial.print("score ");
  //Serial.println(score);
  display.showNumberDec(score);
  //SCORE PANEL UPDATE WITH SCORE

  //win condition
    if (score >= 10) {
      state = "Won";
    }
}
//-------------------------------------------------------------------------------------------------------------------------------------

void multiColorStomp() {
    Serial.println("now playing multiColorStomp");
    correct1 = random(1,5); //Generates the correct tile to be lit and converts it to a string to be sent
    correct2 = random(5,9);
//    correct1 = 1;
    correct2 = 6;
//  correct = 1;
    Serial.print("the random numbers are : ");
    Serial.print(correct1);
    Serial.print("and");
    Serial.println(correct2);
    
        
  //uint8_t data = random(1,9); //Generates the correct tile to be lit and converts it to a string to be sent
  loops = true; // resets looping to true at the beginning of each game cycle (basically break)
//  if (rf95.available()) { //rf95.available()
    //Broadcasts the number of the tile that needs to light up
    if (correct1 == 1) {
      uint8_t data[] = "L1";
      rf95.send(data, sizeof(data));
      rf95.waitPacketSent(); 
      Serial.println("sentttttt L1");
    }
    if (correct1 == 2) {
      uint8_t data[] = "L2";
      rf95.send(data, sizeof(data));
      rf95.waitPacketSent(); 
      Serial.println("sentttttt L2");
    }
    if (correct1 == 3) {
      uint8_t data[] = "L3";
      rf95.send(data, sizeof(data));
      rf95.waitPacketSent(); 
      Serial.println("sentttttt L3");
    }
    if (correct1 == 4) {
      uint8_t data[] = "L4";
      rf95.send(data, sizeof(data));
      rf95.waitPacketSent(); 
      Serial.println("sentttttt L4");
    }
//   tile 5 is unused
//   THIS IS THE FINAL
    if (correct2 == 6) {
      uint8_t data[] = "L6";
      rf95.send(data, sizeof(data));
      rf95.waitPacketSent(); 
      Serial.println("sentttttt L6");
    }
    if (correct2 == 7) {
      uint8_t data[] = "L7";
      rf95.send(data, sizeof(data));
      rf95.waitPacketSent(); 
      Serial.println("sentttttt L7");
    }
    if (correct2 == 8) {
      uint8_t data[] = "L8";
      rf95.send(data, sizeof(data));
      rf95.waitPacketSent(); 
      Serial.println("sentttttt L8");
    }
    if (correct2 == 9) {
      uint8_t data[] = "L9";
      rf95.send(data, sizeof(data));
      rf95.waitPacketSent(); 
      Serial.println("sentttttt L9");
    }
   
    int sendTime = millis(); //keeps track of how long game has gone on to be able to decide when to take point for no response
    while ((millis() < 5000 + sendTime) && loops) { //check for 5 seconds whether a button was pressed before changing to a new tile
      uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
      uint8_t len = sizeof(buf);

        if (!digitalRead(controlButton1) and prevControlPressed != 1) {
          Serial.println("button Rando music pressed");
          reset();
          state = "RandoMusic";
          prevControlPressed = 1;
          loops = false;
        }
        if (!digitalRead(controlButton2) and prevControlPressed != 2) {
          reset();
          state = "MultiColorStomp";
          prevControlPressed = 2;
          loops = false;
        }
        if (!digitalRead(controlButton3) and prevControlPressed != 3) {
          reset();
          state = "SingleColorStomp";
          prevControlPressed = 3;
          loops = false;
        }




      
      if (rf95.recv(buf, &len)) { //if receive something
        if (String((char*)buf) == ("B1") and correct1 == 1) { //if we receive a signal that the correct one was jumped on 
          Serial.println("got b1");
          score1++;
          //happy beep
          loops = false; // breaks this game cycle
          //prevTime = millis(); //makes prevTime into curr time
        }
        else if ((String((char*)buf) == ("B2") or String((char*)buf) == ("B3") or String((char*)buf) == ("B4")) and correct1 == 1) { //if wrong pressed //take some points
          score1 = max(0, score1 - 1);
          Serial.println("score decreased bc wrong one pressed");
          //sad beep
          loops = false; //break
        }
        if (String((char*)buf) == ("B2") and correct1 == 2) { //if we receive a signal that the correct one was jumped on 
          Serial.println("got b2");
          score1++;
          //happy beep
          loops = false; // breaks this game cycle
          //prevTime = millis(); //makes prevTime into curr time
        }
        else if ((String((char*)buf) == ("B1") or String((char*)buf) == ("B3") or String((char*)buf) == ("B4")) and correct1 == 2) { //if wrong pressed //take some points
          score1 = max(0, score1 - 1);
          Serial.println("score decreased bc wrong one pressed");
          //sad beep
          loops = false; //break
        }
        if (String((char*)buf) == ("B3") and correct1 == 3) { //if we receive a signal that the correct one was jumped on 
          Serial.println("got b1");
          score1++;
          //happy beep
          loops = false; // breaks this game cycle
          //prevTime = millis(); //makes prevTime into curr time
        }
        else if ((String((char*)buf) == ("B1") or String((char*)buf) == ("B2") or String((char*)buf) == ("B4")) and correct1 == 3) { //if wrong pressed //take some points
          score1 = max(0, score1 - 1);
          Serial.println("score decreased bc wrong one pressed");
          //sad beep
          loops = false; //break
        }
        if (String((char*)buf) == ("B4") and correct1 == 4) { //if we receive a signal that the correct one was jumped on 
          Serial.println("got b1");
          score1++;
          //happy beep
          loops = false; // breaks this game cycle
          //prevTime = millis(); //makes prevTime into curr time
        }
        else if ((String((char*)buf) == ("B1") or String((char*)buf) == ("B2") or String((char*)buf) == ("B3")) and correct1 == 4) { //if wrong pressed //take some points
          score1 = max(0, score1 - 1);
          Serial.println("score decreased bc wrong one pressed");
          //sad beep
          loops = false; //break
        }
//-----------------------------
//THIS IS FOR THE REAL THING

        if (String((char*)buf) == ("B6") and correct2 == 6) { //if we receive a signal that the correct one was jumped on 
          Serial.println("got b1");
          score2++;
          //happy beep
          loops = false; // breaks this game cycle
          //prevTime = millis(); //makes prevTime into curr time
        }
        else if ((String((char*)buf) == ("B7") or String((char*)buf) == ("B8") or String((char*)buf) == ("B9")) and correct2 == 6 ) { //if wrong pressed //take some points
          score2 = max(0, score2 - 1);
          Serial.println("score decreased bc wrong one pressed");
          //sad beep
          loops = false; //break
        }
        if (String((char*)buf) == ("B7") and correct2 == 7) { //if we receive a signal that the correct one was jumped on 
          Serial.println("got b2");
          score2++;
          //happy beep
          loops = false; // breaks this game cycle
          //prevTime = millis(); //makes prevTime into curr time
        }
        else if ((String((char*)buf) == ("B6") or String((char*)buf) == ("B8") or String((char*)buf) == ("B9")) and correct2 == 7) { //if wrong pressed //take some points
          score2 = max(0, score2 - 1);
          Serial.println("score decreased bc wrong one pressed");
          //sad beep
          loops = false; //break
        }
        if (String((char*)buf) == ("B8") and correct2 == 8) { //if we receive a signal that the correct one was jumped on 
          Serial.println("got b1");
          score2++;
          //happy beep
          loops = false; // breaks this game cycle
          //prevTime = millis(); //makes prevTime into curr time
        }
        else if ((String((char*)buf) == ("B6") or String((char*)buf) == ("B7") or String((char*)buf) == ("B9")) and correct2 == 8) { //if wrong pressed //take some points
          score2 = max(0, score2 - 1);
          Serial.println("score decreased bc wrong one pressed");
          //sad beep
          loops = false; //break
        }
        if (String((char*)buf) == ("B9") and correct2 == 9) { //if we receive a signal that the correct one was jumped on 
          Serial.println("got b1");
          score2++;
          //happy beep
          loops = false; // breaks this game cycle
          //prevTime = millis(); //makes prevTime into curr time
        }
        else if ((String((char*)buf) == ("B6") or String((char*)buf) == ("B7") or String((char*)buf) == ("B8")) and correct2 == 9) { //if wrong pressed //take some points
          score2 = max(0, score2 - 1);
          Serial.println("score decreased bc wrong one pressed");
          //sad beep
          loops = false; //break
        }
    }

  }
  //Serial.print("score ");
  //Serial.println(score);
  display.showNumberDec(score1);
  display1.showNumberDec(score2);

  //TIE CONDITION
  //SCORE PANEL UPDATE WITH SCORE
    //win condition
    if (score1 >= 10){
      state = "Won";
      winner = 1;
    }
    if (score2 >= 10) {
      state = "Won";
      winner = 2;
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------
void randoMusic() {
  //if tile pressed ,(recevied a message) play sound and light up
  uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
  uint8_t len = sizeof(buf);
  if (rf95.recv(buf, &len)) { //if receive something
    if (String((char*)buf) == ("B1")) { //if we receive a signal that the correct one was jumped on 
      Serial.println("got b1");
      uint8_t data[] = "L1";
      rf95.send(data, sizeof(data));
      rf95.waitPacketSent(); 
    }
    if (String((char*)buf) == ("B2")) { //if we receive a signal that the correct one was jumped on 
      Serial.println("got b2");
      uint8_t data[] = "L2";
      rf95.send(data, sizeof(data));
      rf95.waitPacketSent(); 
    }
    if (String((char*)buf) == ("B3")) { //if we receive a signal that the correct one was jumped on 
      Serial.println("got b3");
      uint8_t data[] = "L3";
      rf95.send(data, sizeof(data));
      rf95.waitPacketSent(); 
    }
    if (String((char*)buf) == ("B4")) { //if we receive a signal that the correct one was jumped on 
      Serial.println("got b4");
      uint8_t data[] = "L4";
      rf95.send(data, sizeof(data));
      rf95.waitPacketSent(); 
    }
    if (String((char*)buf) == ("B5")) { //if we receive a signal that the correct one was jumped on 
      Serial.println("got b1");
      uint8_t data[] = "L5";
      rf95.send(data, sizeof(data));
      rf95.waitPacketSent(); 
    }
    if (String((char*)buf) == ("B6")) { //if we receive a signal that the correct one was jumped on 
      Serial.println("got b2");
      uint8_t data[] = "L6";
      rf95.send(data, sizeof(data));
      rf95.waitPacketSent(); 
    }
    if (String((char*)buf) == ("B7")) { //if we receive a signal that the correct one was jumped on 
      Serial.println("got b7");
      uint8_t data[] = "L2";
      rf95.send(data, sizeof(data));
      rf95.waitPacketSent(); 
    }
    if (String((char*)buf) == ("B8")) { //if we receive a signal that the correct one was jumped on 
      Serial.println("got b4");
      uint8_t data[] = "L8";
      rf95.send(data, sizeof(data));
      rf95.waitPacketSent(); 
    }
    if (String((char*)buf) == ("B9")) { //if we receive a signal that the correct one was jumped on 
      Serial.println("got b4");
      uint8_t data[] = "L9";
      rf95.send(data, sizeof(data));
      rf95.waitPacketSent(); 
    }
  }
}
