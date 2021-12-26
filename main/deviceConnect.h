/*
 * deviceConnect.h
 *
 * Created: 2021-12-26 오후 9:29:54
 *  Author: cube_
 */ 


#ifndef DEVICECONNECT_H_
#define DEVICECONNECT_H_

#endif /* DEVICECONNECT_H_ */

#define TFT_RES	0	//D3
#define TFT_CS	14	//D5
#define TFT_DC	2	//D4
#define SW_L	16	//D0
#define SW_M	5	//D1
#define SW_R	4	//D2
#define Plate	1	//TX
#define Temp	A0	

void deviceInit(){
	pinMode(TFT_RES,OUTPUT);
	pinMode(TFT_DC,OUTPUT);
	pinMode(TFT_CS,OUTPUT);
	pinMode(SW_L,INPUT);
	pinMode(SW_M,INPUT);
	pinMode(SW_R,INPUT);
	pinMode(Plate,OUTPUT);
	pinMode(Temp,INPUT);
}