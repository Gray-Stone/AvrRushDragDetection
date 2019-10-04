
void timer5PinSetup();
uint8_t timer5ParamSet(uint16_t bpm);
void timer5Init();
void  timer5Mute();
void timer5Play();

void timer3Init();
void timer3Off();
uint8_t timer3Overflow();
void timer3OverflowClear();

extern uint8_t cpmsTop ; 
extern uint8_t cpmsBtm ;

extern volatile uint16_t inputDiff ;
extern uint16_t halfPeriod ;
extern uint32_t count;
extern volatile uint8_t missInput ; 

