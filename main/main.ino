
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
	LCD_display_OFF();
	LCD_Fill(WHITE);
	LCD_image(25,0,136,80,logo);
	LCD_display_ON();
	delay(2000);
	int timeNow = millis(); 
}

int mode = 0;

void loop() {
	mainScreen();
	
	while(mode==0)	//main screen
	{
		while(1){
			char inputButton = readSW(false);
			if (inputButton=='L'){
				LCD_fill_Rect(1,62,48,17,LGRAY);
				LCD_fill_Rect(51,62,58,17,BLACK);
				LCD_fill_Rect(111,62,48,17,BLACK);
			}
			else if(inputButton == 'M'){
				LCD_fill_Rect(1,62,48,17,BLACK);
				LCD_fill_Rect(51,62,58,17,LGRAY);
				LCD_fill_Rect(111,62,48,17,BLACK);
				mode = 1;
			}
			else if(inputButton == 'R'){
				LCD_fill_Rect(1,62,48,17,BLACK);
				LCD_fill_Rect(51,62,58,17,BLACK);
				LCD_fill_Rect(111,62,48,17,LGRAY);
			}
			else if(inputButton == 'A'){
				LCD_fill_Rect(1,62,48,17,LGRAY);
				LCD_fill_Rect(51,62,58,17,LGRAY);
				LCD_fill_Rect(111,62,48,17,BLACK);
			}
			else if(inputButton == 'B'){
				LCD_fill_Rect(1,62,48,17,LGRAY);
				LCD_fill_Rect(51,62,58,17,BLACK);
				LCD_fill_Rect(111,62,48,17,LGRAY);
			}
			else if(inputButton == 'C'){
				LCD_fill_Rect(1,62,48,17,BLACK);
				LCD_fill_Rect(51,62,58,17,LGRAY);
				LCD_fill_Rect(111,62,48,17,LGRAY);
			}
			else if(inputButton == 'D'){
				LCD_fill_Rect(1,62,48,17,LGRAY);
				LCD_fill_Rect(51,62,58,17,LGRAY);
				LCD_fill_Rect(111,62,48,17,LGRAY);
			}
			else{
				LCD_fill_Rect(1,62,48,17,BLACK);
				LCD_fill_Rect(51,62,58,17,BLACK);
				LCD_fill_Rect(111,62,48,17,BLACK);
			}
			LCD_print(18,66,"<-",WHITE,1);
			LCD_print(65,66,"Start",WHITE,1);
			LCD_print(128,66,"->",WHITE,1);
			int tempBar = map(checkTemp(),-50,105,2,58);
			if(tempBar>58)
			tempBar = 58;
			else if(tempBar<2)
			tempBar = 2;
			LCD_Line(151,tempBar-1,158,tempBar-1,1,BLACK);
			LCD_Line(151,tempBar,158,tempBar,1,YELLOW);
			LCD_Line(151,tempBar+1,158,tempBar+1,1,BLACK);
			delay(1);
			if(mode!=0)
				break;
			digitalWrite(Plate1,HIGH);
		}
	}
	while(mode==1)	//soldering start
	{
		startScreen();
		while(1){
			digitalWrite(Plate1,LOW);
			delay(100);
			char inputButton = readSW(true);
			if(inputButton=='M'){
				mode = 0;
			}
			if(mode!=1)
				break;
		}
		
	}
	
}
