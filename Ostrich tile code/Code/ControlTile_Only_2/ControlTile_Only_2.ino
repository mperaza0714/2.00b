#include <SPI.h>
#include <RH_RF95.h>

//FOR SCOREBOARD
const int CLK = 12; //Set the CLK pin connection to the display
const int DIO = 11; //Set the DIO pin connection to the display
TM1637Display display(CLK, DIO);  //set up the 4-Digit Display.


// for feather m0 RFM9x
#define RFM95_CS 8
#define RFM95_RST 4
#define RFM95_INT 3

// Change to 434.0 or other frequency, must match RX's freq!
#define RF95_FREQ 915.0
// Singleton instance of the radio driver
RH_RF95 rf95(RFM95_CS, RFM95_INT);
int prevTime = 0;
int score = 0;
int correct = 0;
uint8_t data[] = "1";
bool loops = true; //basically a break but ryan hates breaks


void setup() {
  // put your setup code here, to run once:
  pinMode(RFM95_RST, OUTPUT);
  digitalWrite(RFM95_RST, HIGH);

  Serial.begin(115200);
  while (!Serial) {
    delay(1);
  }
    // manual reset
  digitalWrite(RFM95_RST, LOW);
  delay(10);
  digitalWrite(RFM95_RST, HIGH);
  delay(10);

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

  display.setBrightness(0x0a);  //set the diplay to maximum brightness
  
}

void loop() {

  if state = "SingleColorStomp" { 
    singleColorStomp();
  }
  else if state = "MultiColorStomp" {
    multiColorStomp();
  }
  else if state ="RandoMusic" {
    randoMusic();
  }


}

void singleColorStomp() {
      correct = (random(1,3)); //Generates the correct tile to be lit and converts it to a string to be sent
//  correct = 1;
    Serial.print("the random number is: ");
    Serial.println(correct);
        
  //uint8_t data = random(1,9); //Generates the correct tile to be lit and converts it to a string to be sent
  loops = true; // resets looping to true at the beginning of each game cycle (basically break)
//  Serial.println(rf95.available());
//  if (rf95.available()) { //rf95.available()
//    Serial.println("rf95 available");
    
    
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
     
    int sendTime = millis(); //keeps track of how long game has gone on to be able to decide when to take point for no response
    
    while ((millis() < 5000 + sendTime) && loops) {
      
      uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
      uint8_t len = sizeof(buf);
      if (rf95.recv(buf, &len)) {
        

        
        if (String((char*)buf) == ("B1") and correct == 1) { //if we receive a signal that the correct one was jumped on 
          Serial.println("got b1");
          score++;
          //happy beep
          loops = false; // breaks this game cycle
          //prevTime = millis(); //makes prevTime into curr time
        }
        else if (String((char*)buf) != ("B1") and correct == 1) { //if wrong pressed //take some points
          score = max(0, score - 1);
          Serial.println("score decreased bc wrong one pressed");
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
          Serial.println("score decreased bc wrong one pressed");
          //sad beep
          loops = false; //break
          //prevTime = millis(); //makes the prevTime into the current time 
        }

//    }
      
    }
  }
  //Serial.print("score ");
  //Serial.println(score);
  display.showNumberDec(score);
  //SCORE PANEL UPDATE WITH SCORE
}
