
#include "userInteraction.h"

#include "AvrHead.h"
#include "Timer.h"
#include "Arduino.h"
#include "AvrHead.h"

#include "TM1637Display.h"


TM1637Display SegDisplay(SegCLK, SegDIO);

uint16_t BPM=200;
volatile uint8_t LEDwidth = 5; //ms

void userIOInit()
{
	//// Pin setup
  //      DDxn 1 = output, 0 = input
  //      PORTxn output: 1 = high, 0 = low; input: 1 = pullup, written 0 to turn off pullup 
  //      Writing a logic one to PINxn toggles the value of PORTxn, independent on the value of DDRxn.
  //      PINxn for reading pin values
	// pinMode(bpmPotPin,INPUT);

	// for start, record buttons 
	BTNDDR &= ~( (1<<StartBtn) | (1<<RecordBtn) );
	BTNPORT |= (1<<StartBtn) | (1<<RecordBtn) ; // Setup Btn input with pullup.
	edgeDetect(BTNPIN); // flash the values

	// setup for 7-seg display
	pinMode(SegCLK,OUTPUT);
	pinMode(SegDIO,OUTPUT);
	SegDisplay.setBrightness(0x0f);

	DRAGDDR = 0xFF;
	RUSHDDR = 0xFF; 
	DRAGPORT =0;
	RUSHPORT = 0;

	PrefectDDR |= (1<<DDL7); 
	PrefectPort |= (1<<PrefectPin);
}

// * calculate the rush and drag of user input, 
// * directly read data from timer global variable
// * return timming difference in ms. positive for rushing, negative for rushing.    


// takes in a port reading. the pin that has a rising edge will return 1.  
static uint8_t oldValue , presses = 0xFF ;
uint8_t edgeDetect(uint8_t PINx)
{
	
	presses = (~ oldValue) & PINx ; 

	oldValue = PINx ; 
	return presses; 
}


// this function loops
void setBPM()
{	
	uint32_t rawPot= 0 ;
	uint16_t localBPM = 0; 
	for (char i=0 ;i<20;++i)
	{
		rawPot += analogRead(bpmPotPin);
	}
	rawPot/=20;
	localBPM = map(rawPot,10,1022,MINBPM,MAXBPM);

	// Serial.print(rawPot,DEC); Serial.print("  ");  Serial.println(localBPM, DEC); 

	// update timer setting
	if ( timer5ParamSet(localBPM) )
	{
		BPM = localBPM; // only update the global setting if timer setting is successfully updated.  
	}
	// update the display 
	// *use BPM, so a fail timer setting will be rejected on display as well
    // SegDisplay.clear();
    SegDisplay.showNumberDec(BPM, false);
	//********************// 
}

void dispREC()
{
	uint8_t SEG_REC[] = {
	SEG_A | SEG_B | SEG_C | SEG_E | SEG_F | SEG_G,   // R
	SEG_A | SEG_D | SEG_E | SEG_F | SEG_G ,           // E
	SEG_A | SEG_D | SEG_E | SEG_F,                           // C
	SEG_G           									// -
	};
	SegDisplay.clear();
	SegDisplay.setSegments(SEG_REC); 
}

void dispLine()
{
	uint8_t SEG_REC[] = {
	SEG_G,  SEG_G ,	SEG_G,	SEG_G
	};
	SegDisplay.clear();
	SegDisplay.setSegments(SEG_REC); 
}

// Takes the rushing or dragging port, 
// how many ms from center 
// (0 for inside clear zone, and direction is ignored)
void RDDisp(int8_t direction ,uint8_t ms)
{
	// Px0 is LED #8 
	// Px7 is LED #1 
	DRAGPORT =0xFF; 
	RUSHPORT = 0xFF; // turn all off
	PrefectPort &= ~(1<<PrefectPin);

	if (ms==0)
	{
		PrefectPort |= (1<<PrefectPin); // Turn on Prefect LED
		return ;
	}
	if (direction>0 )
	{
	 	RUSHPORT = ~( 0b10000000 >> ((ms-1)/LEDwidth )) ; // pin is connected to negative of LED, pull low to turn on.  
		return ;
	}
	if (direction <0)
	{
		DRAGPORT = ~( 0b10000000 >> ((ms-1)/LEDwidth) ) ; 
		return ; 
	}
	
}