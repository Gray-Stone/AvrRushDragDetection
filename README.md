
Rushing Dragging:

	//describ the device


Data Convension: 
	Rushing is positive, Dragging is Negative

PIN Assignment: 

	SD card #50:53 (PB3:0) (MISO,MOSI,SCK,SS)
	Timer:
		Metronome tick: #45 (PL4 -> OC5B) ==>> Also used to drive metronome LED
		Fake lagged input #44 (PL5 -> OC5C) 
		User Signal input #48 (PL1 -> ICP5)

	User Interface:
		Metronome Pot: 	ADC15 (PK7)
		Metronome LEDs: #18:19 (PD2:3) (CLK,DIO)
		Start Btn : 	#17 (PH0)
		Record Btn: 	#16 (PH1)

	RD LEDs
		Rushing: #29:22 (PA7:0) (#22,PA0 most rush)
		Dragging: #30:37 (PC7:0) (#37,PC0, most drag) 
		Prefect: #42 (PL7)

	External Component: 

	SD card reader
	10 Seg LED bar x2 
	push BTM x2 
	Pot 
	TM1637 4 Digit 7-Sig

	Special Code library:
		internal:
			<avr/io.h>
			<avr/interrupt.h>
			<stdint.h>
		External: 
			TM1637 Libiary (included in subfolder)

Work for futures: 


