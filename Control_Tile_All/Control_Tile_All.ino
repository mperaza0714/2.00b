#include <SPI.h>
#include <RH_RF95.h>

// for feather m0 RFM9x
#define RFM95_CS 8
#define RFM95_RST 4
#define RFM95_INT 3

//#if defined(ESP8266)
//  /* for ESP w/featherwing */ 
//  #define RFM95_CS  2    // "E"
//  #define RFM95_RST 16   // "D"
//  #define RFM95_INT 15   // "B"
//
//#elif defined(ESP32)  
//  /* ESP32 feather w/wing */
//  #define RFM95_RST     27   // "A"
//  #define RFM95_CS      33   // "B"
//  #define RFM95_INT     12   //  next to A
//
//#elif defined(NRF52)  
//  /* nRF52832 feather w/wing */
//  #define RFM95_RST     7   // "A"
//  #define RFM95_CS      11   // "B"
//  #define RFM95_INT     31   // "C"
//  
//#elif defined(TEENSYDUINO)
//  /* Teensy 3.x w/wing */
//  #define RFM95_RST     9   // "A"
//  #define RFM95_CS      10   // "B"
//  #define RFM95_INT     4    // "C"
//#endif

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
  Serial.println("power");
}

void loop() {


//ADD WIN CONDITION



  
    correct = (random(1,10)); //Generates the correct tile to be lit and converts it to a string to be sent
//  correct = 1;
    Serial.print("the random number is: ");
    Serial.println(correct);
        
  loops = true; // resets looping to true at the beginning of each game cycle (basically break)
//  Serial.println(rf95.available());
  if (1) {//rf95.available()
//    Serial.println("rf95 available");
    
    
    //Broadcasts the number of the tile that needs to light up
    if (correct == 1) {
      uint8_t data[] = "L1";
      rf95.send(data, sizeof(data));
      rf95.waitPacketSent(); 
    }
    if (correct == 2) {
      uint8_t data[] = "L2";
      rf95.send(data, sizeof(data));
      rf95.waitPacketSent(); 
    }
    if (correct == 3) {
      uint8_t data[] = "L3";
      rf95.send(data, sizeof(data));
      rf95.waitPacketSent(); 
    }
    if (correct == 4) {
      uint8_t data[] = "L4";
      rf95.send(data, sizeof(data));
      rf95.waitPacketSent(); 
    }
    if (correct == 5) {
      uint8_t data[] = "L5";
      rf95.send(data, sizeof(data));
      rf95.waitPacketSent(); 
    }
    if (correct == 6) {
      uint8_t data[] = "L2";
      rf95.send(data, sizeof(data));
      rf95.waitPacketSent(); 
    }
    if (correct == 7) {
      uint8_t data[] = "L7";
      rf95.send(data, sizeof(data));
      rf95.waitPacketSent(); 
    }
    if (correct == 8) {
      uint8_t data[] = "L8";
      rf95.send(data, sizeof(data));
      rf95.waitPacketSent(); 
    }
    if (correct == 9) {
      uint8_t data[] = "L9";
      rf95.send(data, sizeof(data));
      rf95.waitPacketSent(); 
    }
     
    int sendTime = millis(); //keeps track of how long game has gone on to be able to decide when to take point for no response

    while ((millis() < 5000 + sendTime) && loops) {
      
      uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
      uint8_t len = sizeof(buf);
      if (rf95.recv(buf, &len)) { // not alwaeys receiving that the button has been pressed????????????????
        

        
        if (String((char*)buf) == ("B1") and correct == 1) { //if we receive a signal that the correct one was jumped on 
          Serial.println("got b1");
          score++;
          //happy beep
          loops = false; // breaks this game cycle
        }
        else if (String((char*)buf) != ("B1") and correct == 1) { //if wrong pressed //take some points //fix later, assumes 1 is the correct one ??????????????????????
          score = max(0, score - 1);
          Serial.println("score decreased bc wrong one pressed");
          //sad beep
          loops = false; //break
        }


        if (String((char*)buf) == ("B2") and correct == 2) { //if we receive a signal that the correct one was jumped on 
          Serial.println("got b2");
          score++;
          //happy beep
          loops = false; // breaks this game cycle
        }
        else if (String((char*)buf) != ("B2") and correct == 2) { //if wrong pressed //take some points //fix later, assumes 1 is the correct one ??????????????????????
          score = max(0, score - 1);
          Serial.println("score decreased bc wrong one pressed");
          //sad beep
          loops = false; //break
        }



        if (String((char*)buf) == ("B3") and correct == 3) { //if we receive a signal that the correct one was jumped on 
          Serial.println("got b3");
          score++;
          //happy beep
          loops = false; // breaks this game cycle
        }
        else if (String((char*)buf) != ("B3") and correct == 3) { //if wrong pressed //take some points //fix later, assumes 1 is the correct one ??????????????????????
          score = max(0, score - 1);
          Serial.println("score decreased bc wrong one pressed");
          //sad beep
          loops = false; //break
        }



        if (String((char*)buf) == ("B4") and correct == 4) { //if we receive a signal that the correct one was jumped on 
          Serial.println("got b4");
          score++;
          //happy beep
          loops = false; // breaks this game cycle
        }
        else if (String((char*)buf) != ("B4") and correct == 4) { //if wrong pressed //take some points //fix later, assumes 1 is the correct one ??????????????????????
          score = max(0, score - 1);
          Serial.println("score decreased bc wrong one pressed");
          //sad beep
          loops = false; //break
        }



        if (String((char*)buf) == ("B5") and correct == 5) { //if we receive a signal that the correct one was jumped on 
          Serial.println("got b5");
          score++;
          //happy beep
          loops = false; // breaks this game cycle
        }
        else if (String((char*)buf) != ("B5") and correct == 5) { //if wrong pressed //take some points //fix later, assumes 1 is the correct one ??????????????????????
          score = max(0, score - 1);
          Serial.println("score decreased bc wrong one pressed");
          //sad beep
          loops = false; //break
        }


        if (String((char*)buf) == ("B6") and correct == 6) { //if we receive a signal that the correct one was jumped on 
          Serial.println("got b6");
          score++;
          //happy beep
          loops = false; // breaks this game cycle
        }
        else if (String((char*)buf) != ("B6") and correct == 6) { //if wrong pressed //take some points //fix later, assumes 1 is the correct one ??????????????????????
          score = max(0, score - 1);
          Serial.println("score decreased bc wrong one pressed");
          //sad beep
          loops = false; //break
        }



        if (String((char*)buf) == ("B7") and correct == 7) { //if we receive a signal that the correct one was jumped on 
          Serial.println("got b7");
          score++;
          //happy beep
          loops = false; // breaks this game cycle
        }
        else if (String((char*)buf) != ("B7") and correct == 7) { //if wrong pressed //take some points //fix later, assumes 1 is the correct one ??????????????????????
          score = max(0, score - 1);
          Serial.println("score decreased bc wrong one pressed");
          //sad beep
          loops = false; //break
        }


        if (String((char*)buf) == ("B8") and correct == 8) { //if we receive a signal that the correct one was jumped on 
          Serial.println("got b8");
          score++;
          //happy beep
          loops = false; // breaks this game cycle
        }
        else if (String((char*)buf) != ("B8") and correct == 8) { //if wrong pressed //take some points //fix later, assumes 1 is the correct one ??????????????????????
          score = max(0, score - 1);
          Serial.println("score decreased bc wrong one pressed");
          //sad beep
          loops = false; //break
        }



        if (String((char*)buf) == ("B9") and correct == 9) { //if we receive a signal that the correct one was jumped on 
          Serial.println("got b9");
          score++;
          //happy beep
          loops = false; // breaks this game cycle
        }
        else if (String((char*)buf) != ("B9") and correct == 9) { //if wrong pressed //take some points //fix later, assumes 1 is the correct one ??????????????????????
          score = max(0, score - 1);
          Serial.println("score decreased bc wrong one pressed");
          //sad beep
          loops = false; //break
        }
        
      }
    }
  }
  Serial.print("score ");
  Serial.println(score);
  //SCORE PANEL UPDATE WITH SCORE
}
