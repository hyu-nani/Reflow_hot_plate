
#include "deviceConnect.h"
#include <SPI.h>
#include <math.h>
#include "ST7735.h"
#include "LCDbasic.h"
#include "formation.h"
#include <stdio.h>
#include <stdlib.h>

#define warmingTime		100
#define fluxActiveTime	100	//second
#define fluxActiveTemp	200	//degree
#define reflowTime		100	//second
#define reflowTemp		230	//degree
#define PCBLimitTemp	400	//degree

int tempGap	=	0; //pcb <-> hotplate temperature gap

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
	Serial.println("start");
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
			float nowTemp = checkTemp();
			char inputButton = readSW(false);
			if (inputButton=='L')
			{
				LCD_fill_Rect(1,62,48,17,LGRAY);
				LCD_fill_Rect(51,62,58,17,BLACK);
				LCD_fill_Rect(111,62,48,17,BLACK);
				mode = 3;
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
				mode = 2;
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
			LCD_print(18,66,"setting",WHITE,1);
			LCD_print(65,66,"Soldering",WHITE,1);
			LCD_print(128,66,"keep temp",WHITE,1);
			int tempBar = map(checkTemp(),-50,105,2,58);
			if(tempBar>58)
			tempBar = 58;
			else if(tempBar<2)
			tempBar = 2;
			LCD_Line(151,tempBar-1,158,tempBar-1,1,BLACK);
			LCD_Line(151,tempBar,158,tempBar,1,YELLOW);
			LCD_Line(151,tempBar+1,158,tempBar+1,1,BLACK);
			delay(1);
			char cstr[20] = {'\0'};
			sprintf(cstr,"%f",nowTemp);
			LCD_print_background(5, 40, cstr, RED,BLACK, 2);
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
		int sequence = 0;
		digitalWrite(Plate1,HIGH);	//hot plate off
		while(true)
		{
			nowTime = millis();
			if(nowTime - preTime >= 1000){ //1 sec
				activeTime++;
				preTime = nowTime;
			}
			nowTemp = checkTemp();
			if (activeTime < (warmingTime+fluxActiveTime))//flux active time
			{
				sequence = 1;
				if (nowTemp < 80)
				{
					activeHotplate(30,1000);
				}
				else if (nowTemp < fluxActiveTemp)
				{
					activeHotplate(6+(abs(nowTemp-fluxActiveTemp)/10),1000);
				}
				else{
					activeHotplate(5,1000);
				}
			}
			else if (activeTime >= (warmingTime+fluxActiveTime) && activeTime < (warmingTime+fluxActiveTime+reflowTime)) //reflow time
			{
				sequence = 2;
				if (nowTemp < reflowTemp-10)
				{
					activeHotplate(8+(abs(nowTemp-fluxActiveTemp)/10),1000);
				}
				else{
					activeHotplate(6,1000);
				}
			}
			else{//cooling time
				sequence = 3;
				digitalWrite(Plate1,HIGH);	//hot plate off
			}
			delay(1);
			char cstr[20] = {'\0'};
			sprintf(cstr,"%d",nowTemp);
			LCD_print_background(5, 20,activeTime,CYAN,BLACK,1);
			LCD_print_background(5, 40, cstr, CYAN,BLACK, 2);
			if(sequence==1)
				LCD_print_background(5, 60,"Warming up temperature",RED,BLACK,1);
			else if(sequence==2)
				LCD_print_background(5, 60,"Reflow!               ",RED,BLACK,1);
			else if(sequence==3)
				LCD_print_background(5, 60,"Cooling               ",RED,BLACK,1);
			char inputButton = readSW(true);
			if(inputButton == 'M')
				mode = 0;
			if(mode!=1)
				break;
			else if(activeTime >= warmingTime+reflowTime+fluxActiveTime+100){
				mode = 0;
				break;
			}
		}
		
	}
	while(mode==2)  //keep to temperature mode
	{
		keepScreen();
		int onTime = 100;
		while(true)
		{
			char inputButton = readSW(true);
			float nowTemp = checkTemp();
			if(nowTemp<250)
			{
				activeHotplate(onTime/10,1000);
				LCD_print_background(40,20,"HEATER ON",RED,BLACK,1);
			}
			else
			{
				activeHotplate(onTime/10-10,1000);
				LCD_print_background(40,20,"HEATER OFF",RED,BLACK,1);
			}
			delay(5);
			char cstr[20] = {'\0'};
			sprintf(cstr,"%f",nowTemp);
			LCD_print_background(5, 40, cstr, CYAN,BLACK, 2);
			LCD_print_background(5,60,onTime,RED,BLACK,1);
			if(inputButton == 'M')
				mode = 0;
			if(inputButton == 'L')
				onTime+=10;
			if (inputButton=='R')
				onTime-=10;
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
