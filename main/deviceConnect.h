
#define TFT_RES	3	//D3
#define TFT_CS	5	//
#define TFT_DC	1	//
#define SW_L	16	
#define SW_R	9	
#define SW_M	10
#define Plate1	4
#define Plate2	12
#define Temp	A0	
#define SCK		14
#define MOSI	13
/*
#define TFT_RES	4
#define TFT_CS	8
#define TFT_DC	3
#define SW_L	2
#define SW_R	0
#define SW_U	10
#define SW_D	1
#define Plate	11
#define Temp	A0
#define SCK		5
#define MOSI	7
*/

void deviceInit(){
	pinMode(TFT_RES,OUTPUT);
	pinMode(TFT_DC,OUTPUT);
	pinMode(TFT_CS,OUTPUT);
	pinMode(SW_L,INPUT);
	pinMode(SW_M,INPUT);
	pinMode(SW_R,INPUT);
	pinMode(Plate1,OUTPUT);
	pinMode(Plate2,OUTPUT);
	pinMode(Temp,INPUT);
	digitalWrite(Plate1,HIGH);
	digitalWrite(Plate2,HIGH);
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
			return 'B';	//right & left
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
