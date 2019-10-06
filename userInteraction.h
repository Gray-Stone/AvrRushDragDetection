#include<stdint.h>


void setBPM();
void userIOInit();
int8_t rdCalculation();
uint8_t edgeDetect(uint8_t PINx);
void dispREC();
void dispLine();


extern uint16_t BPM;

#define MAXBPM 400
#define MINBPM 30

#define bpmPotPin A15
// PH0 -> #17  startStop button
// PH1 -> #16  button for recording
// PA0:7 #22:29 LED for rushing
// PC7:0 #30:37 LED for dragging
#define SegCLK 18
#define SegDIO 19

#define StartBtn PORTH0
#define RecordBtn PORTH1
#define BTNPORT PORTH
#define BTNPIN PINH
#define BTNDDR DDRH

#define RUSHPORT PORTA
#define RUSHDDR DDRA

#define DRAGPORT PORTC
#define DRAGDDR DDRC

