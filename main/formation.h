
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
	LCD_print(18,66,"<-",WHITE,1);
	LCD_print(65,66,"Start",WHITE,1);
	LCD_print(128,66,"->",WHITE,1);
	LCD_display_ON();
}

void startScreen()
{
	LCD_display_OFF();
	LCD_Fill(BLACK);
	LCD_print(15,0,"---START---",CYAN,2);
	LCD_display_ON();
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