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
uchar DSY_Buffer[5]={0,0,0,0,0};//显示数字(-511至512)的字符数组   

uchar p; 
axis_info_t  xdata sample;
axis_info_t  xdata cur_sample;
peak_value_t xdata peak; 
slid_reg_t   xdata slid; 
static uint Num;//步数





void main()
{ 	
  	EA=1;
	EX0=1;
	EX1=1;
	PX0=0;
	PX1=1;

	delay(500);		//上电延时	
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

//界面设计
//检查算法是否正确，以及加速度如何转换，角度如何获取和转换
//分解代码
void ext0()interrupt 0//外部中断0中断函数	 
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
	
	DisplayOneChar(0,0,'A');//列 行
	DisplayOneChar(0,1,'J');
    DisplayOneChar(1,1,'B');
    DisplayOneChar(8,1,'C');
	DisplayOneChar(15,1,'R');
	 //string(0x81,4);
	//InitLcd();		//液晶初始化
	//while(1);
	InitMPU6050();
    peak_value_init(&peak);//动态阈值结构体初始化
	//初始化MPU6050
	delay(150);
	while(1)
	{
	if(ANJIAN1==0)//按键操作 //
	{
		ANJIAN1=1;
		delay(150);
		if(ANJIAN1==0)
		{
			DisplayOneChar(15,1,'S');
			while(1)
			{
				
				/*if(ANJIAN2==0)//在暂停过程中将计步清零
				{
					step_cnt=0;
    				string(0xC2,0);	//显示的第2行字符串,，从左边第2个字符开始显示
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
					DisplayOneChar(0,0,'A');//列 行
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
  
    peak_update(&peak,&sample);//取样更新样本
    slid_update(&slid,&sample);//过滤高频噪声
    
	Display10BitData(GetData(ACCEL_XOUT_H),2,0);	//显示X轴加速度	 左列 右行
	Display10BitData(GetData(ACCEL_YOUT_H),7,0);	//显示Y轴加速度
	Display10BitData(GetData(ACCEL_ZOUT_H),12,0);  //显示Z轴加速度
    Num=detect_step(&peak,&slid);//计算步数
	Single_WriteI2C(0x00,Num,0xa0);
	   //蓝牙初始化
       //蓝牙模块数据传输  	
    UartConfiguration();
    //sendDate('1');  //发送字符1 
	IntConversionChar(Num);

	//掉电保护


    DSY_Buffer[0] = Num/10000;
    DSY_Buffer[1] = Num/1000%10;
    DSY_Buffer[2] = Num/100%10;
	DSY_Buffer[3] = Num/10%10;
	DSY_Buffer[4] = Num%10;  
    string(0xC2,DSY_Buffer[0]);	//显示的第2行字符串,，从左边第2个字符开始显示
    string(0xC3,DSY_Buffer[1]);
    string(0xC4,DSY_Buffer[2]);
    string(0xC5,DSY_Buffer[3]);
    string(0xC6,DSY_Buffer[4]);
    
    display_temp();
    
		//Display10BitData(GetData(GYRO_XOUT_H),2,1);	//显示X轴角速度
		//Display10BitData(GetData(GYRO_YOUT_H),7,1);	//显示Y轴角速度
		//Display10BitData(GetData(GYRO_ZOUT_H),12,1);	//显示Z轴角速度
	delay(500);
	}
}

void ext1()interrupt 2//外部中断1中断函数	 复位按钮
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
    date=SBUF;        //取出接受到的数据
    RI=0;
     				  //清除接受中断标志位

 
    //收到的数据是date
	
}
