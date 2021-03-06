/******************************************************** 
      LCD4_P0.C  2013/07/30
 
      P0.0=RS,P0.1=EN,P0.4~P0.7=DATA_BUS                              
*********************************************************/
#include "MPC82N.H"

#define uchar unsigned char 
#define uint  unsigned int 

uchar code Table1[] = "Hi! How are you."; //第一行列表字元
uchar code Table2[] = "I'm fine. Thanks"; //第二行列表字�

sbit  RS=P0^0;       // P00:RS=0指令暫在器，RS=1資料暫存器
sbit  EN=P0^1;       // P01:EN=0禁能LCD，EN=1致能LCD
sfr   DATA_BUS=0x80; // DATA bus=P0.4~P0.7

void Init_LCD(void); 
void LCD_CmdI(uchar comm);
void LCD_Cmd(uchar comm);
void Send_Data(uchar Data);
void Delay(uint del);
void DLY(uchar dly);

void main(void)
{
	uchar i; EN=0;  
   Init_LCD();               //重置及清除LCD    
   LCD_Cmd(0x80);            //游標由第一行開始顯示  
   for(i=0; i< 16; i++) Send_Data(Table1[i]);    
    
   LCD_Cmd(0xC0);            //游標由第二行開始顯示   
   for(i=0; i< 16; i++) Send_Data(Table2[i]);      
   while (1) {      
      LCD_Cmd(0x0C); 
		for (i=12; i>0; i--) Delay(65000);       
      LCD_Cmd(0x08); 
		for (i=12; i>0; i--) Delay(65000);       
	}
}

void Init_LCD(void)//LCD的啟始程式  
{
	LCD_CmdI(0x30); //DL=1：8bit傳輸 
	Delay(5000);  
   LCD_CmdI(0x30);  
	Delay(200);
   LCD_CmdI(0x30);  
   LCD_CmdI(0x20); //DL=0：4bit傳輸  

   LCD_Cmd(0x28);  //DL=0：4bit傳輸 
   LCD_Cmd(0x01);  //清除顯示幕   
	Delay(5000);
   LCD_Cmd(0x0C);  //D=1：顯示幕ON,C=0：不顯示游標    
   LCD_Cmd(0x06);  //I/D=1：顯示完游標右移,S=0：游標移位禁能   
}

void LCD_CmdI(uchar comm)  //傳送命令到LCD
{
   DATA_BUS=(DATA_BUS&0x0C)|(comm&0xF3);
   RS=0; 
	EN=1; DLY(100); EN=0; DLY(100); //將資料從databus送上
}

void LCD_Cmd(uchar comm)   //傳送命令到LCD
{
   DATA_BUS=(DATA_BUS&0x0C)|(comm&0xF3);
   RS=0; EN=1; DLY(100); EN=0; DLY(100); //將資料從databus送上
   DATA_BUS=(DATA_BUS&0x0C)|((comm<<4)&0xF3);
   RS=0; 
	EN=1; DLY(100); EN=0; DLY(100); //將資料從databus送上
}

void Send_Data(uchar Data)  //傳送資料到LCD
{
   DATA_BUS=(DATA_BUS&0x0C)|(Data&0xF3);  
   RS=1; EN=1; DLY(100); EN=0; DLY(100); 
	DATA_BUS=(DATA_BUS&0x0C)|((Data<<4)&0xF3);
   RS=1; 
	EN=1; DLY(100); EN=0; DLY(100);  //將資料從databus送上
}

void Delay(uint del)      //延時函數
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

