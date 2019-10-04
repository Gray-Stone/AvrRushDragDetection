
#include "AvrHead.h"
#include "Timer.h"
#include "percisionCheck.h"
#include "userInteraction.h"

void debugIoInit();


volatile int a;

void setup() {
    Serial.begin(115200);
    //sei(); // turn on global interrupt 
  
  // setup all the pins
  timer5PinSetup();
  userIOInit(); 

  debugIoInit();

  timer5Init();
  delay(1000);
  timer5Mute();
  timer3Init();
  PORTB &= 0<<PORTB6;





}


void loop() {
   
  // waitting for user to setup all the paramters
  while(1)
  {
    break; 
    setBPM();
    delay(500);
    Serial.print(" BPM ");
    Serial.print(BPM,DEC);
    
    uint8_t pressed = edgeDetect(PING)  ;
    if (pressed & (1<<PORTG1) ) // record is pressed
    {
      ; 
      // run sd card record function
    }
    else if (pressed & (1<<PORTG2) ) // start is pressed
    {
      Serial.println("started");
      break;
    }
  }

  timer5ParamSet(BPM);

  // finished setup, start the timer
  missInput =0;
  int8_t temp = -101;
  timer5Play();
  while(1)
  {

    // When user input appeared
    if (inputDiff != 0xFFFF)  
    {
      int8_t diff = rdCalculation(); 
      temp = diff;
      // display diff 
      inputDiff = 0xFFFF; //reset the variable, ready for next one. 
      // record diff
    }

    if ( edgeDetect(PING) & (1<<PORTG2) ) { break; }// the start/stop is pressed

  }
  timer5Mute();
  Serial.print(" mc : ");
  Serial.print(missInput,DEC);
  Serial.print("  delay:: ");
  Serial.println(temp,DEC); 


}

void debugIoInit()
{

  //// Pin setup
  //      DDxn 1 = output, 0 = input
  //      PORTxn output: 1 = high, 0 = low; input: 1 = pullup, written 0 to turn off pullup 
  //      Writing a logic one to PINxn toggles the value of PORTxn, independent on the value of DDRxn.
  //      PINxn for reading pin values
 
  // used for debugging
  // PB7 = #13 PB6 = #12  
  DDRB = 1<<DDB7 | 1<<DDB6; //  pin 13 output for general usage
}
