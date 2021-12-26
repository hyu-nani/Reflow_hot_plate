
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
	digitalWrite(Plate,LOW);
}

char readSW(){
		int i=0;
		if (digitalRead(SW_L))
		i += 1;
		if (digitalRead(SW_M))
		i += 3;
		if (digitalRead(SW_R))
		i += 5;
		//while (digitalRead(SW_L)){delay(10);}
		//while (digitalRead(SW_M)){delay(10);}
		//while (digitalRead(SW_R)){delay(10);}
		switch (i)
		{
			case 1:
			return 'L';	//left
			break;
			case 3:
			return 'M';	//middle
			break;
			case 4:
			return 'A'; //middle & left
			break;
			case 5:
			return 'R';	//right
			break;
			case 6:
			return 'B';	//right & lest
			break;
			case 8:
			return 'C'; //middle & right
			break;
			case 9:
			return 'D'; //ALL
			break;
			default:
			return 'n';
			break;
		}
}