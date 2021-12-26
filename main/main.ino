#include "deviceConnect.h"
#include <SPI.h>
#include <Wire.h>
#include <EEPROM.h>
#include <math.h>
#include "ST7735.h"
#include "LCDbasic.h"
void setup() {
	deviceInit();
	Wire.begin();
	Serial.begin(115200);
	EEPROM.begin(20);
	//SPIClass(VSPI);
	SPI.begin();		//SPI begin
	mySPISettings = SPISettings(40000000, MSBFIRST, SPI_MODE0); //ESP speed /4
}


void loop() {
	char a = readSW();
	digitalWrite(Plate,HIGH);
	delay(1000);
	digitalWrite(Plate,LOW);
	delay(1000);
	LCD_Fill(BLUE);
	if(a=='R'){
		digitalWrite(Plate,HIGH);
	}
	else{
		digitalWrite(Plate,LOW);	
	}
  
}