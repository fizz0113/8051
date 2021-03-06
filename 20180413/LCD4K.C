/***********************************************
    LCD4K.C  2013/07/30   
    16X2 LCD
    P00=RS ,P01=EN ,P0=DATA_BUS   
    4X4 KEYBOARD, P2, NO INTERRUPT                 
************************************************/  
#include "MPC82N.H" 
#define  uchar  unsigned char
#define	uint   unsigned int

//uchar code MSGA[] = "    LCD TEST    "; // 第一列字元
//uchar code MSGB[] = " KEYBOARD TEST  "; // 第二列字元
uchar code MSGC[] = {'2','0','1','8',0,'1','1',1,'2','0',2};

int a=0;
uchar code FONT1[]={0,0,7,4,7,4,7,0};
uchar code FONT2[]={0,0,0x1C,4,0x1C,4,0x1C,0};



uchar code FONT3[]={0x1F,0x11,0x1F,0x14,0x1F,0x14,0x1B,0};//民
uchar code FONT4[]={0x1F,0x13,0x1F,0x1B,0x17,0x1B,0x1F,0};//國

uchar code FONT5[] = {0x04,0x0F,0x12,0x0F,0x0A,0x1F,0x02,0x00,  //年
					      0x1F,0x11,0x1F,0x11,0x1F,0x11,0x11,0x00,  //月
				         0x1F,0x11,0x11,0x1F,0x11,0x11,0x1F,0x00,  //日
							};
uchar code FONT6[] = {0x1F,4,4,0x0F,5,5,0x1F,0,//五
							8,4,0x1F,0x0A,0x0A,0x11,0,0,//六
							0,8,8,0x1F,8,8,0x0F,0,//七
							
							};
uchar code FONT7[] = {0,8,0x1F,8,8,0x0F,0,0};
//uchar code FONT5[]={};


//uchar code FONT1[] = {1,2,4,0x0A,2,2,2,0};			 							// 人
//uchar code FONT2[] = {0,0x1F,2,0x1A,0x1A,6,2,0};		 						// 可
//uchar code FONT3[] = {8,6,8,6,1,2,0x0C,0}; 										// 水
//uchar code FONT4[] = {0x0A,0x1F,2,0x1F,0,0x0E,0x0A,0x0E};				 	// 告
//uchar code FONT5[] = {1,0x1F,0x17,0x0F,0x0F,0x0A,0x0B,0x12}; 	    		// 宸左	
//uchar code FONT6[] = {0,0x1F,0x1D,0x1C,0x1E,0x0A,4,2};						//	宸右
//uchar code Table[] = {3,4,5,6,7,8};

sbit RS=P0^0;         // P00: RS=0指令暫存器，RS=1資料暫存器
sbit EN=P0^1;         // P01: EN=0禁能LCD，EN=1致能LCD
sfr  DATA_BUS = 0x80; // DATA BUS=P0.4 -- P0.7

sbit COLUMN4 = P2^0;
sbit COLUMN3 = P2^1;
sbit COLUMN2 = P2^2;
sbit COLUMN1 = P2^3;	
sbit ROW4	 = P2^4;
sbit ROW3	 = P2^5;
sbit ROW2	 = P2^6;
sbit ROW1	 = P2^7;
 
uchar bdata  f; 
sbit L_FLAG	 = f^0;
sbit K_FLAG  = f^1;
sbit B_FLAG  = f^2;
sbit C_FLAG  = f^3;

uchar KEY_NO,K_TEMP,LCD_CNT;
	
void Init_DISP(void);
void Init_LCD(void); 
void CGRAM(uchar inp);
void LCD_CmdI(uchar comm);
void LCD_Cmd(uchar comm);
void Send_Data(uchar Data);
void Delay(uint del);
void DLY(uchar dly);
void Kdelay(void);
void SCAN(void);
void SCAN_ON(void);
void SCAN_OFF(void);
void K_DISP(void);
void KEY_PRO(void);
void CGRAM2(void);
void CGRAM3(void);
//uchar TE2018[] = "2018";
uchar code STUDENT_ID[] = "        9A417018"; //第一行列表字元
uchar code TEST[] = "R=470K";
void main(void)
{
	EN=0;	Delay(5000);
	f=0; LCD_CNT=16;	
   Init_LCD();
	Init_DISP();
   while (1) Kdelay();      
}

void Init_DISP(void)
{
	uchar i;
	CGRAM(0);
	Delay(5000);
	
   LCD_Cmd(0x80);           // 游標由第一列開始顯示  
   Send_Data(0);
	Delay(5000);
	LCD_Cmd(0x81);
	Send_Data(1);
	Delay(5000);
   LCD_Cmd(0xC0);           // 游標由第二列開始顯示   
   Send_Data(6);
	Delay(5000);
	LCD_Cmd(0xC1);
	Delay(5000);
	Send_Data(6);
	for(i=0;i<120;i++)Delay(50000);
	LCD_Cmd(0x01);
	



	Delay(5000);
	LCD_Cmd(0x80);
	Delay(5000);
	
	for(i=0;i<16;i++)Send_Data(STUDENT_ID[i]);
	

	Delay(5000);
	LCD_Cmd(0xC0);
	Delay(5000);

	for(i=0;i<6;i++)Send_Data(TEST[i]);
	Send_Data(0xF4);
	
}

void Init_LCD(void)  // LCD的啟始程式  
{
	LCD_CmdI(0x30);   // DL=1：8bit傳輸,N=0：顯示1列,F=0：5*7字型 
	Delay(5000);  
   LCD_CmdI(0x30);      
	Delay(200);
   LCD_CmdI(0x30);       
   LCD_CmdI(0x20);   // DL=0：4bit傳輸,N=0：顯示1列,F=0：5*7字型 

   LCD_Cmd(0x28);    // DL=0：4bit傳輸,N=1：顯示2列,F=0：5*7字型 
   LCD_Cmd(0x01);    // 清除顯示幕   
	Delay(5000);
   LCD_Cmd(0x0C);    // D=1：顯示幕ON,C=0：游標不顯示,B=0：游標不閃爍     
   LCD_Cmd(0x06);    // I/D=1：顯示完游標右移,S=0：顯示移位禁能   
}

void CGRAM(uchar inp) 
{
	uchar i;
	LCD_Cmd(0x40);        // CGRAM位址設定 	
	for (i=inp; i<64; i++) Send_Data(FONT1[i]);	
}
void CGRAM2(){
	uchar i;
	LCD_Cmd(0x40);        // CGRAM位址設定 	
	for (i=0; i<64; i++) Send_Data(FONT3[i]);	
	}
void CGRAM3(){
	uchar i;
	LCD_Cmd(0x40);        // CGRAM位址設定 	
	for (i=0; i<8; i++) Send_Data(FONT7[i]);	
}

void LCD_CmdI(uchar comm)  //傳送命令到LCD
{
   DATA_BUS=(DATA_BUS&0x0C)|(comm&0xF3);
   RS=0; EN=1; DLY(100); EN=0; DLY(100); 
}

void LCD_Cmd(uchar comm)   //傳送命令到LCD
{
   DATA_BUS=(DATA_BUS&0x0C)|(comm&0xF3);
   RS=0; EN=1; DLY(100); EN=0; DLY(100); 
   DATA_BUS=(DATA_BUS&0x0C)|((comm<<4)&0xF3);
   RS=0; EN=1; DLY(100); EN=0; DLY(100); 
}

void Send_Data(uchar Data)  //傳送資料到LCD
{
   DATA_BUS=(DATA_BUS&0x0C)|(Data&0xF3);  
   RS=1; EN=1; DLY(100); EN=0; DLY(100); 
	DATA_BUS=(DATA_BUS&0x0C)|((Data<<4)&0xF3);
   RS=1; EN=1; DLY(100); EN=0; DLY(100);  
}

void Delay(uint del)       // 延時函數  
{
  next:
	del--;
	if (del!=0) goto next;
}

void DLY(uchar dly)       // 延時函數  SHORT
{
  wait:
	dly--;
	if (dly!=0) goto wait;
}

void Kdelay(void)          // 延時函數
{
	uint KDCNT2;
	uchar KDCNT;
	KDCNT=10;
   while (KDCNT>0) {
		KDCNT2=30000;
		while (KDCNT2>0) KDCNT2--;
		SCAN();
		if (KEY_NO==0xFF) KDCNT--;
	}
	if (B_FLAG==0 && C_FLAG==1) {
		SP-=6; C_FLAG=0;
	}
}

void SCAN(void)
{
	uchar TEMP, COD_TMP;
	P2=0xF0;
	TEMP=P2&0xF0;
	KEY_NO=0xFF;
	if (TEMP==0xF0) return;
	SCAN_ON();
	if (K_TEMP==0xFF) {KEY_NO=0xFF; return;}
	COD_TMP=K_TEMP;
	Delay(5000);
	SCAN_ON();
	if (K_TEMP==0xFF) {KEY_NO=0xFF; return;}
	if (COD_TMP!=K_TEMP) {KEY_NO=0xFF; return;}
	SCAN_OFF();
	KEY_PRO();
}

void SCAN_ON(void)
{
	KEY_NO=0xFF; K_TEMP=0xFF;
	P2=0xFE;       //P20=0掃描第4行
   if (P2==0xEE) {K_TEMP=P2; KEY_NO=0x0F; return;} 
   if (P2==0xDE) {K_TEMP=P2; KEY_NO=0x0E; return;}  
   if (P2==0xBE) {K_TEMP=P2; KEY_NO=0x0D; return;}   
   if (P2==0x7E) {K_TEMP=P2; KEY_NO=0x0C; return;}  
       
   P2=0xFD;       //P21=0掃描第3行
   if (P2==0xED) {K_TEMP=P2; KEY_NO=0x0B; return;}   
   if (P2==0xDD) {K_TEMP=P2; KEY_NO=0x0A; return;} 
   if (P2==0xBD) {K_TEMP=P2; KEY_NO=9; return;} 
   if (P2==0x7D) {K_TEMP=P2; KEY_NO=8; return;}  
       
   P2=0xFB;       //P22=0掃描第2行
   if (P2==0xEB) {K_TEMP=P2; KEY_NO=7; return;}   
   if (P2==0xDB) {K_TEMP=P2; KEY_NO=6; return;}   
   if (P2==0xBB) {K_TEMP=P2; KEY_NO=5; return;}  
   if (P2==0x7B) {K_TEMP=P2; KEY_NO=4; return;}  
      
   P2=0xF7;       //P23=0掃描第1行
   if (P2==0xE7) {K_TEMP=P2; KEY_NO=3; return;}  
   if (P2==0xD7) {K_TEMP=P2; KEY_NO=2; return;}  
   if (P2==0xB7) {K_TEMP=P2; KEY_NO=1; return;}  
   if (P2==0x77) {K_TEMP=P2; KEY_NO=0; return;} 
}

void SCAN_OFF(void)
{
LOOP:	
	P2=0xF0;
	while (P2!=0xF0);
	Delay(5000);
	if (P2!=0xF0) goto LOOP;
}

void K_DISP(void)
{
	uchar TEMP;
	if (K_FLAG==0) {
  		LCD_Cmd(0x01); Delay(5000);
	   LCD_Cmd(0x0E); 
	   LCD_Cmd(0x6); 
		L_FLAG=0; K_FLAG=1;
		LCD_CNT=16;					
	}
	B_FLAG=0;
 	KEY_NO |= 0x30; 
	TEMP=KEY_NO;
	Send_Data(TEMP); LCD_CNT--;
	if (LCD_CNT!=0) return;
	L_FLAG=!L_FLAG;
	if (L_FLAG==1) { 
		LCD_Cmd(0xC0); LCD_CNT=16;
	}	
	else {
   	LCD_Cmd(0x01); Delay(5000); LCD_CNT=16;				
	}			
}

void KEY_PRO(void)
{
	uchar i,j;
	int count;
	int var;
	switch (KEY_NO) {
		case 0: 
		case 1: 
		case 2: 
		case 3: 
		case 4: 
		case 5: 
		case 6: 
		case 7: 
		case 8: 
		case 9: 
			K_DISP();
			break;	
		case 0x0A: 
			B_FLAG=0;K_FLAG=1;
			Delay(5000);
			B_FLAG=0;K_FLAG=1;
			Delay(5000);
			B_FLAG=0;K_FLAG=1;
			Delay(5000);
			CGRAM2();
			Delay(5000);
			Delay(5000);
    		LCD_Cmd(0x01); Delay(5000);
    		LCD_Cmd(0x80);Delay(5000);
			
    		Send_Data(0);
			Send_Data(1);
			Delay(5000);

			Send_Data(0xB0);//一
			Send_Data('O');
			Send_Data(5);//五
			Send_Data(2);//年
			Send_Data(6);//六
			Send_Data(3);//月
			Send_Data('+');		
			Send_Data(7);//七

			Delay(5000);			
			Send_Data(4);//日
			

			LCD_Cmd(0xC0);
			Delay(5000);
	
			for(i=0;i<16;i++)Send_Data(STUDENT_ID[i]);
			

			break;	
		case 0x0B:
			B_FLAG=0;K_FLAG=1;
			Delay(5000);
			B_FLAG=1; C_FLAG=1; K_FLAG=0;
    		LCD_Cmd(0x01); Delay(5000);
    		//LCD_Cmd(0x0E);Delay(5000);
			LCD_Cmd(0x80);Delay(5000);
			for(i=0;i<16;i++)Send_Data(STUDENT_ID[i]);
			while(B_FLAG==1&&K_FLAG==0){
				for(count=0;count<1000;count++)
				{
					if(B_FLAG!=1&&K_FLAG!=0)break;
					LCD_Cmd(0xC1);
					if(count<10)
					{
						LCD_Cmd(0xC3);Delay(5000);
						Send_Data((char)0x30+count);
					}
					
					else if(count<100)
					{
						LCD_Cmd(0xC2);Delay(5000);
						var = count/10;
						Send_Data((char)0x30+var);
						var = count%10;
						Send_Data((char)0x30+var);
					}
					else if(count<1000)
					{
						LCD_Cmd(0xC1);Delay(5000);
						var = count/100;
						Send_Data((char)0x30+var);
						var = (count-var*100)/10;
						Send_Data((char)0x30+var);
						var = count%10;
						Send_Data((char)0x30+var);
					}
					if(B_FLAG!=1&&K_FLAG!=0)break;
					Kdelay();
				}
			}
			break;
				
		case 0x0C: 
			B_FLAG=0;K_FLAG=1;
			Delay(5000);
			B_FLAG=1; C_FLAG=1; K_FLAG=0;
    		LCD_Cmd(0x01); Delay(5000);
    		//LCD_Cmd(0x0E);Delay(5000);
			LCD_Cmd(0x80);Delay(5000);
			for(i=0;i<16;i++)Send_Data(STUDENT_ID[i]);
			while(B_FLAG==1&&K_FLAG==0){
				for(count=789;count>=123;count--)
				{
					if(B_FLAG!=1&&K_FLAG!=0)break;
					LCD_Cmd(0xC1);
					if(count<10)
					{
						LCD_Cmd(0xC3);Delay(5000);
						Send_Data((char)0x30+count);
					}
					else if(count<100)
					{
						LCD_Cmd(0xC2);Delay(5000);
						var = count/10;
						Send_Data((char)0x30+var);
						var = count%10;
						Send_Data((char)0x30+var);
					}
					else if(count<1000)
					{
						LCD_Cmd(0xC1);Delay(5000);
						var = count/100;
						Send_Data((char)0x30+var);
						var = (count-var*100)/10;
						Send_Data((char)0x30+var);
						var = count%10;
						Send_Data((char)0x30+var);
					}
					if(B_FLAG!=1&&K_FLAG!=0)break;
					Kdelay();
				}
			}
			break;
		case 0x0D:
				B_FLAG=0;K_FLAG=1;
				Delay(5000);
				B_FLAG=1; C_FLAG=1; K_FLAG=0;
	    		LCD_Cmd(0x01); Kdelay();
			   LCD_Cmd(0x84);            //游標由第一行開始顯示 
				LCD_Cmd(0x0C);					//關閉游標
			   for(i=0; i< 8; i++) Send_Data(STUDENT_ID[i]);    
			   for(i=0;i<4;i++)
				{
					LCD_Cmd(0x18);
					for(j=0;j<4;j++)Kdelay();
				}
			   while (B_FLAG==1&&K_FLAG==0) {      
			      for(i=0;i<8;i++)
					{
						LCD_Cmd(0x1C);		//右移
						for(j=0;j<4;j++)Kdelay();
					}
					for(i=0;i<8;i++)
					{
						LCD_Cmd(0x18);		//左移
						for(j=0;j<4;j++)Kdelay();
					}
				}
			break;
		case 0x0E:
				B_FLAG=0;K_FLAG=1;
				Delay(5000);
				B_FLAG=1; C_FLAG=1; K_FLAG=0;
	    		LCD_Cmd(0x01); Delay(5000);
	    		LCD_Cmd(0x0E);
				while (B_FLAG==1&&K_FLAG==0) {
					LCD_Cmd(0x8F);	Kdelay();
					for (i=15; i>0; i--) {LCD_Cmd(0x10); Kdelay();} 	 
	   			LCD_Cmd(0xC0);	
					Kdelay();
					for (i=15; i>0; i--) {LCD_Cmd(0x14); Kdelay();}					
				}	
			break;
		case 0x0F:
				L_FLAG=0; K_FLAG=1; B_FLAG=0;
				Delay(5000);
				LCD_Cmd(0x01); Delay(5000);
				LCD_Cmd(0x80);            //游標由第一行開始顯示  
				LCD_Cmd(0x0C);					//關閉游標
			   for(i=0 ; i<6 ; i++)
			      //Send_Data(Table[i]);   //讀取列�16字送到LCD顯示   
			break;	
		default: 
			break;		
	}
}
