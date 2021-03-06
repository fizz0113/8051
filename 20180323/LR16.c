/********** LR16.C ***************************   
  動作:游P2極P0輸出16為元的左右移動
	    P2.7~P2.0 P0.7~P0.0
*********************************************/ 
#include "MPC82N.H"  

#define uint unsigned int

void Delay(uint dly);

void main(void) 
{
	uint i=0x8000;
	uint j=0x8000;
   P0=P1=P2=P3=0; Delay(20000);
   while(1) {  			//重覆執行
			for(;j>0x0000;) { //判斷是否移到最右邊
	      	P2=j;          //低為元組資料由P0輸出
	         P3=j>>8;       //高為元組資料由P2輸出
	         Delay(20000);  //延遲
	         j>>=1;     	   //資料右移
	      }
			for(;i>0x0001;) { //判斷是否移到最右邊
	      	P0=i;          //低為元組資料由P0輸出
	         P1=i>>8;       //高為元組資料由P2輸出
	         Delay(20000);  //延遲
	         i>>=1;     	   //資料右移
	      }         
			for(;i<0x8000;) { //判斷是否移到最左邊
	         P0=i;          //低為元組資料由P0輸出
	         P1=i>>8;       //高為元組資料由P2輸出
	         Delay(20000);  //延遲
	         i<<=1;         //資料左移
	      }
   		for(;j<0x8000;) { //判斷是否移到最左邊
	         P2=j;          //低為元組資料由P0輸出
	         P3=j>>8;       //高為元組資料由P2輸出
	         Delay(20000);  //延遲
	         j<<=1;         //資料左移
	      }
	}
}

void Delay(uint dly)   //掃描延遲,決定數字是否會閃爍 
{
  wait:
	dly--;
	if (dly!=0) goto wait;   
}