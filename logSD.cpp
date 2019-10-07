
#include <SPI.h>
#include <SD.h>
#include "arduino.h"

#include "logSD.h"
#include "Timer.h"
#include "userInteraction.h"

// set up variables using the SD utility library functions:
#define chipSelect 53


bool SDInit()
{
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
	return 0; 
  }
  Serial.println("card initialized.");
  return 1;
}

bool SDDump()
{
	String fileName = "log01.csv" ;
	// change this to go though files and find a not used file number 
	// while ( SD.exists("SDFile.txt") ) 
	// {
	// 	 ;
	// }
  
 	File dataFile = SD.open("RDrecord.csv", FILE_WRITE);

	// if the file is available, write to it:
	if ( ! dataFile) 
	{
		Serial.println("fail to open file") ; 
		 return 0;
	}

	Serial.println("csv available");

	
	String info = "BPM : " + String(BPM) ; 
	dataFile.println(info);
	dataFile.println("Rushing is positive Dragging is Negative");
	dataFile.println("Beat # , Rush/Drag ");	// header for data

	for (uint16_t i=0; i<logIndex-2 ;i+=3)
	{
		String data = String( (logArray[i]<<8) + logArray[i+1] ) + "," + String( (int8_t) (logArray[i+2]) ) ; 
		dataFile.println(data);
	}
	dataFile.close();

	
}