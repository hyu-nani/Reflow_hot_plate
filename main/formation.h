
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
int graphCol = 60;

int graphEndX = graphStaX+graphROW;
int graphEndY = graphStaY+graphCol;
void solderingLoopScreen()
{
	nowTime = millis();
	if(nowTime - preTime >= 1000){ //1 sec
		activeTime++;
		preTime = nowTime;
	}
	nowTemp = checkTemp();
	LCD_Rect(0,0,160,10,RED);
	LCD_Rect(graphStaX,graphStaY,graphEndX,graphEndY,WHITE);
	int warmLine = map(warmingTime,0,allTime,graphStaX+1,graphEndX-1);
	int fluxLine = map(warmingTime+fluxActiveTime,0,allTime,graphStaX+1,graphEndX-1);
	int	reflLine = map(warmingTime+fluxActiveTime+reflowTime,0,allTime,graphStaX+1,graphEndX-1);
	LCD_Line(warmLine,11,warmLine,graphCol+9,1,BRRED);
	LCD_Line(fluxLine,11,fluxLine,graphCol+9,1,YELLOW);
	LCD_Line(reflLine,11,reflLine,graphCol+9,1,BLUE);
	LCD_print(warmLine+2,graphEndY-10,"Warm",BRRED,1);
	LCD_print(fluxLine+2,graphEndY-10,"Flux",YELLOW,1);
	LCD_print(reflLine+2,graphEndY-10,"Refl",BLUE,1);
	if(sequence ==0)
		LCD_print_background(1, 2," Warm ",RED,BLACK,1);
	else if(sequence==1)
		LCD_print_background(1, 2," Flux ",RED,BLACK,1);
	else if(sequence==2)
		LCD_print_background(1, 2,"Reflow",RED,BLACK,1);
	else if(sequence==3)
		LCD_print_background(1, 2," Cool ",RED,BLACK,1);
	LCD_print_background(40,2,"TEMP:",CYAN,BLACK,1);
	char cstr[10] = {'\0'};
	sprintf(cstr,"%1f",nowTemp);
	LCD_print_background(65, 2, cstr, CYAN,BLACK,1);
	int tempSpot = map(nowTemp,0,PCBLimitTemp,graphEndY-1,graphStaY+1);
	int timeSpot = map(activeTime,0,allTime,graphStaX+1,graphEndX-1);
	LCD_Pixel(timeSpot,tempSpot,1,GREEN);
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

void setScreen()
{
  LCD_display_OFF();
  LCD_Fill(BLACK);
  LCD_display_ON();
}