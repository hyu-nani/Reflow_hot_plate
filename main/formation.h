
void mainScreen(){
	LCD_off();
	LCD_Fill(BLACK);
	LCD_Rect(0,10,160,50,GREEN);
	LCD_print(135,11,"TEMP",GREEN,1);
	LCD_print(10,0,"ReFlow Soldering System",CYAN,1);
	LCD_Rect(0,61,50,19,MAGENTA);
	LCD_Rect(50,61,60,19,MAGENTA);
	LCD_Rect(110,61,50,19,MAGENTA);
}