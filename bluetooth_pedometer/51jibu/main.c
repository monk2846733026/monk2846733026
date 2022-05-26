#include"tongyong.h"
#include"LCD1602.h"
#include"MPU6050.h"
#include"BLE.h"	 

sbit ANJIAN1=P3^2; 
sbit ANJIAN2=P3^3; 

uchar welcome[8]="welcome";	 
uchar to[3]="to";			 
uchar use[4]="use";			 
uchar the[4]="the";			 
uchar  pedometer[11]="pedometer!";
uchar DSY_Buffer[5]={0,0,0,0,0};//��ʾ����(-511��512)���ַ�����   

uchar p; 
axis_info_t  xdata sample;
axis_info_t  xdata cur_sample;
peak_value_t xdata peak; 
slid_reg_t   xdata slid; 
static uint Num;//����





void main()
{ 	
  	EA=1;
	EX0=1;
	EX1=1;
	PX0=0;
	PX1=1;

	delay(500);		//�ϵ���ʱ	
	InitLcd();
	string_2(0x80,&welcome);
	string_2(0x88,&to);
	string_2(0x8B,&use);
	string_2(0xc0,&the);
	string_2(0xc4,&pedometer);
	delay(1000);
	write_command(0x01);
	string_3(0x80,"Press 1 for step");
	string_3(0xc0,"count!");
	while(1)
 	{
		string_2(0xc7,".  ");
		string_2(0xc7,"   ");
		string_2(0xc7," . ");
		string_2(0xc7,"   ");
		string_2(0xc7,"  .");
		string_2(0xc7,"   ");
	}
}

//�������
//����㷨�Ƿ���ȷ���Լ����ٶ����ת�����Ƕ���λ�ȡ��ת��
//�ֽ����
void ext0()interrupt 0//�ⲿ�ж�0�жϺ���	 
{
	uint X,Y,Z,i=3000000;

    //static uint Num;write_command(0x01);
	/*if(Single_ReadI2C(0x00,0xa0)!=0)
	{*/
	
	write_command(0x01);
	string_3(0x80,"continue last?");
	string_3(0xc0,"YES 1  NO 2");
	delay(1000);
	while(1)
	{	
		if(ANJIAN1==0)
		{
		ANJIAN1=1;
		delay(150);
		if(ANJIAN1==0)
		{
			step_cnt=Single_ReadI2C(0x00,0xa0);	
			break;
		}
		}
		if(p!=0)
		{
			p=0;
			break;

		}
	}
	//while(1);
	/*}*/		
	write_command(0x01);
	string_3(0x80,"begin to count t");
	string_3(0xc0,"he steps now");
	delay(1000);
	DisplayOneChar(15,1,'1');
	delay(1000);
	DisplayOneChar(15,1,'2');
	delay(1000);
	DisplayOneChar(15,1,'3');
	delay(1000);
	DisplayOneChar(13,1,'G');
	DisplayOneChar(14,1,'O');
	DisplayOneChar(15,1,'!');
	delay(100);


	while(i--)
	{
		_nop_();
	}
	write_command(0x01);
	
	DisplayOneChar(0,0,'A');//�� ��
	DisplayOneChar(0,1,'J');
    DisplayOneChar(1,1,'B');
    DisplayOneChar(8,1,'C');
	DisplayOneChar(15,1,'R');
	 //string(0x81,4);
	//InitLcd();		//Һ����ʼ��
	//while(1);
	InitMPU6050();
    peak_value_init(&peak);//��̬��ֵ�ṹ���ʼ��
	//��ʼ��MPU6050
	delay(150);
	while(1)
	{
	if(ANJIAN1==0)//�������� //
	{
		ANJIAN1=1;
		delay(150);
		if(ANJIAN1==0)
		{
			DisplayOneChar(15,1,'S');
			while(1)
			{
				
				/*if(ANJIAN2==0)//����ͣ�����н��Ʋ�����
				{
					step_cnt=0;
    				string(0xC2,0);	//��ʾ�ĵ�2���ַ���,������ߵ�2���ַ���ʼ��ʾ
    				string(0xC3,0);
    				string(0xC4,0);
    				string(0xC5,0);
    				string(0xC6,0);
				}*/
				delay(150);
				if(ANJIAN1==0)
				{
					ANJIAN1=1;
					delay(150);
					if(ANJIAN1==0)
					{
					write_command(0x01);
					DisplayOneChar(0,0,'A');//�� ��
					DisplayOneChar(0,1,'J');
    				DisplayOneChar(1,1,'B');
   					DisplayOneChar(8,1,'C');
					DisplayOneChar(15,1,'R');
					break;
					
					}	
				}	
			}
			
		}
	}//
    X=GetData(ACCEL_XOUT_H);
    Y=GetData(ACCEL_YOUT_H);
    Z=GetData(ACCEL_ZOUT_H);
    sample.x=X;
    sample.y=Y;
    sample.z=Z;
  
    peak_update(&peak,&sample);//ȡ����������
    slid_update(&slid,&sample);//���˸�Ƶ����
    
	Display10BitData(GetData(ACCEL_XOUT_H),2,0);	//��ʾX����ٶ�	 ���� ����
	Display10BitData(GetData(ACCEL_YOUT_H),7,0);	//��ʾY����ٶ�
	Display10BitData(GetData(ACCEL_ZOUT_H),12,0);  //��ʾZ����ٶ�
    Num=detect_step(&peak,&slid);//���㲽��
	Single_WriteI2C(0x00,Num,0xa0);
	   //������ʼ��
       //����ģ�����ݴ���  	
    UartConfiguration();
    //sendDate('1');  //�����ַ�1 
	IntConversionChar(Num);

	//���籣��


    DSY_Buffer[0] = Num/10000;
    DSY_Buffer[1] = Num/1000%10;
    DSY_Buffer[2] = Num/100%10;
	DSY_Buffer[3] = Num/10%10;
	DSY_Buffer[4] = Num%10;  
    string(0xC2,DSY_Buffer[0]);	//��ʾ�ĵ�2���ַ���,������ߵ�2���ַ���ʼ��ʾ
    string(0xC3,DSY_Buffer[1]);
    string(0xC4,DSY_Buffer[2]);
    string(0xC5,DSY_Buffer[3]);
    string(0xC6,DSY_Buffer[4]);
    
    display_temp();
    
		//Display10BitData(GetData(GYRO_XOUT_H),2,1);	//��ʾX����ٶ�
		//Display10BitData(GetData(GYRO_YOUT_H),7,1);	//��ʾY����ٶ�
		//Display10BitData(GetData(GYRO_ZOUT_H),12,1);	//��ʾZ����ٶ�
	delay(500);
	}
}

void ext1()interrupt 2//�ⲿ�ж�1�жϺ���	 ��λ��ť
{		
	   p=1;	
	   step_cnt=0;
	   Single_WriteI2C(0x00,step_cnt,0xa0);	   


	   //write_command(0x01);
	   //string_2(0x80,"begin to count t");
	   //string_2(0xc0,"he steps now");
	   //write_command(0x01);
}


void Uart() interrupt 4
{
    uchar date;
    date=SBUF;        //ȡ�����ܵ�������
    RI=0;
     				  //��������жϱ�־λ

 
    //�յ���������date
	
}
