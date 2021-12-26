#include "font.h"

#define LCD_swap(a, b) { u16 t = a; a = b; b = t; }
boolean wrap;

void LCD_Fill(u16 color)
{
	LCD_CS_Clr();
	u16 i,j;
	SPI.beginTransaction(mySPISettings);
	LCD_Address_Set(0,0,LCD_W-1,LCD_H-1);
	LCD_WR_REG(0x2c);
	for(i=0;i<LCD_H;i++)
	{
		for(j=0;j<LCD_W;j++)
		{
			LCD_WR_DATA(color);
		}
	}
	SPI.endTransaction();
	LCD_CS_Set();
}


void LCD_image(u16 x0,u16 y0,u16 x1,u16 y1 ,const short unsigned A[])
{
	LCD_CS_Clr();
	u16 i,j;
	int k=0;
	SPI.beginTransaction(mySPISettings);
	LCD_Address_Set(x0,y0,x1-1,y1-1);
	LCD_WR_REG(0x2c);
	for(i=x0;i<x1;i++)
	{
		for(j=y0;j<y1;j++)
		{
			LCD_WR_DATA((word)pgm_read_word(A+k));
			k++;
		}
	}
	SPI.endTransaction();
	LCD_CS_Set();
}

void LCD_fill_Rect(u16 x, u16 y, u16 w, u16 h,u16 color)
{
	LCD_CS_Clr();
	SPI.beginTransaction(mySPISettings);
	if((x >= LCD_W) || (y >= LCD_H)) return;
	if((x + w - 1) >= LCD_W)  w = LCD_W  - x;
	if((y + h - 1) >= LCD_H) h = LCD_H - y;
	
	LCD_Address_Set(x, y, x+w-1, y+h-1);
	
	LCD_WR_REG(0x2C);
	for(y=h; y>0; y--) {
		for(x=w; x>0; x--) {
			LCD_WR_DATA(color);
		}
	}
	SPI.endTransaction();
	LCD_CS_Set();
}

void LCD_HLine(u16 x, u16 y, u16 w, u16 color)
{
	LCD_CS_Clr();
	if((x >= LCD_W) || (y >= LCD_H)) return;
	if((x+w-1) >= LCD_W)  w = LCD_W-x;
	SPI.beginTransaction(mySPISettings);
	LCD_Address_Set(x, y, x+w-1, y);
	LCD_WR_REG(0x2C);
	while (w--) {
		LCD_WR_DATA(color);
	}
	SPI.endTransaction();
	LCD_CS_Set();
}
void LCD_VLine(u16 x, u16 y, u16 h,u16 color) {
	LCD_CS_Clr();
	// Rudimentary clipping
	if((x >= LCD_W) || (y >= LCD_H)) return;
	if((y+h-1) >= LCD_H)
	h = LCD_H-y;
	SPI.beginTransaction(mySPISettings);
	LCD_Address_Set(x, y, x, y+h-1);
	LCD_WR_REG(0x2c);
	while (h--) {
		LCD_WR_DATA(color);
	}
	SPI.endTransaction();
	LCD_CS_Set();
}
void LCD_Pixel(u16 x, u16 y,u16 thin ,u16 color)
{
	LCD_CS_Clr();
	if((x < 0) ||(x >= LCD_W) || (y < 0) || (y >= LCD_H)) return;

	SPI.beginTransaction(mySPISettings);
	LCD_Address_Set(x,y,x+thin-1,y+thin-1);
	LCD_WR_REG(0x2C);
	for(int a=0;a<(thin*thin);a++){
		LCD_WR_DATA(color);
	}
	SPI.endTransaction();
	LCD_CS_Set();
}


void drawCircleHelper( u16 x0, u16 y0,u16 r, uint8_t cornername,u16 thin, u16 color) {
	LCD_CS_Clr();
	int16_t f     = 1 - r;
	int16_t ddF_x = 1;
	int16_t ddF_y = -2 * r;
	int16_t x     = 0;
	int16_t y     = r;

	while (x<y) {
		if (f >= 0) {
			y--;
			ddF_y += 2;
			f     += ddF_y;
		}
		x++;
		ddF_x += 2;
		f     += ddF_x;
		if (cornername & 0x4) {
			LCD_Pixel(x0 + x, y0 + y,thin, color);
			LCD_Pixel(x0 + y, y0 + x,thin, color);
		}
		if (cornername & 0x2) {
			LCD_Pixel(x0 + x, y0 - y,thin, color);
			LCD_Pixel(x0 + y, y0 - x,thin, color);
		}
		if (cornername & 0x8) {
			LCD_Pixel(x0 - y, y0 + x,thin, color);
			LCD_Pixel(x0 - x, y0 + y,thin, color);
		}
		if (cornername & 0x1) {
			LCD_Pixel(x0 - y, y0 - x,thin, color);
			LCD_Pixel(x0 - x, y0 - y,thin, color);
		}
	}
	LCD_CS_Set();
}

void fillCircleHelper(u16 x0, u16 y0, u16 r,uint8_t cornername, u16 delta, u16 color) {
	LCD_CS_Clr();
	int16_t f     = 1 - r;
	int16_t ddF_x = 1;
	int16_t ddF_y = -2 * r;
	int16_t x     = 0;
	int16_t y     = r;

	while (x<y) {
		if (f >= 0) {
			y--;
			ddF_y += 2;
			f     += ddF_y;
		}
		x++;
		ddF_x += 2;
		f     += ddF_x;

		if (cornername & 0x1) {
			LCD_VLine(x0+x, y0-y, 2*y+1+delta, color);
			LCD_VLine(x0+y, y0-x, 2*x+1+delta, color);
		}
		if (cornername & 0x2) {
			LCD_VLine(x0-x, y0-y, 2*y+1+delta, color);
			LCD_VLine(x0-y, y0-x, 2*x+1+delta, color);
		}
	}
	LCD_CS_Set();
}
void LCD_fill_Circle(u16 x0, u16 y0, u16 r,u16 color) {
	LCD_CS_Clr();
	LCD_VLine(x0, y0-r, 2*r+1, color);
	fillCircleHelper(x0, y0, r, 3, 0, color);
	LCD_CS_Set();
}
void LCD_Circle(u16 x0, u16 y0, u16 r,u16 thin,u16 color) {
	LCD_CS_Clr();
	LCD_Pixel(x0,y0-r,thin,color);
	LCD_Pixel(x0,y0+r,thin,color);
	LCD_Pixel(x0-r,y0,thin,color);
	LCD_Pixel(x0+r,y0,thin,color);
	drawCircleHelper(x0,y0,r,15,thin,color);
	LCD_CS_Set();
}
void LCD_Rect(u16 x, u16 y,u16 w, u16 h,u16 color) {
	LCD_CS_Clr();
	LCD_HLine(x, y, w, color);
	LCD_HLine(x, y+h-1, w, color);
	LCD_VLine(x, y, h, color);
	LCD_VLine(x+w-1, y, h, color);
	LCD_CS_Set();
}
void LCD_Round_Rec(u16 x, u16 y, u16 w,u16 h, u16 r, u16 color) {
	LCD_CS_Clr();
	// smarter version
	LCD_HLine(x+r  , y    , w-2*r, color); // Top
	LCD_HLine(x+r  , y+h-1, w-2*r, color); // Bottom
	LCD_VLine(x    , y+r  , h-2*r, color); // Left
	LCD_VLine(x+w-1, y+r  , h-2*r, color); // Right
	// draw four corners
	drawCircleHelper(x+r    , y+r    , r, 1,1, color);
	drawCircleHelper(x+w-r-1, y+r    , r, 2,1, color);
	drawCircleHelper(x+w-r-1, y+h-r-1, r, 4,1, color);
	drawCircleHelper(x+r    , y+h-r-1, r, 8,1, color);
	LCD_CS_Set();
}
void LCD_Line(u16 x0, u16 y0,u16 x1, u16 y1, u16 thin, u16 color) {
	LCD_CS_Clr();
	u16 steep = abs(y1 - y0) > abs(x1 - x0);
	if (steep) {
		LCD_swap(x0, y0);
		LCD_swap(x1, y1);
	}

	if (x0 > x1) {
		LCD_swap(x0, x1);
		LCD_swap(y0, y1);
	}

	int16_t dx, dy;
	dx = x1 - x0;
	dy = abs(y1 - y0);

	int16_t err = dx / 2;
	int16_t ystep;

	if (y0 < y1) {
		ystep = 1;
		} else {
		ystep = -1;
	}

	for (; x0<=x1; x0++) {
		if (steep) {
			LCD_Pixel(y0, x0, thin, color);
			} else {
			LCD_Pixel(x0, y0, thin, color);
		}
		err -= dy;
		if (err < 0) {
			y0 += ystep;
			err += dx;
		}
	}
	LCD_CS_Set();
}
void LCD_Char_bg(u16 x, u16 y, unsigned c,u16 color, u16 bg, uint8_t size) {
	LCD_CS_Clr();
	if((x >= LCD_W)            || // Clip right
	(y >= LCD_H)           || // Clip bottom
	((x + 6 * size - 1) < 0) || // Clip left
	((y + 8 * size - 1) < 0))   // Clip top
	return;

	for (int8_t i=0; i<6; i++ ) {
		uint8_t line;
		if (i == 5)
		line = 0x0;
		else
		line = pgm_read_byte(font+(c*5)+i);
		for (int8_t j = 0; j<8; j++) {
			if (line & 0x1) {
				LCD_Pixel(x+i*size, y+j*size,size,color);
			}
			else if (bg != color) {
				LCD_Pixel(x+i*size, y+j*size,size,bg);
			}
			line >>= 1;
		}
	}
	LCD_CS_Set();
}

void LCD_Char(u16 x, u16 y, unsigned c,u16 color, uint8_t size) {
	LCD_CS_Clr();
	if((x >= LCD_W)            || // Clip right
	(y >= LCD_H)           || // Clip bottom
	((x + 6 * size - 1) < 0) || // Clip left
	((y + 8 * size - 1) < 0))   // Clip top
	return;

	for (int8_t i=0; i<6; i++ ) {
		uint8_t line;
		if (i == 5)
		line = 0x0;
		else
		line = pgm_read_byte(font+(c*5)+i);
		for (int8_t j = 0; j<8; j++) {
			if (line & 0x1) {
				LCD_Pixel(x+i*size, y+j*size,size,color);
			}
			line >>= 1;
		}
	}
	LCD_CS_Set();
}


void LCD_needle(u16 x,u16 y, u16 w,u16 thin, int value ,u16 color)
{
	LCD_CS_Clr();
	int separate = 60;
	int S = w*sin(2*PI*value/separate);
	int C = w*cos(2*PI*value/separate);
	LCD_Line(x,y,x+S,y-C,thin,color);
	LCD_CS_Set();
}
void LCD_dot(u16 x,u16 y, u16 r,u16 thin ,u16 color)
{
	LCD_CS_Clr();
	int separate = 60;
	for(int a=0;a<separate;a++){
		int S = r*sin(2*PI*a/separate);
		int C = r*cos(2*PI*a/separate);
		LCD_Pixel(x+S,y-C,thin,color);
	}
	LCD_CS_Set();
}
void LCD_dot_needle(u16 x,u16 y, u16 w,u16 thin, int value ,u16 color)
{
	LCD_CS_Clr();
	int separate = 60;
	int S = w*sin(2*PI*value/separate);
	int C = w*cos(2*PI*value/separate);
	LCD_Line(x,y,x+S,y-C,thin,color);
	if(value >= (separate*0/4)&&value < (separate*1/4))
	LCD_Pixel(x+S+thin,y-C-thin,thin,color);
	if(value >= (separate*1/4)&&value < (separate*2/4))
	LCD_Pixel(x+S+thin,y-C+thin,thin,color);
	if(value >= (separate*2/4)&&value < (separate*3/4))
	LCD_Pixel(x+S-thin,y-C+thin,thin,color);
	if(value >= (separate*3/4)&&value < (separate*4/4))
	LCD_Pixel(x+S-thin,y-C-thin,thin,color);
	LCD_CS_Set();
}


void LCD_print_background(u16 x,u16 y, const char c[],u16 color ,u16 bgcolor ,uint8_t size)
{
	LCD_CS_Clr();
	int i = 0;
	while(c[i] != '\0'){
		if (c[i] == '\n') {
			y += size*8;
			x  = 0;
		}
		else {
			LCD_Char_bg(x, y, c[i], color, bgcolor, size);
			x += 6*size;
			i++;
			if (x > (LCD_W - size*6)) {
				y += size*8;
				x = 0;
			}
		}
		
	}
	LCD_CS_Set();
}
void LCD_print(u16 x,u16 y, const char c[],u16 color ,uint8_t size)
{
	LCD_CS_Clr();
	int i = 0;
	while(c[i] != '\0'){
		if (c[i] == '\n') {
			y += size*8;
			x  = 0;
		}
		else {
			LCD_Char(x, y, c[i], color, size);
			x += 6*size;
			i++;
			if (x > (LCD_W - size*6)) {
				y += size*8;
				x = 0;
			}
		}
	}
	LCD_CS_Set();
}
void LCD_print_background(u16 x,u16 y, int c,u16 color ,u16 bgcolor ,uint8_t size)
{
	LCD_CS_Clr();
	char A[10];
	int i =0;
	sprintf(A, "%d", c);
	while(A[i] != '\0'){
		if (A[i] == '\n') {
			y += size*8;
			x  = 0;
		}
		else {
			LCD_Char_bg(x, y, A[i], color, bgcolor, size);
			x += 6*size;
			if (x > (LCD_W - size*6)) {
				y += size*8;
				x = 0;
			}
		}
		i++;
	}
	LCD_CS_Set();
}
void LCD_print(u16 x,u16 y, int c,u16 color ,uint8_t size)
{
	LCD_CS_Clr();
	char A[10];
	int i =0;
	sprintf(A, "%d", c);
	while(A[i] != '\0'){
		if (A[i] == '\n') {
			y += size*8;
			x  = 0;
		}
		else {
			LCD_Char(x, y, A[i], color, size);
			x += 6*size;
			if (x > (LCD_W - size*6)) {
				y += size*8;
				x = 0;
			}
		}
		i++;
	}
	LCD_CS_Set();
}

int output_light=0;
void LCD_smooth_on(int wait,int Bright)
{
	delay(10);
	for(;output_light<Bright;output_light+=5){
		//ledcWrite(0,output_light);
		delay(wait);
	}
	//ledcWrite(0,Bright);
	delay(10);
}
void LCD_smooth_off(int wait)
{
	delay(10);
	for(;output_light>0;output_light-=5){
		//ledcWrite(0,output_light);
		delay(wait);
	}
	//ledcWrite(0, 0);
	delay(10);
}
void LCD_off()
{
	//ledcWrite(0, 0);
	delay(10);
}
void LCD_on(int Bright)
{
	//ledcWrite(0,Bright);
	delay(10);
}