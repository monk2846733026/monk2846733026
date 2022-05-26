#include"BLE.h"

/*串口初始化配置*/
void UartConfiguration()  
{
    TMOD=0x20;      //设置计数器1的工作方式2
    TH1=0xfd;	    //设置计数器1的初值，决定波特率
    TL1=0xfd;		//设置计数器1的初值，决定波特率
    PCON=0x00;      // 波特率倍增0x00不加倍	 0x80加倍
    SCON=0x50;		//设置工作方式1 开启接受允许
    EA=1;		    //开启总中断
    ES=1;			//开启串口接受中断
    TR1=1;			//计数器1开始运行
}
/*发送数据*/
void sendDate(uchar date)
{
    SBUF=date;		  //接收到的数据放入发送缓存器发送
    while(!TI);       //等待发送数据完成
    TI=0;			  //清除发送完成标志位
}

//十进制数据发送
void IntConversionChar(uint number)
{
       uint model;	//模
       uint numberByte; //十进制数据不同位上的值
 
     for(model=1000;model>=1;model=model/10) //model的取值根据实际需要选择，选择过大会浪费资源
 
      {
            numberByte = number/model;	//相除取整
			//当取到整数数据的最高位时，将其加上0x30或者48并转为字符型后发送
            if(numberByte >=1 ) sendDate((uchar)(numberByte%10+0x30));
      		//if(numberByte >=1 ) PostChar((unsigned char)(numberByte%10+48));
	  }	
}

