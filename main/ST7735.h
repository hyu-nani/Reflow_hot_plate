/*
 * ST7735.h
 *
 * Created: 2021-12-26 오후 9:39:38
 *  Author: cube_
 */ 


#ifndef ST7735_H_
#define ST7735_H_





#endif /* ST7735_H_ */

//deviceConnect


#define WHITE            0xFFFF
#define BLACK            0x0000
#define BLUE             0x001F
#define BRED             0XF81F
#define GRED             0XFFE0
#define GBLUE            0X07FF
#define RED              0xF800
#define MAGENTA          0xF81F
#define GREEN            0x07E0
#define CYAN             0x7FFF
#define YELLOW           0xFFE0
#define BROWN            0XBC40
#define BRRED            0XFC07
#define GRAY             0X8430
#define DARKBLUE         0X01CF
#define LIGHTBLUE        0X7D7C
#define GRAYBLUE         0X5458
#define LIGHTGREEN       0X841F
#define LGRAY            0XC618
#define LGRAYBLUE        0XA651
#define LBBLUE           0X2B12
#define TRANSPARENT  0xFFFFFFFF

#define LCD_W	160 //106 / 26
#define LCD_H	80 //161 / 1
#define W_GAP	1
#define H_GAP	26

#define LCD_RES_Clr()	digitalWrite(TFT_RES,LOW)
#define LCD_RES_Set()	digitalWrite(TFT_RES,HIGH)

#define LCD_DC_Clr()	digitalWrite(TFT_DC,LOW)
#define LCD_DC_Set()	digitalWrite(TFT_DC,HIGH)

#define LCD_CS_Clr()	digitalWrite(TFT_CS,LOW)
#define LCD_CS_Set()	digitalWrite(TFT_CS,HIGH)

#define u16 word
#define delay_ms delayMicroseconds

#if defined (SPI_HAS_TRANSACTION)
static SPISettings mySPISettings;
#endif

void LCD_WR_REG(byte c)
{
	LCD_DC_Clr();
	#if defined (SPI_HAS_TRANSACTION)
	SPI.transfer(c);
	#endif
	LCD_DC_Set();
}

void LCD_WR_DATA(u16 dat)
{
	#if defined (SPI_HAS_TRANSACTION)
	SPI.transfer(dat>>8);
	SPI.transfer(dat);
	#endif
}

void LCD_WR_DATA8(byte c)
{
	#if defined (SPI_HAS_TRANSACTION)
	SPI.transfer(c);
	#endif
}


void LCD_Address_Set(u16 x1,u16 y1,u16 x2,u16 y2)
{
	x1 += W_GAP;
	x2 += W_GAP;
	y1 += H_GAP;
	y2 += H_GAP;
	LCD_WR_REG(0x2A);
	LCD_WR_DATA(x1);
	LCD_WR_DATA(x2);
	LCD_WR_REG(0x2B);
	LCD_WR_DATA(y1);
	LCD_WR_DATA(y2);
}

void LCD_enter_sleep_mode(){
	LCD_CS_Clr();
	SPI.beginTransaction(mySPISettings);
	LCD_WR_REG(0x10);
	SPI.endTransaction();
	LCD_CS_Set();
}

void LCD_sleep_out(){
	LCD_CS_Clr();
	SPI.beginTransaction(mySPISettings);
	LCD_WR_REG(0x11);
	SPI.endTransaction();
	LCD_CS_Set();
}

void LCD_partial_mode_ON(){
	LCD_CS_Clr();
	SPI.beginTransaction(mySPISettings);
	LCD_WR_REG(0x12);
	SPI.endTransaction();
	LCD_CS_Set();
}

void LCD_normal_display_mode_ON(){
	LCD_CS_Clr();
	SPI.beginTransaction(mySPISettings);
	LCD_WR_REG(0x13);
	SPI.endTransaction();
	LCD_CS_Set();
}

void LCD_display_inversion_ON(){
	LCD_CS_Clr();
	SPI.beginTransaction(mySPISettings);
	LCD_WR_REG(0x21);
	SPI.endTransaction();
	LCD_CS_Set();
}

void LCD_display_inversion_OFF(){
	LCD_CS_Clr();
	SPI.beginTransaction(mySPISettings);
	LCD_WR_REG(0x20);
	SPI.endTransaction();
	LCD_CS_Set();
}

void LCD_display_ON(){
	LCD_CS_Clr();
	SPI.beginTransaction(mySPISettings);
	LCD_WR_REG(0x29);
	SPI.endTransaction();
	LCD_CS_Set();
}

void LCD_display_OFF(){
	LCD_CS_Clr();
	SPI.beginTransaction(mySPISettings);
	LCD_WR_REG(0x28);
	SPI.endTransaction();
	LCD_CS_Set();
}

void LCD_memory_write(u16 data){
	LCD_CS_Clr();
	SPI.beginTransaction(mySPISettings);
	LCD_WR_REG(0x2c);
	LCD_WR_DATA(data);
	SPI.endTransaction();
	LCD_CS_Set();
}

void LCD_tearing_effect_line_off(){
	LCD_CS_Clr();
	SPI.beginTransaction(mySPISettings);
	LCD_WR_REG(0x34);
	SPI.endTransaction();
	LCD_CS_Set();
}

void LCD_tearing_effect_line_on(){
	LCD_CS_Clr();
	SPI.beginTransaction(mySPISettings);
	LCD_WR_REG(0x35);
	SPI.endTransaction();
	LCD_CS_Set();
}

void LCD_Init(void)
{
	LCD_CS_Clr();
	SPI.beginTransaction(mySPISettings);
	LCD_RES_Clr();
	delay_ms(100);
	LCD_RES_Set();
	delay_ms(100);
	
	LCD_WR_REG(0x11);//Sleep exit
	delay(120);
	
	LCD_WR_REG(0xB1);   //in nomal mode
	LCD_WR_DATA8(0x05); //
	LCD_WR_DATA8(0x3C);
	LCD_WR_DATA8(0x3C);
	LCD_WR_REG(0xB2);   //in idle mode
	LCD_WR_DATA8(0x05);
	LCD_WR_DATA8(0x3C);
	LCD_WR_DATA8(0x3C);
	LCD_WR_REG(0xB3);   //in partical mode
	LCD_WR_DATA8(0x05);
	LCD_WR_DATA8(0x3C);
	LCD_WR_DATA8(0x3C);
	LCD_WR_DATA8(0x05);
	LCD_WR_DATA8(0x3C);
	LCD_WR_DATA8(0x3C);

	LCD_WR_REG(0xB4);   //displat incersion control
	LCD_WR_DATA8(0x03);
	
	LCD_WR_REG(0xC0);   //power control 1
	LCD_WR_DATA8(0x28);
	LCD_WR_DATA8(0x08);
	LCD_WR_DATA8(0x04);
	LCD_WR_REG(0xC1);   //power control 2
	LCD_WR_DATA8(0xC0);
	LCD_WR_REG(0xC2);   //power control 3 nomal mode
	LCD_WR_DATA8(0x0D);
	LCD_WR_DATA8(0x00);
	LCD_WR_REG(0xC3);   //power control 4 idle mode
	LCD_WR_DATA8(0x8D);
	LCD_WR_DATA8(0x2A);
	LCD_WR_REG(0xC4);   //power control 5 particle mode
	LCD_WR_DATA8(0x8D);
	LCD_WR_DATA8(0xEE);
	
	LCD_WR_REG(0xC5);   //VCOM control
	LCD_WR_DATA8(0x1A);
	
	LCD_WR_REG(0x36);   //MX, MY, RGB mode rotation
	LCD_WR_DATA8(0x68); //XXX01000

	LCD_WR_REG(0xE0);   //Gamma (‘+’polarity) Correction Characteristics Setting
	LCD_WR_DATA8(0x03);
	LCD_WR_DATA8(0x22);
	LCD_WR_DATA8(0x07);
	LCD_WR_DATA8(0x0A);
	LCD_WR_DATA8(0x2E);
	LCD_WR_DATA8(0x30);
	LCD_WR_DATA8(0x25);
	LCD_WR_DATA8(0x2A);
	LCD_WR_DATA8(0x28);
	LCD_WR_DATA8(0x26);
	LCD_WR_DATA8(0x2E);
	LCD_WR_DATA8(0x3A);
	LCD_WR_DATA8(0x00);
	LCD_WR_DATA8(0x01);
	LCD_WR_DATA8(0x03);
	LCD_WR_DATA8(0x13);
	LCD_WR_REG(0xE1);   //Gamma (‘-’polarity) Correction Characteristics Setting
	LCD_WR_DATA8(0x04);
	LCD_WR_DATA8(0x16);
	LCD_WR_DATA8(0x06);
	LCD_WR_DATA8(0x0D);
	LCD_WR_DATA8(0x2D);
	LCD_WR_DATA8(0x26);
	LCD_WR_DATA8(0x23);
	LCD_WR_DATA8(0x27);
	LCD_WR_DATA8(0x27);
	LCD_WR_DATA8(0x25);
	LCD_WR_DATA8(0x2D);
	LCD_WR_DATA8(0x3B);
	LCD_WR_DATA8(0x00);
	LCD_WR_DATA8(0x01);
	LCD_WR_DATA8(0x04);
	LCD_WR_DATA8(0x13);

	//LCD_WR_REG(0x2A);    //Column Address Set
	//LCD_WR_DATA8(0x00);
	//LCD_WR_DATA8(0x00);
	//LCD_WR_DATA8(0x00);
	//LCD_WR_DATA8(0x7F);
	//LCD_WR_REG(0x2B);   // Row Address Set
	//LCD_WR_DATA8(0x00);
	//LCD_WR_DATA8(0x00);
	//LCD_WR_DATA8(0x00);
	//LCD_WR_DATA8(0x9F);

	LCD_WR_REG(0x21);   //Display Inversion On
	LCD_WR_REG(0x3A);   //Interface Pixel Format
	LCD_WR_DATA8(0x05);
	LCD_WR_REG(0x29);   //Display on
	SPI.endTransaction();
	LCD_CS_Set();
}
