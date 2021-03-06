/***********************************************  
    ROT_ADC_MD.C   2018/05/18

    16X2 LCD, P00=RS ,P01=EN ,P04~P07=DATA_BUS   
    ROTARY, P36, P37, P32 
    MPC82G516 OK
************************************************/  
#include "MAIN.H" 

uchar code MSG[6][16] ={"A: ENTER        ",  
                        "B: HELP         ",  
                        "C: TIME DISPLAY ",        
                        "D: PWM4         ",  
                        "E: ADC0         ",  
                        "F: ADC5         "};
uchar code MSGA[] = "C: TIME D: TSET ";  
uchar code MSGB[] = "E: RTC  F: RSET "; 
uchar code MSGC[] = "TIME";

uchar bdata f;
sbit  D_FLAG   = f^0;

uchar bdata g;
sbit  BE_FLAG  = g^0;
sbit  D_ABLE   = g^1;

uchar OLD,MENUPTR,B_CNT;
uchar SECOND,HOUR,MIN,SEC;

void ROTP_INT(void) interrupt 0 using 1
{
	uchar i;
	BEEP_PRO();	
  	switch (MENUPTR) {
		case 0:
		   break;
		case 1:
         HELP();
			break;
		case 2:
 			LCD_Cmd(0x01); Delay(5000);  
			LCD_Cmd(0x0C); LCD_Cmd(0x86);
  			for (i=0; i<4; i++) Send_Data(MSGC[i]);  
			LCD_Cmd(0xC4); 	 
         TIME_DISP(); D_ABLE=1;						 
			break;
		case 3:
			break;
		case 4:
		   break;
		case 5:
			break;
		default:
			break;
	}
	while (ROTP==0);
}

void T0_INT(void) interrupt 1 using 2 
{
	if (BE_FLAG==1) {
		B_CNT--;
		if (B_CNT==0) {BE_FLAG=0; TR0=0; BEEP=1;}
	}
}

void T2_INT(void) interrupt 5 using 3
{
	TF2=0;
	SECOND-=1;
	if (SECOND>0) return;
	SECOND=10; TIME_INC();
	if (D_ABLE==1) D_FLAG=1;
	else D_FLAG=0;
} 

void MAIN(void)
{
	uchar j,k;
	EN=0;	f=0; g=0;  	
 	TMOD=2; TH0=0; TL0=0;
	SECOND=10; // SECOND=100, 1��
	T2CON=0;   // T2 16 BIT Timer 
              // Fosc=22.1184MHz�ATimer�ɯ�=Fosc/12=1.8432MHz
              // Timer���ɮɶ�=(1/1.8432MHz)*18432=10000uS=10ms  
   TH2=(65536-18432)>>8; TL2=(65536-18432); TR2=1; // 10ms 
	RCAP2H=(65536-18432)>>8; RCAP2L=(65536-18432); 

   Init_LCD(); 
	j=ROTA; k=ROTB; OLD=k*2+j; 
	MENUPTR=0; MENU_DISP();
   HOUR=0; MIN=0; SEC=0;
	EX0=1; IT0=1;      // ROTARY PUSH BUTTON
   ET0=1; PT0=1;  
	ET2=1; PT2=1;
   EA=1;
	while (1) {
		ROT_SCAN(10);
		if (D_FLAG==1) {
         TIME_DISP(); D_FLAG=0;
		}
	}
}

void BEEP_PRO(void)
{
	B_CNT=0; BE_FLAG=1; 
	BEEP=0; TR0=1;
}

void MENU_DISP(void)
{
	uchar i;
	D_ABLE=0; D_FLAG=0;
	LCD_Cmd(0x80);	
	for (i=0; i<16; i++) Send_Data(MSG[MENUPTR%6][i]); 
	LCD_Cmd(0xC0);	
	for (i=0; i<16; i++) Send_Data(MSG[(MENUPTR+1)%6][i]); 
}

void HELP(void)
{
	uchar i;
	f=0; D_ABLE=0;
	LCD_Cmd(1); Delay(5000);
	LCD_Cmd(0x0C); LCD_Cmd(0x80);            
   for (i=0; i<16; i++) Send_Data(MSGA[i]);    
   LCD_Cmd(0xC0);                
   for (i=0; i<16; i++) Send_Data(MSGB[i]);  
}

void TIME_INC(void)
{
	SEC+=1; 
	if (SEC < 60) return; 
	SEC=0; MIN+=1;
	if (MIN < 60) return;
	MIN=0; HOUR+=1;
	if (HOUR < 24) return;
	HOUR=0;	
}

void TIME_DISP(void)
{
	uchar i,j;
	LCD_Cmd(0xC4); LCD_Cmd(0x0C);

	j=HOUR; i=j/10; 
	if (i==0) i=0x20;
	else i+=0x30;
	Send_Data(i);
	j%=10; j+=0x30;
	Send_Data(j); Send_Data(':');

	j=MIN; i=j/10; 
	if (i==0) i=0x20;
	else i+=0x30;
	Send_Data(i);
	j%=10; j+=0x30;
	Send_Data(j); Send_Data(':');

	j=SEC; i=j/10; 
	if (i==0) i=0x20;
	else i+=0x30;
	Send_Data(i);
	j%=10; j+=0x30;
	Send_Data(j);
}
