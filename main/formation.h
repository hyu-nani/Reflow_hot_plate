
void mainScreen()
{
	LCD_display_OFF();
	LCD_Fill(BLACK);
	LCD_print(10,0,"ReFlow",CYAN,2);
	LCD_print(10,20,"Soldering",CYAN,2);
	LCD_Rect(150,0,10,60,RED);
	LCD_Rect(0,61,50,19,WHITE);
	LCD_Rect(50,61,60,19,WHITE);
	LCD_Rect(110,61,50,19,WHITE);
	LCD_display_ON();
}


int graphStaX = 0;
int graphStaY = 10;
int graphROW = 160;
int graphCol = 70;

int graphEndX = graphStaX+graphROW;
int graphEndY = graphStaY+graphCol;
boolean initialCommand = true;
void solderingLoopScreen()
{
	if(initialCommand == true){
		LCD_Rect(0,0,160,10,RED);
		LCD_Rect(graphStaX,graphStaY,graphROW,graphCol,WHITE);
		int warmTimeLine = map(warmingTime,0,allTime,graphStaX+1,graphEndX-1);
		int fluxTimeLine = map(warmingTime+fluxActiveTime,0,allTime,graphStaX+1,graphEndX-1);
		int	reflTimeLine = map(warmingTime+fluxActiveTime+reflowTime,0,allTime,graphStaX+1,graphEndX-1);
		int warmTempLine = map(warmingTemp,0,SensingTempMax,graphEndY-1,graphStaY+1);
		int fluxTempLine = map(fluxActiveTemp,0,SensingTempMax,graphEndY-1,graphStaY+1);
		int reflTempLine = map(reflowTemp,0,SensingTempMax,graphEndY-1,graphStaY+1);
		LCD_Line(graphStaX+1,warmTempLine,graphEndX-2,warmTempLine,1,GRAY);
		LCD_Line(graphStaX+1,fluxTempLine,graphEndX-2,fluxTempLine,1,GRAY);
		LCD_Line(graphStaX+1,reflTempLine,graphEndX-2,reflTempLine,1,GRAY);
		
		LCD_print(graphStaX+4,graphStaY+2,"Warm",BRRED,1);
		LCD_print(warmTimeLine+4,graphStaY+2,"Flux",YELLOW,1);
		LCD_print(fluxTimeLine+4,graphStaY+2,"Refl",LIGHTBLUE,1);
		LCD_print(reflTimeLine+4,graphStaY+2,"Cooling",GREEN,1);
		LCD_print(warmTimeLine-20,graphEndY-10,warmingTime,BRED,1);
		LCD_print(fluxTimeLine-20,graphEndY-10,fluxActiveTime,YELLOW,1);
		LCD_print(reflTimeLine-20,graphEndY-10,reflowTime,BLUE,1);
		
		LCD_Line(graphStaX+1,graphStaY+1,graphStaX+1,graphEndY-2,1,BRRED);
		LCD_Line(warmTimeLine,graphStaY+1,warmTimeLine,graphEndY-2,1,BRRED);
		LCD_Line(warmTimeLine+1,graphStaY+1,warmTimeLine+1,graphEndY-2,1,YELLOW);
		LCD_Line(fluxTimeLine,graphStaY+1,fluxTimeLine,graphEndY-2,1,YELLOW);
		LCD_Line(fluxTimeLine+1,graphStaY+1,fluxTimeLine+1,graphEndY-2,1,LIGHTBLUE);
		LCD_Line(reflTimeLine,graphStaY+1,reflTimeLine,graphEndY-2,1,LIGHTBLUE);
		LCD_Line(reflTimeLine+1,graphStaY+1,reflTimeLine+1,graphEndY-2,1,GREEN);
		LCD_Line(graphEndX-2,graphStaY+1,graphEndX-2,graphEndY-2,1,GREEN);
		initialCommand = false;
	}
	nowTime = millis();
	if(nowTime - preTime >= 1000){ //1 sec
		activeTime++;
		preTime = nowTime;
	}
	nowTemp = checkTemp();
	
	if(sequence ==0)
		LCD_print_background(1, 1," Warming....",RED,BLACK,1);
	else if(sequence==1)
		LCD_print_background(1, 1," Flux active",YELLOW,BLACK,1);
	else if(sequence==2)
		LCD_print_background(1, 1," [ Reflow ] ",LIGHTBLUE,BLACK,1);
	else if(sequence==3)
		LCD_print_background(1, 1," Cooling....",GREEN,BLACK,1);
	LCD_print_background(70,2,"TEMP:",CYAN,BLACK,1);
	char cstr[10] = {'\0'};
	sprintf(cstr,"%0.2lf",nowTemp);
	LCD_print_background(65, 1, cstr, CYAN,BLACK,1);
	int tempSpot = map(nowTemp,0,SensingTempMax,graphEndY-1,graphStaY+1);
	int timeSpot = map(activeTime,0,allTime,graphStaX+1,graphEndX-1);
	LCD_Pixel(timeSpot,tempSpot,1,LIGHTGREEN);
	LCD_Pixel(timeSpot,tempSpot+1,1,BLACK);
	LCD_Pixel(timeSpot,tempSpot-1,1,BLACK);
	
}

void keepScreen()
{
  LCD_display_OFF();
  LCD_Fill(BLACK);
  LCD_print(15,0,"-Keep temp-",CYAN,2);
  LCD_display_ON();
}

int cursorY = 0;
char setMenu[20][20] = {
	"  Warming up time",
	"  Flux active time",
	"  Reflow time",
	"  Cooling time",
	"  Warming up temp",
	"  Flux active temp",
	"  Reflow temp",
	"  Temp Gap",
	"  Keep mode temp",
	"",
	"",
	"",
};
int cursor = 0;
void setScreen()
{
	if (initialCommand == true)
	{
		LCD_print_background(0,0,"SETTING MENU",CYAN,BLACK,1);
		for(int col = 0; col < 7; col++){
			LCD_print(1,10+10*col,setMenu[col],WHITE);
		}
		initialCommand = false;
	}
	delay(0.01);
	LCD_print(1,10+cursor*10,">>");
	int inputButton = readSW(true);
	if (inputButton == 'L')
		cursor++;
	else if(inputButton == 'R')
		cursor--;
}