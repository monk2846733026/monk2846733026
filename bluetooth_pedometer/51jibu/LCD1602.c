#include"LCD1602.h"

sbit    LCM_RS=P2^6;		//LCD1602命令端口		
sbit    LCM_RW=P2^5;		//LCD1602命令端口		
sbit    LCM_EN=P2^7;		//LCD1602命令端口 

//****************************************
//整数转字符串
//****************************************
void lcd_printf(uchar *s,int temp_data)	//十六位 带一位符号位 
{
	if(temp_data<0)
	{
		temp_data=-temp_data;
		*s='-';
	}
	else *s=' ';
	*++s =temp_data/100+0x30;
	temp_data=temp_data%100;     //取余运算
	*++s =temp_data/10+0x30;
	temp_data=temp_data%10;      //取余运算
	*++s =temp_data+0x30; 	
}
void delay(uint j)/**/	
{						
	uchar i=250;				
	for(;j>0;j--)
	{
		while(--i);
		i=249;
		while(--i);
		i=250;
	}					
}
//****************************************
//LCD1602初始化
//****************************************
void InitLcd()				
{	
			
	WriteCommandLCM(0x38,1);	
	WriteCommandLCM(0x08,1);	
	WriteCommandLCM(0x01,1);	
	WriteCommandLCM(0x06,1);	
	WriteCommandLCM(0x0c,1);
  
}			
//****************************************
//LCD1602写允许
//****************************************
void WaitForEnable(void)/**/	
{					
	uchar dt;
	do
	{
		dt=0xff;
		LCM_EN=0;
		LCM_RS=0;	
		LCM_RW=1;
        LCM_EN=1;
        dt=DataPort;
    }while(dt&0x80);
    LCM_EN=0;		
}					
//****************************************
//LCD1602写入命令
//****************************************
void WriteCommandLCM(uchar CMD,uchar Attribc)/**/
{					
	if(Attribc)WaitForEnable();	
	LCM_EN=0;LCM_RS=0;LCM_RW=0;
	DataPort=CMD;LCM_EN=1;_nop_();	
	LCM_EN=0;delay(1);
}					
//****************************************
//LCD1602写入数据
//****************************************
void WriteDataLCM(uchar dataW)/**/
{					
	WaitForEnable();		
    LCM_EN=0;LCM_RS=1;LCM_RW=0;
	DataPort=dataW;LCM_EN=1;_nop_();	
	LCM_EN=0;delay(1);
}		

// 自己定义 LCD写命令与写数据。用数组方式实现显示计步数，按照数组个位分别显示

void check_busy(void)	    //检查忙标志函数
{
	uchar dt;
	do
	{
		dt=0xff;
		LCM_EN=0;
		LCM_RS=0;	
		LCM_RW=1;
        LCM_EN=1;
        dt=DataPort;
    }while(dt&0x80);
    LCM_EN=0;
}																				
void write_command(uchar com) 	//写命令函数
{
check_busy();
LCM_EN=0;
LCM_RS=0;
LCM_RW=0;
DataPort=com;
LCM_EN=1;
_nop_( );
LCM_EN=0;
 delay(1); 
//delay2(1);
}

void write_data(uchar dat)		 //写数据函数
{ 
	check_busy();
	LCM_EN=0;
	LCM_RS=1;
	LCM_RW=0;
	DataPort=dat;
	LCM_EN=1;
	_nop_();
	LCM_EN=0;
  	delay(1);
	//delay2(1);	
}

void string(uchar ad,uchar s)	//输出显示字符串的函数	用于显示单个数字
{
    write_command(ad);
    write_data((0x30+s));
   
}

void string_2(uchar ad,uchar *s)
{
	write_command(ad);
	while(*s>0)
	{
		write_data(*s++);
		delay(120);
	}
}
void string_3(uchar ad,uchar *s)
{
	write_command(ad);
	while(*s>0)
	{
		write_data(*s++);
		//delay(100);
	}
}
//****************************************
//LCD1602写入一个字符
//****************************************
void DisplayOneChar(uchar X,uchar Y,uchar DData)
{						
	Y&=1;						
	X&=15;						
	if(Y)X|=0x40;					
	X|=0x80;			
	WriteCommandLCM(X,0);		
	WriteDataLCM(DData);		
}						
//****************************************
//LCD1602显示字符串
//****************************************
void DisplayListChar(uchar X,uchar Y,uchar *DData,L)
{
	uchar ListLength=0; 
	Y&=0x1;                
	X&=0xF;                
	while(L--)             
	{                       
		DisplayOneChar(X,Y,DData[ListLength]);
		ListLength++;  
		X++;                        
	}    
}