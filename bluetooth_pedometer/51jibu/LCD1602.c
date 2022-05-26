#include"LCD1602.h"

sbit    LCM_RS=P2^6;		//LCD1602����˿�		
sbit    LCM_RW=P2^5;		//LCD1602����˿�		
sbit    LCM_EN=P2^7;		//LCD1602����˿� 

//****************************************
//����ת�ַ���
//****************************************
void lcd_printf(uchar *s,int temp_data)	//ʮ��λ ��һλ����λ 
{
	if(temp_data<0)
	{
		temp_data=-temp_data;
		*s='-';
	}
	else *s=' ';
	*++s =temp_data/100+0x30;
	temp_data=temp_data%100;     //ȡ������
	*++s =temp_data/10+0x30;
	temp_data=temp_data%10;      //ȡ������
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
//LCD1602��ʼ��
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
//LCD1602д����
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
//LCD1602д������
//****************************************
void WriteCommandLCM(uchar CMD,uchar Attribc)/**/
{					
	if(Attribc)WaitForEnable();	
	LCM_EN=0;LCM_RS=0;LCM_RW=0;
	DataPort=CMD;LCM_EN=1;_nop_();	
	LCM_EN=0;delay(1);
}					
//****************************************
//LCD1602д������
//****************************************
void WriteDataLCM(uchar dataW)/**/
{					
	WaitForEnable();		
    LCM_EN=0;LCM_RS=1;LCM_RW=0;
	DataPort=dataW;LCM_EN=1;_nop_();	
	LCM_EN=0;delay(1);
}		

// �Լ����� LCDд������д���ݡ������鷽ʽʵ����ʾ�Ʋ��������������λ�ֱ���ʾ

void check_busy(void)	    //���æ��־����
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
void write_command(uchar com) 	//д�����
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

void write_data(uchar dat)		 //д���ݺ���
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

void string(uchar ad,uchar s)	//�����ʾ�ַ����ĺ���	������ʾ��������
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
//LCD1602д��һ���ַ�
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
//LCD1602��ʾ�ַ���
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