#include <SPI.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

int warmingTime		=	120;
int fluxActiveTime	=	200;	//second
int reflowTime		=	150;	//second
int coolTime		=	200;
int allTime			=	warmingTime+fluxActiveTime+reflowTime+coolTime;

int warmingTemp		=	100;	//degree
int fluxActiveTemp	=	160;	//degree
int reflowTemp		=	220;	//degree

#define PCBLimitTemp	400	//degree

int		tempGap		=	-10; //pcb - hotplate temperature gap
int		activeTime	=	0;
float	nowTemp;
int		sequence	=	0;
int		nowTime		=	millis();
int		preTime		=	nowTime;
int		keepTemp	=	100;
int		mode		=	0;

#include "deviceConnect.h"
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
	Serial.println("start");
	delay(2000);
}

void loop() {
	initialCommand = true;
	while(mode==0)	//main screen
	{ 
		LCD_Fill(BLACK);
		mainScreen();
		while(true)
		{
			nowTime = millis();
			nowTemp = checkTemp();
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
			LCD_print(5,66,"Setting",WHITE,1);
			LCD_print(53,66,"Soldering",WHITE,1);
			LCD_print(112,66,"Keeptemp",WHITE,1);
			int tempBar = map(nowTemp,0,305,56,2);
			if(tempBar>58)
			tempBar = 58;
			else if(tempBar<2)
			tempBar = 2;
			LCD_Line(151,tempBar-1,158,tempBar-1,1,BLACK);
			LCD_Line(151,tempBar,158,tempBar,1,YELLOW);
			LCD_Line(151,tempBar+1,158,tempBar+1,1,YELLOW);
			LCD_Line(151,tempBar+2,158,tempBar+2,1,BLACK);
			delay(1);
			char cstr[20] = {'\0'};
			sprintf(cstr,"%0.2lf",nowTemp);
			LCD_print_background(5, 40, cstr, RED,BLACK, 2);
			if(mode!=0)
				break;
			digitalWrite(Plate1,HIGH);
		}
	}
	while(mode==1)	//soldering start
	{
		preTime = nowTime;
		sequence = 0;
		activeTime = 0;
		digitalWrite(Plate1,HIGH);	//hot plate off
		LCD_Fill(BLACK);
		while(true)
		{
			nowTime = millis();
			if(nowTime - preTime >= 1000){ //1 sec
				activeTime++;
				preTime = nowTime;
			}
			nowTemp = checkTemp();
			if (activeTime < warmingTime)
			{
				sequence = 0;
				if (nowTemp+tempGap*(warmingTime-activeTime)/warmingTime < warmingTemp)
				{
					activeHotplate(20+(abs(nowTemp-warmingTemp)/5),1000);
				}
				else{
					activeHotplate(10,1000);
				}
			}
			else if (activeTime < (warmingTime+fluxActiveTime))//flux active time
			{
				sequence = 1;
				if ((nowTemp+tempGap*(fluxActiveTime+warmingTime-activeTime)/(warmingTime+fluxActiveTime)) < fluxActiveTemp)
				{
					activeHotplate(18+(abs(nowTemp-fluxActiveTemp)/3),1000);
				}
				else{
					activeHotplate(10,1000);
				}
			}
			else if (activeTime < (warmingTime+fluxActiveTime+reflowTime)) //reflow time
			{
				sequence = 2;
				if ((nowTemp+tempGap*(warmingTime+fluxActiveTime+reflowTime-activeTime)/(warmingTime+fluxActiveTime+reflowTime)) < reflowTemp)
				{
					activeHotplate(20+(abs(nowTemp-reflowTemp)/5),1000);
				}
				else{
					activeHotplate(10,1000);
				}
			}
			else{//cooling time
				sequence = 3;
				digitalWrite(Plate1,HIGH);	//hot plate off
			}
			solderingLoopScreen();
			delay(0.01);
			char inputButton = readSW(true);
			if(inputButton == 'M')
				mode = 0;
			if(mode!=1)
				break;
			else if(activeTime >= warmingTime+reflowTime+fluxActiveTime+100 && nowTemp < 50){
				mode = 0;
				break;
			}
		}
		
	}
	while(mode==2)  //keep to temperature mode
	{
		keepScreen();
		while(true)
		{
			char inputButton = readSW(true);
			float nowTemp = checkTemp();
			if(nowTemp<=keepTemp)
			{
				activeHotplate(20+(abs(nowTemp-keepTemp)/5),1000);
				LCD_print_background(20,20,"HEATER ON:",RED,BLACK,1);
				LCD_print_background(50,20,(20+(abs(nowTemp-keepTemp)/5)),RED,BLACK,1);
			}
			else
			{
				activeHotplate(15,1000);
				LCD_print_background(40,20,"HEATER 15",RED,BLACK,1);
			}
			delay(5);
			char cstr[10] = {'\0'};
			sprintf(cstr,"%0.2lf",nowTemp);
			LCD_print_background(5,30,"Seting temp :",GREEN,BLACK,1);
			LCD_print_background(70,30,keepTemp,GREEN,BLACK,1);
			LCD_print_background(5,40, cstr, CYAN,BLACK, 2);
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
		while(true)
		{
			delay(10);
			setScreen();
			
		}
	}
}
