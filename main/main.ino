
#include "deviceConnect.h"
#include <SPI.h>
#include <math.h>
#include "ST7735.h"
#include "LCDbasic.h"
#include "formation.h"
#include <stdio.h>
#include <stdlib.h>

#define warmingTime		90
#define fluxActiveTime	100	//second
#define fluxActiveTemp	180	//degree
#define reflowTime		80	//second
#define reflowTemp		220	//degree

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
}
int nowTime = millis();
int preTime = nowTime;
int keepTemp = 100;

int mode = 0;
void loop() {
	while(mode==0)	//main screen
	{
	  mainScreen();
		while(true)
		{
			nowTime = millis();
			char inputButton = readSW(false);
			if (inputButton=='L')
			{
				LCD_fill_Rect(1,62,48,17,LGRAY);
				LCD_fill_Rect(51,62,58,17,BLACK);
				LCD_fill_Rect(111,62,48,17,BLACK);
			}
			else if(inputButton == 'M')
			{
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
			else if(inputButton == 'A')
			{
				LCD_fill_Rect(1,62,48,17,LGRAY);
				LCD_fill_Rect(51,62,58,17,LGRAY);
				LCD_fill_Rect(111,62,48,17,BLACK);
			}
			else if(inputButton == 'B')
			{
				LCD_fill_Rect(1,62,48,17,LGRAY);
				LCD_fill_Rect(51,62,58,17,BLACK);
				LCD_fill_Rect(111,62,48,17,LGRAY);
				mode = 2;
			}
			else if(inputButton == 'C')
			{
				LCD_fill_Rect(1,62,48,17,BLACK);
				LCD_fill_Rect(51,62,58,17,LGRAY);
				LCD_fill_Rect(111,62,48,17,LGRAY);
			}
			else if(inputButton == 'D')
			{
				LCD_fill_Rect(1,62,48,17,LGRAY);
				LCD_fill_Rect(51,62,58,17,LGRAY);
				LCD_fill_Rect(111,62,48,17,LGRAY);
			}
			else
			{
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
		int activeTime =0;
		int sequence = 1;
		float nowTemp;
		preTime = nowTime;
		digitalWrite(Plate1,HIGH);	//hot plate off
		while(true)
		{
			nowTime = millis();
			if(nowTime - preTime >= 1000){ //1 sec
				activeTime++;
				preTime = nowTime;
			}
			nowTemp = checkTemp();
			if (activeTime >= 0 && activeTime < warmingTime) //warming time
			{
				if (nowTemp < fluxActiveTemp)
				{
					digitalWrite(Plate1,LOW);	//hot plate on
				}
				else{
					digitalWrite(Plate1,HIGH);	//hot plate off
				}
			}
			else if (activeTime >= warmingTime && activeTime < (warmingTime+fluxActiveTime))//flux active time
			{
				if (nowTemp < fluxActiveTemp)
				{
					digitalWrite(Plate1,LOW);	//hot plate on
				}
				else{
					digitalWrite(Plate1,HIGH);	//hot plate off
				}
			}
			else if (activeTime >= (warmingTime+fluxActiveTime) && activeTime < (warmingTime+fluxActiveTime+reflowTime)) //reflow time
			{
				if (nowTemp < reflowTemp)
				{
					digitalWrite(Plate1,LOW);	//hot plate on
				}
				else{
					digitalWrite(Plate1,HIGH);	//hot plate off
				}
			}
			else{//cooling time
				digitalWrite(Plate1,HIGH);	//hot plate off
			}
			delay(100);
			char cstr[20] = {'\0'};
			sprintf(cstr,"%f",nowTemp);
			LCD_print(5,40,cstr,CYAN,2);
			char inputButton = readSW(true);
			if(inputButton == 'M')
			{
				mode = 0;
			}
			if(mode!=1)
				break;
		}
		
	}
	while(mode==2)  //keep to temperature mode
	{
		keepScreen();
		while(true)
		{
			char inputButton = readSW(true);
			int currentTemp = checkTemp();
			if(keepTemp < currentTemp-10)
			{
				digitalWrite(Plate1,LOW); //on
				LCD_print(40,40,"HEATER ON",RED,1);
			}
			else
			{
				digitalWrite(Plate1,HIGH);  //off
				LCD_print(40,40,"HEATER OFF",RED,1);
			}
			if(inputButton == 'M')
				mode = 0;
			if(mode != 2)
				break;
		}
	}
	while(mode==3)  //SPISettings
	{
		setScreen();
		while(true)
		{
		delay(10);
		}
	}
}
