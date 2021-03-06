/********************************************** 
		LCD4_P0_CGRAM.C  2009/11/08

      P0.0=RS,P0.1=EN,P0.4~P0.7=DATA_BUS                              
**********************************************/
#include "MPC82N.H"

#define uchar unsigned char 
#define uint  unsigned int 

uchar code FONT1[] = {1,2,4,0x0A,2,2,2,0};			 							// 人
uchar code FONT2[] = {0,0x1F,2,0x1A,0x1A,6,2,0};		 						// 可
uchar code FONT3[] = {8,6,8,6,1,2,0x0C,0}; 										// 水
uchar code FONT4[] = {0x0A,0x1F,2,0x1F,0,0x0E,0x0A,0x0E};				 	// 告
uchar code FONT5[] = {1,0x1F,0x17,0x0F,0x0F,0x0A,0x0B,0x12}; 	    		// 宸左	
uchar code FONT6[] = {0,0x1F,0x1D,0x1C,0x1E,0x0A,4,2};						//	宸右
uchar code Table[] = {0,1,2,3,4,5};
sbit   RS=P0^0;       // P00 : RS=0指令暫在器，RS=1資料暫存器
sbit   EN=P0^1;       // P01 : EN=0禁能LCD，EN=1致能LCD
sfr    DATA_BUS=0x80; // DATA BUS=P0.4~P0.7

void Init_LCD(void); 
void CGRAM(void);
void LCD_CmdI(uchar comm);
void LCD_Cmd(uchar comm);
void Send_Data(uchar Data);
void Delay(uint del);
void DLY(uchar dly);

void main(void)
{
	uchar i; EN=0;  
   Init_LCD();               //重置及清除LCD    
	CGRAM();
   LCD_Cmd(0x80);            //游標由第一行開始顯示  
   for(i=0 ; i<6 ; i++)
      Send_Data(Table[i]);   //讀取列�16字送到LCD顯示   
   while (1) {      
      LCD_Cmd(0x08); 
      for(i=0;i<12;i++)Delay(65000);  // D=0關閉顯示幕    
      LCD_Cmd(0x0C);
      for(i=0;i<12;i++)Delay(65000);  // D=1開始顯示幕  
	}
}

void Init_LCD(void)    //LCD的啟始程式  
{
	Delay(65000);	
   LCD_CmdI(0x30); //DL=1：8bit傳輸,N=1：顯示1行,F=0：5*7字型 
	Delay(50000);  
   LCD_CmdI(0x30); //DL=1：8bit傳輸,N=1：顯示1行,F=0：5*7字型 
	Delay(200);
   LCD_CmdI(0x30); //DL=1：8bit傳輸,N=1：顯示1行,F=0：5*7字型 
   LCD_CmdI(0x20); //DL=1：4bit傳輸,N=1：顯示1行,F=0：5*7字型 

   LCD_Cmd(0x28);  //DL=1：4bit傳輸,N=1：顯示2行,F=0：5*7字型 
   LCD_Cmd(0x01);  //清除顯示幕   
	Delay(5000);
   LCD_Cmd(0x0C);  //D=1：顯示幕ON,C=0：不顯示游標,B=0：游標不閃爍     
   LCD_Cmd(0x06);  //I/D=1：顯示完游標右移,S=0：游標移位禁能   
}

void CGRAM(void)
{
	uchar i;
   LCD_Cmd(0x40); 
	for (i=0; i<40; i++)
		Send_Data(FONT1[i]);
}

void Send_Data(uchar Data)  //傳送資料到LCD
{
   DATA_BUS=(DATA_BUS&0x0C)|(Data&0xF3);   
   RS=1; 
	EN=1; DLY(100);	 //資料到LCD內   
   EN=0; DLY(100);  //禁能LCD

   DATA_BUS=(DATA_BUS&0x0C)|((Data<<4)&0xF3); 
   RS=1;       	  
	EN=1; DLY(100);	 //資料到LCD內   
   EN=0; DLY(100);  //禁能LCD
}

void LCD_Cmd(uchar comm) //傳送命令到LCD
{
   DATA_BUS=(DATA_BUS&0x0C)|(comm&0xF3);  
   RS=0;  
	EN=1; DLY(100);	 //資料到LCD內   
   EN=0; DLY(100);  //禁能LCD

   DATA_BUS=(DATA_BUS&0x0C)|((comm<<4)&0xF3);
   RS=0;  
	EN=1; DLY(100);	 //資料到LCD內   
   EN=0; DLY(100);  //禁能LCD
}

void LCD_CmdI(uchar comm) //傳送命令到LCD
{
   DATA_BUS=(DATA_BUS&0x0C)|(comm&0xF3); 
	RS=0;
	EN=1; DLY(100);	 //資料到LCD內   
   EN=0; DLY(100);  //禁能LCD
}
void Delay(uint del)     //延時函數
{
 wait:
	del--;
	if (del!=0) goto wait;
}

void DLY(uchar dly)      //延時函數short
{
 next:
   dly--;
	if (dly!=0) goto next;
}

