
#include "AvrHead.h"
#include "Timer.h"
#include "percisionCheck.h"
#include "userInteraction.h"
#include "logSD.h"

// set up variables using the SD utility library functions:
#define chipSelect 53




void debugIoInit();

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

  SDInit();

}


void loop() {
   
  // ---------------- waitting for user to setup all the paramters
  while(1)
  {
    setBPM();
    Serial.print(" BPM ");
    Serial.println(BPM,DEC);
    delay(100);
    
    uint8_t pressed = edgeDetect(BTNPIN)  ;

    // maybe move this to everytime the device is stopped, this btn can then be used to set delay per division
    if (pressed & (1<<RecordBtn) ) // record is pressed
    {
      dispREC();
      if( SDInit() ) { SDDump(); } 
      delay(1000);
      // run sd card record function
    }
    else if (pressed & (1<<StartBtn) ) // start is pressed
    {
      Serial.println("started");
      break;
    }
  }

  //---------------- finished setup, start the timer ------------------
  timer5ParamSet(BPM);
  Serial.println("---Start Playing---");

  missInput =0;
  logIndex=0;
  dispLine();
  delay(500);

  timer5Play();
  while(1)
  {

    // When user input appeared
    if (inputDiff != 0xFFFF)  
    {
      OCR5C+=10;
      int8_t diff = rdCalculation(); 
     
      // display diff 
      Serial.println(diff,DEC); 
      inputDiff = 0xFFFF; //reset the variable, ready for next one. 
    
      // record diff
      if (logIndex < LOGSize -2)  // only record when there's enough space
      {
        logArray[logIndex++] =  (uint8_t) ( (beatNumber>>8) & 0xff ) ; 
        logArray[logIndex++] =  (uint8_t) ( (beatNumber) & 0xff ) ;  
        logArray[logIndex++] =  (uint8_t) ( diff ) ;  
      }
      else // log overflowed Show something
      {
        ;
      }
    }

    if ( edgeDetect(BTNPIN) & (1<<StartBtn) ) { break; }// the start/stop is pressed

  }
  timer5Mute();
  Serial.println("---Done Playing---");
  dispLine();
  delay(500);

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
