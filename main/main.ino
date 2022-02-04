#include <SPI.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <EEPROM.h>

int warmingTime		=	100;
int	fluxActiveTime	=	160;	//second
int reflowTime		=	80;	//second
int coolTime		=	200;

int warmingTemp		=	130;	//degree
int fluxActiveTemp	=	180;	//degree
int reflowTemp		=	255;	//degree
int divideTemp		=	80;
int basicAddTime	=	7;

#define PCBLimitTemp	400	//degree

int		cursor		=	0;
int		page		=	0;

int		tempGap		=	-5; //pcb - hotplate temperature gap
int		activeTime	=	0;
float	nowTemp;
int		sequence	=	0;
int		nowTime		=	millis();
int		preTime		=	nowTime;
int		keepTemp	=	100;
int		pageMode	=	0;


int setValue[20] = {
	warmingTime,
	fluxActiveTime,
	reflowTime,
	coolTime,
	warmingTemp,
	fluxActiveTemp,
	reflowTemp,
	divideTemp,
	keepTemp,
	basicAddTime,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0
};
int allTime			=	setValue[0]+setValue[1]+setValue[2]+setValue[3];

#include "deviceConnect.h"
#include "ST7735.h"
#include "LCDbasic.h"
#include "formation.h"
<<<<<<< Updated upstream
#include "eepromList.h"
=======
#include <iostream>
#include <string>

using namespace std;
>>>>>>> Stashed changes

void setup() {
	deviceInit();
	SPIClass(HSPI);
	SPI.begin();		//SPI begin
	mySPISettings = SPISettings(40000000, MSBFIRST, SPI_MODE0); //ESP speed /4
	EEPROM.begin(20);
	pinMode(LED_BUILTIN, OUTPUT);
	digitalWrite(LED_BUILTIN, LOW);
	LCD_Init();
	LCD_Fill(BLACK);
	LCD_display_OFF();
	LCD_Fill(WHITE);
	LCD_image(25,0,136,80,logo);
	LCD_display_ON();
	delay(2000);
<<<<<<< Updated upstream
	//eepromDataSave();
	eepromDataLoad();
}
=======
}
int nowTime = millis();
int preTime = nowTime;
int mode = 0;
>>>>>>> Stashed changes

void loop() {
	while(pageMode==0)	//main screen
	{ 
		initialCommand = true;
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
				pageMode = 3;
			}
			else if(inputButton == 'M')
			{
				LCD_fill_Rect(1,62,48,17,BLACK);
				LCD_fill_Rect(51,62,58,17,LGRAY);
				LCD_fill_Rect(111,62,48,17,BLACK);
				pageMode = 1;
			}
			else if(inputButton == 'R'){
				LCD_fill_Rect(1,62,48,17,BLACK);
				LCD_fill_Rect(51,62,58,17,BLACK);
				LCD_fill_Rect(111,62,48,17,LGRAY);
				pageMode = 2;
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
				pageMode = 2;
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
			LCD_print(112,66,"keepTemp",WHITE,1);
			int tempBar = map(nowTemp,0,300,56,2);
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
			LCD_print_background(110, 10, cstr, GREEN,BLACK, 1);
			if(pageMode!=0)
				break;
			digitalWrite(Plate,LOW);
		}
	}
	while(pageMode==1)	//soldering start
	{
<<<<<<< Updated upstream
		preTime = nowTime;
		sequence = 0;
		activeTime = 0;
		digitalWrite(Plate,LOW);	//hot plate off
		LCD_Fill(BLACK);
		float add;
		while(true)
		{
			nowTime = millis();
			if(nowTime - preTime >= 1000){ //1 sec
				activeTime++;
				preTime = nowTime;
			}
			nowTemp = checkTemp();
			if (nowTemp > 320){
				digitalWrite(Plate,LOW);
			}
			if (activeTime < setValue[0])
			{
				sequence = 0;
				if(setValue[4]-nowTemp >= 0)
					add = pow((setValue[4]-nowTemp)/(setValue[7]/10),2);
				else
					add = -pow((setValue[4]-nowTemp)/(setValue[7]/10),2);
				add = add+basicAddTime;
				if(add>25)
					add=25;
				if(add<-25)
					add=-25;
				activeHotplate(add,1000);
			}
			else if (activeTime < (setValue[0]+setValue[1]))//flux active time
			{
				sequence = 1;
				if(setValue[5]-nowTemp >= 0)
					add = pow((setValue[5]-nowTemp)/(setValue[7]/10),2);
				else
					add = -pow((setValue[5]-nowTemp)/(setValue[7]/10),2);
				add = add+basicAddTime;
				if(add>25)
					add=25;
				if(add<-25)
					add=-25;
				activeHotplate(add,1000);
			}
			else if (activeTime < (setValue[0]+setValue[1]+setValue[2])) //reflow time
			{
				sequence = 2;
				if(setValue[6]-nowTemp >= 0)
					add = pow((setValue[6]-nowTemp)/(setValue[7]/10),2);
				else
					add = -pow((setValue[6]-nowTemp)/(setValue[7]/10),2);
				add = add+basicAddTime;
				if(add>25)
					add=25;
				if(add<-25)
					add=-25;
				activeHotplate(add,1000);
			}
			else{//cooling time
				sequence = 3;
				digitalWrite(Plate,LOW);	//hot plate off
			}
			solderingLoopScreen();
			delay(0.001);
=======
		startScreen();
		int ativeTime = 0;
		int nowTemp = 0;
		int sequence = 1;
		digitalWrite(Plate1,HIGH); //hot plate off
		while(true){
			nowTime = millis();
			if(nowTime - preTime >= 1000){//1sec
				ativeTime++;
				preTime = nowTime;
			}
			nowTemp	= checkTemp();
			if(ativeTime < 120){//warming up goto 150 degree
				if(nowTemp < 150){
					digitalWrite(Plate1,LOW); //hot plate on
				}
				else{
					digitalWrite(Plate1,HIGH); //hot plate off
				}
			}
			else if(ativeTime >= 120 && ativeTime < 240){//flux active time
				if(nowTemp < 145){
					digitalWrite(Plate1,LOW); //hot plate on
				}
				else{
					digitalWrite(Plate1,HIGH); //hot plate off
				}
			}
			else if(ativeTime >= 240 && ativeTime < 300){//reflow time
				if(nowTemp < 230){
					digitalWrite(Plate1,LOW); //hot plate on
				}
				else{
					digitalWrite(Plate1,HIGH); //hot plate off
				}
			}
			else{//end time cooling
				digitalWrite(Plate1,HIGH); //hot plate off
			}
			delay(100);
			char str[10];
			itoa(nowTemp, str, 10);
			LCD_print(0,40,str,CYAN,2);
>>>>>>> Stashed changes
			char inputButton = readSW(true);
			if(inputButton == 'M')
				pageMode = 0;
			if(pageMode!=1 || nowTemp > 300){
				digitalWrite(Plate,LOW);
				pageMode = 0;
				break;
			}
			else if(activeTime >= 1000 && nowTemp < 50){
				pageMode = 0;
				break;
			}
		}
		
	}
	while(pageMode==2)  //keep to temperature pageMode
	{
		keepScreen();
		while(true)
		{
			char inputButton = readSW(false);
			float nowTemp = checkTemp();
			if(nowTemp<=setValue[8])
			{
				activeHotplate(15+(abs(nowTemp-setValue[8])/5),2000);
				LCD_print_background(40,20,"HEATER ON:",RED,BLACK,1);
				LCD_print_background(100,20,(20+(abs(nowTemp-setValue[8])/5)),RED,BLACK,1);
			}
			else
			{
				activeHotplate(8,2000);
				LCD_print_background(40,20,"HEATER LOW       ",RED,BLACK,1);
			}
			delay(5);
			char cstr[10] = {'\0'};
			sprintf(cstr,"%0.2lf",nowTemp);
			LCD_print_background(20,30,"Setting temp :",GREEN,BLACK,1);
			LCD_print_background(110,30,setValue[8],GREEN,BLACK,1);
			LCD_print_background(40,40, cstr, CYAN,BLACK, 2);
			if(pageMode != 2)
				break;
			if (inputButton == 'R')
				setValue[8]--;
			else if(inputButton== 'L')
				setValue[8]++;
			else if(inputButton== 'M')
				pageMode=0;
		}
	}
	while(pageMode==3)  //SPISettings
	{
		eepromDataLoad();
		LCD_Fill(BLACK);
		cursor=0;
		page=0;
		while(true)
		{
			delay(1);
			setScreen();
			LCD_print(1,10+cursor*10,">",CYAN,1);
			int inputButton = readSW(true);
			if (inputButton == 'L'){
				cursor++;
				if(page<2 && cursor == 7){
					page++;
					LCD_print(1,cursor*10,">",BLACK,1);
					cursor = 0;
					initialCommand = true;
				}
				LCD_print(1,cursor*10,">",BLACK,1);
			}
			else if(inputButton == 'R'){
				if(page > 0	&& cursor==0){
					page--;
					LCD_print(1,20+cursor*10,">",BLACK,1);
					cursor=7;
					initialCommand = true;
				}
				if(cursor>0)
				cursor--;
				LCD_print(1,20+cursor*10,">",BLACK,1);
			}
			//
			if(inputButton=='M'&&cursor==3&&page==1){
				eepromDataSave();
				allTime			=	setValue[0]+setValue[1]+setValue[2]+setValue[3];
				pageMode = 0;
				break;
			}
			else if(inputButton=='M'){
				inputButton = 'n';
				LCD_print_background(1,(10+10*cursor),setMenu[cursor+page*7],YELLOW,BLACK,1);
				LCD_print_background(125,(10+10*cursor),setValue[cursor+page*7],YELLOW,BLACK,1);
				LCD_print_background(118,(10+10*cursor),"+",YELLOW,BLACK,1);
				LCD_print_background(145,(10+10*cursor),"-",YELLOW,BLACK,1);
				delay(100);
				while(true){
					inputButton = readSW(false);
					if(inputButton=='M'){
						LCD_print_background(1,(10+10*cursor),setMenu[cursor+page*7],WHITE,BLACK,1);
						LCD_print_background(125,(10+10*cursor),setValue[cursor+page*7],WHITE,BLACK,1);
						LCD_print_background(118,(10+10*cursor),"+",BLACK,BLACK,1);
						LCD_print_background(145,(10+10*cursor),"-",BLACK,BLACK,1);
						delay(100);
						break;
					}
					else if(inputButton=='L'){
						setValue[cursor+page*7]++;
						LCD_fill_Rect(125,(10+10*cursor),18,8,BLACK);
						LCD_print_background(125,(10+10*cursor),setValue[cursor+page*7],YELLOW,BLACK,1);
					}
					else if(inputButton=='R'){
						setValue[cursor+page*7]--;
						LCD_fill_Rect(125,(10+10*cursor),18,8,BLACK);
						LCD_print_background(125,(10+10*cursor),setValue[cursor+page*7],YELLOW,BLACK,1);
					}
					delay(10);
				}
			}
		}
	}
}
