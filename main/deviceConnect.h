
#define TFT_RES	3	//D3
#define TFT_CS	5	//
#define TFT_DC	1	//
#define SW_L	16	
#define SW_R	9	
#define SW_M	10
#define Plate	4
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
	pinMode(Plate,OUTPUT);
	pinMode(Temp,INPUT);
	digitalWrite(Plate,LOW);
}

char readSW(boolean nonStop){
		int i=0;
		if (digitalRead(SW_L))
		i += 1;
		if (digitalRead(SW_M))
		i += 3;
		if (digitalRead(SW_R))
		i += 5;
		if(nonStop == true){
			while (digitalRead(SW_L)){delay(10);}
			while (digitalRead(SW_M)){delay(10);}
			while (digitalRead(SW_R)){delay(10);}
		}
		else{
			delay(50);
		}
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


/************************************************************************/
/*  TEMPERATURE SENSING CODE                                            */
/************************************************************************/
//sensor information
float	SensingTempMax	=	350;	//
float	SensingTempMin	=	-50;	//
double	TH25			=	100000;	//register of thermistor at 25`
double	B_Value			=	3470;	//B parameter

//circuit information
float	R1		=	10000;	//series register
double	T0		=	298.15;	//K	275.15 + 25

float	averageTemp1,averageTemp2,averageTemp3;
float	C;
double	T;
int		val;
double	TH_R;				//thermistor resister

float checkTemp(){
	val		= analogRead(Temp);
	TH_R	= R1*(1023-val)/val;	//thermistor resister value
	T		= 1.0/((1.0/T0)+(1.0/B_Value)*log((TH_R/TH25)));
	C		= T-273.15;
	return C;
}

long plateNowTime = millis();
long platePreTime = plateNowTime;
int flipCount = 0;
void activeHotplate(float percentage,int period_ms){
	plateNowTime = millis();
	if(plateNowTime <= ((percentage/100*period_ms)+platePreTime)){
		digitalWrite(Plate,HIGH);//on
	}
	else{
		digitalWrite(Plate,LOW);//off
	}
	if(plateNowTime >= (period_ms + platePreTime)){
		platePreTime = plateNowTime;
	}
}