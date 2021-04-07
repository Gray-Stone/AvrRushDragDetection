
Rushing Dragging:

	// A simple device that detects if the input notes are rushing or dragging compare to the metronome (internal) 
	// Analog frontend is not included in this project, the input notes need to be properly shaped to be a short pulse. The device is set to detect rising edge. 


Data Convension: 
	Rushing is positive, Dragging is Negative

PIN Assignment: 

	SD card #50:53 (PB3:0) (MISO,MOSI,SCK,SS)
	Timer:
		Metronome tick: #45 (PL4 -> OC5B) ==>> Also used to drive metronome LED
		Fake lagged input #44 (PL5 -> OC5C) ==>> could be wired to user signal input for self test
		User Signal input #48 (PL1 -> ICP5)

	User Interface:
		Metronome Pot: 	ADC15 (PK7)
		Metronome LEDs: #18:19 (PD2:3) (CLK,DIO)
		Start Btn : 	#17 (PH0)
		Aux Btn: 	#16 (PH1) 
			-->> currently called the record button in code. 
			-->> record should be changed to everything after play.
			-->> Aux Btn should be emptied for something else. 

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


