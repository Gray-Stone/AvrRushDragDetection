
#include "userInteraction.h"

#include "AvrHead.h"
#include "Timer.h"
#include "Arduino.h"
#include "AvrHead.h"

#include "TM1637Display.h"


TM1637Display SegDisplay(SegCLK, SegDIO);

uint16_t BPM=200;

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

}

// * calculate the rush and drag of user input, 
// * directly read data from timer global variable
// * return timming difference in ms. positive for rushing, negative for rushing.    
int8_t rdCalculation()
{

	// maybe allow output of 0.1ms 
	int16_t diff =0; // diff (count) * / Y cpms = diff (ms) 
	// case of dragging
	if (inputDiff > halfPeriod)
	{
		diff = inputDiff - count ; // this should give a negative number 
	} 
	else // case of rushing
	{
		diff = inputDiff; // this should give a positive number
	}
	// TODO: make sure no over flow will happen
	// TODO: currently within first 1ms each direction will be return as 0. Maybe this needs to be tighter 
	diff = (diff * cpmsBtm ) / cpmsTop ;
	return (int8_t) diff ; 
}

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