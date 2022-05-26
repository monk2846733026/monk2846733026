#include"MPU6050.h"
int step_cnt=0;
uchar dis_temp[2];
uchar dis[4];
int	Temperature,Temp_h,Temp_l;
//****************************************
// 定义MPU6050内部地址
//****************************************
/*#define DataPort P0		//LCD1602数据端口
#define	SMPLRT_DIV		0x19	//陀螺仪采样率，典型值：0x07(125Hz)
#define	CONFIG			0x1A	//低通滤波频率，典型值：0x06(5Hz)
#define	GYRO_CONFIG		0x1B	//陀螺仪自检及测量范围，典型值：0x18(不自检，2000deg/s)
#define	ACCEL_CONFIG	0x1C	//加速计自检、测量范围及高通滤波频率，典型值：0x01(不自检，2G，5Hz)
#define	ACCEL_XOUT_H	0x3B
#define	ACCEL_XOUT_L	0x3C
#define	ACCEL_YOUT_H	0x3D
#define	ACCEL_YOUT_L	0x3E
#define	ACCEL_ZOUT_H	0x3F
#define	ACCEL_ZOUT_L	0x40
#define	TEMP_OUT_H		0x41
#define	TEMP_OUT_L		0x42
#define	GYRO_XOUT_H		0x43
#define	GYRO_XOUT_L		0x44	
#define	GYRO_YOUT_H		0x45
#define	GYRO_YOUT_L		0x46
#define	GYRO_ZOUT_H		0x47
#define	GYRO_ZOUT_L		0x48
#define	PWR_MGMT_1		0x6B	//电源管理，典型值：0x00(正常启用)
#define	WHO_AM_I			0x75	//IIC地址寄存器(默认数值0x68，只读)
#define	SlaveAddress	0xD0	//IIC写入时的地址字节数据，+1为读取*/


/*int step_cnt=0;*/
//**************************************
//初始化MPU6050
//**************************************
void InitMPU6050()
{
	Single_WriteI2C(PWR_MGMT_1, 0x00,SlaveAddress);	//解除休眠状态 电源管理 0x00启动
	Single_WriteI2C(SMPLRT_DIV, 0x07,SlaveAddress); 	//陀螺仪采样率 默认0x07	125HZ
	Single_WriteI2C(CONFIG, 0x06,SlaveAddress);		//低通滤波频率 典型值 0x06 5HZ
	Single_WriteI2C(GYRO_CONFIG, 0x18,SlaveAddress);	//陀螺仪自检及测量范围，典型值：0x18(不自检，2000deg/s)
	//Single_WriteI2C(ACCEL_CONFIG, 0x01);
    Single_WriteI2C(ACCEL_CONFIG, 0x03,SlaveAddress); //加速计自检、测量范围及高通滤波频率，典型值：0x01(不自检，2G，5Hz)???
}
//**************************************
//合成数据
//**************************************
int GetData(uchar REG_Address)//获取高低寄存器合成数据
{
	char H,L;
	H=Single_ReadI2C(REG_Address,SlaveAddress);
	L=Single_ReadI2C(REG_Address+1,SlaveAddress);
	return (H<<8)+L;/*似乎数据的获得有点问题*/   //合成数据
}
//**************************************
//在1602上显示10位数据
//**************************************
void Display10BitData(int value,uchar x,uchar y) //？？？
{
	value/=64;							//转换为10位数据 所得数据为16位，若要转化为10位数据需除以64
	lcd_printf(dis, value);			//转换数据显示
	DisplayListChar(x,y,dis,4);/*可根据显示数据大小适当改变，同上述定义数组长度及lcd_printf函数求余数量一同变*/	//启始列，行，显示数组，显示长度 
}
//**************************************
//显示温度注意该测量出来的为华氏度 ，需要使用摄氏度转换
//**************************************
void display_temp()
{ 
	Temp_h=Single_ReadI2C(TEMP_OUT_H,SlaveAddress); //读取温度
	Temp_l=Single_ReadI2C(TEMP_OUT_L,SlaveAddress); //读取温度
	Temperature=Temp_h<<8|Temp_l;     //合成温度
	Temperature = 35+ ((double) (Temperature + 13200)) / 280; // 计算出温度
    Temperature=(Temperature-32)/1.8;
    dis_temp[0]=Temperature/10;
    dis_temp[1]=Temperature%10;
    string(0xC9,dis_temp[0]);
    string(0xCa,dis_temp[1]);
	//lcd_printf(dis,Temperature);     //转换数据显示
	//DisplayListChar(11,1,dis,4);     //启始列，行，显示数组，显示位数
}
//*********************************************************
//主程序
//*********************************************************
// ************初始化 采样结构体的值，最大变成最小，最小变成最大***********
static void peak_value_init(peak_value_t *peak)//动态阈值初始化
{
      peak->newmax.x=0;
      peak->newmax.y=0;
      peak->newmax.z=0;
     
      peak->newmin.x=2048;
      peak->newmin.y=2048;
      peak->newmin.z=2048;
     
}
static void peak_update(peak_value_t *peak,axis_info_t *cur_sample)//收集样本并更新取值 
{
       uint sample_size=0;
       sample_size ++;
      if(sample_size>SAMPLE_SIZE)
      {
            sample_size=0;/*更新50次后第五十一次应当改样例规模为1，后续考虑再改*/
            peak->oldmax=peak->newmax;
            peak->oldmin=peak->newmin;
            peak_value_init(peak);
      }
      peak->newmax.x=MAX(peak->newmax.x,cur_sample->x);
      peak->newmax.y=MAX(peak->newmax.y,cur_sample->y);
      peak->newmax.z=MAX(peak->newmax.z,cur_sample->z);
      
      peak->newmin.x=MIN(peak->newmin.x,cur_sample->x);
      peak->newmin.y=MIN(peak->newmin.y,cur_sample->y);
      peak->newmin.z=MIN(peak->newmin.y,cur_sample->z);
}
//*********8*****动态精度********

static void slid_update(slid_reg_t *slid,axis_info_t *cur_sample)//用于过滤高频噪声	 为什么要过滤高频噪声
{
    //char res=0;
    if(ABS((cur_sample->x-slid->new_sample.x))>DYNAMIC_PRECISION)
    {
        slid->old_sample.x=slid->new_sample.x;
        slid->new_sample.x=cur_sample->x;
        //res=1;
    }
    else
    {
        slid->old_sample.x=slid->new_sample.x;
    }
    //char res=0;
    if(ABS((cur_sample->y-slid->new_sample.y))>DYNAMIC_PRECISION)
    {
        slid->old_sample.y=slid->new_sample.y;
        slid->new_sample.y=cur_sample->y;
        //res=1;
    }
    else
    {
        slid->old_sample.y=slid->new_sample.y;
    }
    //char res=0;
    if(ABS((cur_sample->z-slid->new_sample.z))>DYNAMIC_PRECISION)
    {
        slid->old_sample.z=slid->new_sample.z;
        slid->new_sample.z=cur_sample->z;
        //res=1;
    }
    else
    {
        slid->old_sample.z=slid->new_sample.z;
    }
        //return res;
}

static char is_most_active(peak_value_t *peak)
{
    char res=MOST_ACTIVE_NULL;
    short x_change =ABS((peak->newmax.x-peak->newmin.x));
    short y_change =ABS((peak->newmax.y-peak->newmin.y));
    short z_change =ABS((peak->newmax.z-peak->newmin.z));
    
  
    if(x_change>y_change&&x_change>z_change&&x_change>=ACTIVE_PRECISION)
    {res=MOST_ACTIVE_X;}
    else if(y_change>x_change&&y_change>z_change&&y_change>=ACTIVE_PRECISION)
    { res=MOST_ACTIVE_Y;}
    
    else if(z_change>x_change&&z_change>y_change&&z_change>=ACTIVE_PRECISION)
    {res=MOST_ACTIVE_Z;}
    return res;
}
static int detect_step(peak_value_t *peak,slid_reg_t *slid)
{
      //char flag=0;
        
      char res=is_most_active(peak);
      switch (res)
      {
        case MOST_ACTIVE_NULL:
      {
          break;
      }
      case MOST_ACTIVE_X:
      {
        short threshold_x=(peak->oldmax.x+peak->oldmin.x)/2;
        if(slid->old_sample.x>threshold_x&&slid->new_sample.x<threshold_x)
        {
          //flag=1;
          step_cnt++;
        }
        break;
      }
      case MOST_ACTIVE_Y:
      {
          short threshold_y=(peak->oldmax.y+peak->oldmin.y)/2;
           if(slid->old_sample.y>threshold_y&&slid->new_sample.y<threshold_y)
           {
             //flag=1;
              step_cnt++;
           }
           break;
      }
      case MOST_ACTIVE_Z:
      {
          short threshold_z=(peak->oldmax.z+peak->oldmin.y)/2;
           if(slid->old_sample.z>threshold_z&&slid->new_sample.z<threshold_z)
           {
             //flag=1;
              step_cnt++;
           }
           break;
      }
      default:
        break;
      }
      return step_cnt;
      
}