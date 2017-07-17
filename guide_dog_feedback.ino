#include <IRremote.h> // Include the IRremote library

/*button consts*/
const uint16_t BUTTON_A = 0xF807;
const uint16_t BUTTON_B = 0x7887;
const uint16_t BUTTON_C = 0x58A7;
const uint16_t BUTTON_LEFT = 0x10EF;
const uint16_t BUTTON_RIGHT = 0x807F;
const uint16_t BUTTON_CENTER = 0x20DF;

/*Pins*/
const int RECV_PIN = 18;
const int OUT_LEFT = 9;
const int OUT_RIGHT = 5;

/* Feedbacj Consts */
const int FEEDBACK_INTENSITYS[] = {100, 150, 210};

/*globals*/
int intensitySetting = 0;
IRrecv irrecv(RECV_PIN);
decode_results results;
long lastIrResult = 0;

void setup(){
  Serial.begin(9600); // Use serial to debug. 
  Serial.println("System On");
  
  pinMode(RECV_PIN, INPUT);
  pinMode(OUT_LEFT, OUTPUT);
  pinMode(OUT_RIGHT, OUTPUT);

  // Start the receiver
  irrecv.enableIRIn(); 
}

void buzz(boolean left, boolean right){
  if(right){
      analogWrite(OUT_RIGHT, FEEDBACK_INTENSITYS[intensitySetting] );
  }else{
      analogWrite(OUT_RIGHT, 0);
  }
  if(left){
      analogWrite(OUT_LEFT, FEEDBACK_INTENSITYS[intensitySetting] );
  }else{
      analogWrite(OUT_LEFT, 0);
  }
}

void loop() {
  if (irrecv.decode(&results)){
      uint16_t resultCode = (results.value & 0xFFFF);
      /*remote spits out 0xfff if button is being held down*/
      if (resultCode != 0xFFFF){
        /*Handle button result*/
        switch (resultCode){
          case BUTTON_A:
            Serial.println("A");
            intensitySetting = 0;
            break;
          case BUTTON_B:
            Serial.println("B");
            intensitySetting = 1;
            break;
          case BUTTON_C:
            Serial.println("C");
            intensitySetting = 2;
            break;
          case BUTTON_LEFT:
            Serial.println("LEFT");
            buzz(false,false);
            buzz(true,false);
            break;
          case BUTTON_RIGHT:
            Serial.println("RIGHT");
            buzz(false,false);
            buzz(false, true);
            break;
          case BUTTON_CENTER:
            Serial.println("CENTER");
            buzz(true,true);
            break; 
        }
      }else{
        Serial.println("repeat results");
      }
      //store current time
      lastIrResult = millis();
      //resume ir reading
      irrecv.resume(); 
  }else{
      //check to see if a sum of time has elapsed scince last response from ir module
      if(millis() - lastIrResult > 300){
        buzz(false,false);
      }
  }
}
