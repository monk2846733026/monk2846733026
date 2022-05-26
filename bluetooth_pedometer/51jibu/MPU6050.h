#include"tongyong.h"
#include"i2C.h"
#include"LCD1602.h"
#define DataPort P0		//LCD1602数据端口
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
#define	SlaveAddress	0xD0	//IIC写入时的地址字节数据，+1为读取
#define SAMPLE_SIZE 50
#define MAX(a,b)((a)>(b)?(a):(b))
#define MIN(a,b)((a)<(b)?(a):(b))
#define ABS(a)(0-(a))>0?((-a)):(a)
#define DYNAMIC_PRECISION  30  // 动态精度值
#define MOST_ACTIVE_NULL 0
#define MOST_ACTIVE_X    1
#define MOST_ACTIVE_Y    2
#define MOST_ACTIVE_Z    3
#define ACTIVE_PRECISION 60       //活跃轴最小判定值
typedef struct{
  short x;
  short y;
  short z;
  
}axis_info_t;                     //该结构体用于接收X,Y,Z的3轴灵敏度

//****************88*****1动态阀值

typedef struct{
      axis_info_t newmax;
      axis_info_t newmin;
      axis_info_t oldmax;
      axis_info_t oldmin;
}peak_value_t;    // 更新这一时间 与上一时间 最大最小值

typedef struct slid_reg
{
  axis_info_t new_sample;
  axis_info_t old_sample;
  
}slid_reg_t;

extern uchar dis[4];
extern int	Temperature,Temp_h,Temp_l;	//温度及高低位数据
extern uchar dis_temp[2];
extern int step_cnt;
extern uchar p; 
extern axis_info_t  xdata sample;
extern axis_info_t  xdata cur_sample;
extern peak_value_t xdata peak; 
extern slid_reg_t   xdata slid; 
extern uint Num;//步数

void InitMPU6050();
int GetData(uchar REG_Address);
void Display10BitData(int value,uchar x,uchar y);
void display_temp();
static void peak_value_init(peak_value_t *peak);
static void peak_update(peak_value_t *peak,axis_info_t *cur_sample);
static void slid_update(slid_reg_t *slid,axis_info_t *cur_sample);
static char is_most_active(peak_value_t *peak);
static int detect_step(peak_value_t *peak,slid_reg_t *slid);
