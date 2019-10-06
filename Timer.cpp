
#include "AvrHead.h"
#include "Timer.h"
#include "Arduino.h"

//////////// globals //////////////

uint8_t cpmsTop = 250; // count per ms 
uint8_t cpmsBtm = 1;
uint32_t count = 25000;

volatile uint16_t inputDiff =0xFFFF; // since the period is always smaller then 0xFFFF, a 0xFFFF means no input
volatile uint8_t missInput = 0; // indicate if there is a missed input
uint16_t halfPeriod =25000 ; // this is to speed up calculation

volatile uint16_t beatNumber =0; // record how many beat played reset on start 

volatile uint8_t logArray[LOGSize] = {0} ;
volatile uint16_t logIndex =0;

// * Setup I/O pins used by timer5
void timer5PinSetup()
{
	DDRL |= (1<<DDL4) ;
  DDRL |= (1<<DDL5) ; // optional for OC5C output
	DDRL &= ~(1<<DDL1) ;  // setup input pin
}

// * Setup the registers for timer5 with output compare and input capture. 
// * BPM initially setup to be 300BPM
void timer5Init()
{ 
  //// Timer Setup: Timer 5 Fast PWM mode with OCR5A as top, input capture, and OCR5B as output
  // WGM5 3:0 = 0b1111 :  Fast PWM TOP:OCRnA
  // COM5B 1:0 = 0b10 : OC5B set at Button, clear on match
  // ICNC5 = 0 turn off noise canceler (avoid latency)
  // ICES5 = 0b1 : trigger on rising edge
  // CS5 2:0 = 0b1 to 0b101 for CLKi/o with prescaler (1 for no prescaler)
  // ICIE5 = 1 : enable interrupt (for capture) when ICF1 is set
  // TOIE5 = 1 : over flow interrupt enable (for counting how many cycles)

  //R//   OCR5A (H/L) for top value
  //R//   OCR5B (H/L) for output clear value           =>>>> OC5B = PL4 = #45 
  //R//   OCR5C (H/L) for generate fake intput         =>>>> OC5C = PL5 = #44
  //R//   ICR5 (H/L) storing capture value             =>>>> ICP5 = PL1 = #48
  //R//   TCCR5A -->> COM5B1:0 WGM51:0
  //R//   TCCR5B -->> ICNC5 ICES5 WGM53:2 CS52:0
  //R//   TIMSK5 -->> ICIE5 TOIE5 




  //R// OC5B =>> PL4 output DDL4=1. Overridding signals for alt-fun: PVOV PVOE  OC5B enable
  //R// ICP5 =>> PL1  . Overriding signals for alt-fun: DI   ICP5 enable

  TCCR5A =  (0b10<<COM5A0) | (0b10<<COM5B0) | (0b11<<WGM50);
  TCCR5A |= (0b11<<COM5C0) ; //additional COMP C output for testing
  TCCR5B =  0<<ICNC5 | 1<<ICES5 | 0b11<<WGM52 | (0b11<CS50) ; // preserve the timer. 
  TIMSK5 = (1<<ICIE5) | (1<<TOIE5);

  // 0b10   1/8   prescaler,  = 2MHz; 0.5us per count
  // 0b11   1/64  prescaler,  = 250Khz; 4us per count     ; 250 count per 1ms  
  // 0b100  1/256 prescaler,  = 62.5Khz; 16us per count   ; 125 count per 2ms
  // 0b101  1/1024 prescaler, = 15.625Khz; 64us per count ; 125 count per 8ms
  OCR5A = 50000;  // 1/64 with 200ms period
  OCR5B = OCR5A /10;  // 5% duty
  
}

void timer5Mute()
{
  TIMSK5=0;
  TCCR5A &= ~ (0b1111<<COM5B0); // clear output
  TCCR5A &= ~(0b11<<COM5C0);
  TCCR5B &= ~ (1<<ICES5);
}

void timer5Play()
{
  // OCR5C = OCR5A - 65 ; // set to about 2 ms 
  OCR5C = 150; // set to about 2 ms 
  beatNumber =0; 
  TCCR5A |=  (0b10<<COM5A0) | (0b10<<COM5B0);
  TCCR5A |= (0b11<<COM5C0);
  TCCR5B |=  0<<ICNC5 | 1<<ICES5 ;
  TIMSK5 = (1<<ICIE5) | (1<<TOIE5);
}

// * set the timer5 paramters base on needed bpm range:[10 -700]
// * takes bpm, return 0 for fail, 1 for succed
uint8_t timer5ParamSet(uint16_t bpm) // measured to be 88us to run
{
  // case of out of range bpm 
  if ( (bpm < 10) || (bpm > 700 ) ) { return 0 ; }



  // X BPM, Y count per ms
  // 60*1000*Y/X = count per beat 
  // use 256 prescaler for 60BPM and above, 1024 for below. 

  // count per ms is not round, split into top and buttom


  if( bpm<60 ) // use prescaler of 1024 below 60bpm
  {
    TCCR5B &= ~(0b111<<CS50) ; 
    TCCR5B |= (0b101<<CS50) ; 
    cpmsTop = 125;
    cpmsBtm = 8;
  }
  else if (bpm < 240 ) // use prescaler of 256 from 60-240bpm
  {
    TCCR5B &= ~(0b111<<CS50) ; 
    TCCR5B |= (0b100<<CS50) ; 
    cpmsTop = 125;
    cpmsBtm = 2;
  }
  else              // use prescaler of 64 above 240bpm
  {
    TCCR5B &= ~(0b111<<CS50) ; 
    TCCR5B |= (0b011<<CS50) ; 
    cpmsTop = 250;
    cpmsBtm = 1;
  }

  // calculate counter top value
  count = cpmsTop;
  count = count * 60000 ;
  count = count / (uint32_t) cpmsBtm ;
  count = count / bpm ;
  OCR5A = (uint16_t) (count-1);
  halfPeriod = (uint16_t) count/2;
  return 1;
}


// timer3 for general delay use
void timer3Init()
{ 
  //// Timer Setup: Timer 4 Fast PWM mode for performance measure
  
  TCCR3A = 0 ;
  TCCR3B = (0b01<<WGM32) | (0b11<<CS30);  // top at OCR3A 0b11   1/64  prescaler, 4us per count; 250 count per 1ms  
  OCR3A = 0xFFFF; // default run to max. 

}

void timer3Off()
{
  TCCR3B =0; // turn of CS, turn off timer. 
}

uint8_t timer3Overflow()  // period is 261.14 ms
{
  return (TIFR3 & (1<<TOV3)) ;
}

void timer3OverflowClear()
{
  TIFR3 |=  1<<TOV3;
}

/////////////////////// ISRs ///////////////////////

ISR(TIMER5_CAPT_vect)
{
  PORTB ^= 1<<PORTB7;

 // to prevent next input value destory previous one
  if (inputDiff != 0xFFFF)
  {
    ++ missInput;
    return ; 
  }

  inputDiff = ICR5 ; 
}

ISR(TIMER5_COMPB_vect)
{
  ;
}

ISR(TIMER5_OVF_vect)
{
  ++beatNumber;
}
