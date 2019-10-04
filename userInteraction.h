#include<stdint.h>


void setBPM();
void userIOInit();
int8_t rdCalculation();
uint8_t edgeDetect(uint8_t PINx);


extern uint16_t BPM;

#define MAXBPM 400
#define MINBPM 30
