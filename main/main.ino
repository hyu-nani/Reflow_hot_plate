
#include "deviceConnect.h"
#include <SPI.h>
#include <math.h>
#include "ST7735.h"
#include "LCDbasic.h"
#include "formation.h"

void setup() {
	deviceInit();
	pinMode(LED_BUILTIN, OUTPUT);
	digitalWrite(LED_BUILTIN, LOW);
	SPIClass(HSPI);
	SPI.begin();		//SPI begin
	mySPISettings = SPISettings(40000000, MSBFIRST, SPI_MODE0); //ESP speed /4
	LCD_Init();
	LCD_Fill(BLACK);
	LCD_off();
	LCD_Fill(WHITE);
	LCD_image(25,0,136,80,logo);
	LCD_on(10);
	delay(2000);
	int timeNow = millis(); 
}


void loop() {
	int count=1;
	mainScreen();
	while(1){
		char inputButton = readSW();
		if (inputButton=='L'){
			LCD_fill_Rect(1,62,48,17,MAGENTA);
			LCD_fill_Rect(51,62,58,17,BLACK);
			LCD_fill_Rect(111,62,48,17,BLACK);
		}
		else if(inputButton == 'M'){
			LCD_fill_Rect(1,62,48,17,BLACK);
			LCD_fill_Rect(51,62,58,17,MAGENTA);
			LCD_fill_Rect(111,62,48,17,BLACK);
		}
		else if(inputButton == 'R'){
			LCD_fill_Rect(1,62,48,17,BLACK);
			LCD_fill_Rect(51,62,58,17,BLACK);
			LCD_fill_Rect(111,62,48,17,MAGENTA);
		}
		else if(inputButton == 'A'){
			LCD_fill_Rect(1,62,48,17,MAGENTA);
			LCD_fill_Rect(51,62,58,17,MAGENTA);
			LCD_fill_Rect(111,62,48,17,BLACK);
		}
		else if(inputButton == 'B'){
			LCD_fill_Rect(1,62,48,17,MAGENTA);
			LCD_fill_Rect(51,62,58,17,BLACK);
			LCD_fill_Rect(111,62,48,17,MAGENTA);
		}
		else if(inputButton == 'C'){
			LCD_fill_Rect(1,62,48,17,BLACK);
			LCD_fill_Rect(51,62,58,17,MAGENTA);
			LCD_fill_Rect(111,62,48,17,MAGENTA);
		}
		else if(inputButton == 'D'){
			LCD_fill_Rect(1,62,48,17,MAGENTA);
			LCD_fill_Rect(51,62,58,17,MAGENTA);
			LCD_fill_Rect(111,62,48,17,MAGENTA);
		}
		else{
			LCD_fill_Rect(1,62,48,17,BLACK);
			LCD_fill_Rect(51,62,58,17,BLACK);
			LCD_fill_Rect(111,62,48,17,BLACK);
		}
		
		int yran = random(11,59);
		LCD_Pixel(count,yran,1,BLUE);
		count++;
		if(count>=159){
			LCD_fill_Rect(1,11,158,48,BLACK);
			LCD_print(135,11,"TEMP",GREEN,1);
			count = 1; 
		}
		delay(5);
	}
	
}
