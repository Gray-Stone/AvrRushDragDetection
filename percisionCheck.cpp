
#include "percisionCheck.h"
#include "AvrHead.h"

#include<stdint.h>
#include<Arduino.h>

#include "Timer.h"

void mathCheck()
{
    Serial.begin(115200);
    uint16_t bpm=0;
    for (bpm = 30;bpm<60;bpm++)
    {
        timer5ParamSet(bpm);
        Serial.println(OCR5A+1);
    }
    Serial.println();
    for (bpm = 60;bpm<240;bpm++)
    {
        timer5ParamSet(bpm);
        Serial.println(OCR5A+1);
    }

    Serial.println();
    for (bpm = 240;bpm<400;bpm++)
    {
        timer5ParamSet(bpm);
        Serial.println(OCR5A+1);
    }
}

void typeLengthCheck()
{   
    Serial.begin(115200);

    // uint64_t numU64 = 0;
    uint32_t numU32 = 0;
    uint16_t numU16 = 0;


     Serial.println("size of");

    // Serial.print("uint64_t");   Serial.println( sizeof(numU64) , DEC ); 
    Serial.print("uint32_t");   Serial.println( sizeof(numU32) , DEC ); 
    Serial.print("uint16_t");   Serial.println( sizeof(numU16) , DEC ); 

    Serial.println("Max number");

    // Serial.print("uint64_t");   Serial.println( (numU64-1) , DEC ); 
    Serial.print("uint32_t");   Serial.println( (numU32-1) , DEC ); 
    Serial.print("uint16_t");   Serial.println( (numU16-1) , DEC ); 

}

void calTimeCheck()
{
    uint16_t tStart,tEnd ;
    uint32_t tAvg;
    volatile uint16_t x =0;
    volatile int i ;

    timer3Init(); // make sure the timer 4 used is in 1/64 prescale 250 count per 1ms
    Serial.begin(115200);

    tAvg=0;
    for (int aa  = 0 ; aa <100 ; ++aa  )
    {   
        tStart = TCNT3;

        PORTB |= 1 << PORTB7;
        // function under test
        timer5ParamSet(i+50);
        PORTB &= ~ (1 << PORTB7);
    
        tEnd = TCNT3;
        Serial.println( (tEnd-tStart), DEC); 
        tAvg += (tEnd-tStart)  ;  // get the average; 
    }
        tAvg/=100 ;
    Serial.print("tick average Timer4 is : ");
    Serial.println(tAvg, DEC); 
}