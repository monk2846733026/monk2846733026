#include"tongyong.h"
#include"i2C.h"
#include"LCD1602.h"
#define DataPort P0		//LCD1602���ݶ˿�
#define	SMPLRT_DIV		0x19	//�����ǲ����ʣ�����ֵ��0x07(125Hz)
#define	CONFIG			0x1A	//��ͨ�˲�Ƶ�ʣ�����ֵ��0x06(5Hz)
#define	GYRO_CONFIG		0x1B	//�������Լ켰������Χ������ֵ��0x18(���Լ죬2000deg/s)
#define	ACCEL_CONFIG	0x1C	//���ټ��Լ졢������Χ����ͨ�˲�Ƶ�ʣ�����ֵ��0x01(���Լ죬2G��5Hz)
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
#define	PWR_MGMT_1		0x6B	//��Դ��������ֵ��0x00(��������)
#define	WHO_AM_I			0x75	//IIC��ַ�Ĵ���(Ĭ����ֵ0x68��ֻ��)
#define	SlaveAddress	0xD0	//IICд��ʱ�ĵ�ַ�ֽ����ݣ�+1Ϊ��ȡ
#define SAMPLE_SIZE 50
#define MAX(a,b)((a)>(b)?(a):(b))
#define MIN(a,b)((a)<(b)?(a):(b))
#define ABS(a)(0-(a))>0?((-a)):(a)
#define DYNAMIC_PRECISION  30  // ��̬����ֵ
#define MOST_ACTIVE_NULL 0
#define MOST_ACTIVE_X    1
#define MOST_ACTIVE_Y    2
#define MOST_ACTIVE_Z    3
#define ACTIVE_PRECISION 60       //��Ծ����С�ж�ֵ
typedef struct{
  short x;
  short y;
  short z;
  
}axis_info_t;                     //�ýṹ�����ڽ���X,Y,Z��3��������

//****************88*****1��̬��ֵ

typedef struct{
      axis_info_t newmax;
      axis_info_t newmin;
      axis_info_t oldmax;
      axis_info_t oldmin;
}peak_value_t;    // ������һʱ�� ����һʱ�� �����Сֵ

typedef struct slid_reg
{
  axis_info_t new_sample;
  axis_info_t old_sample;
  
}slid_reg_t;

extern uchar dis[4];
extern int	Temperature,Temp_h,Temp_l;	//�¶ȼ��ߵ�λ����
extern uchar dis_temp[2];
extern int step_cnt;
extern uchar p; 
extern axis_info_t  xdata sample;
extern axis_info_t  xdata cur_sample;
extern peak_value_t xdata peak; 
extern slid_reg_t   xdata slid; 
extern uint Num;//����

void InitMPU6050();
int GetData(uchar REG_Address);
void Display10BitData(int value,uchar x,uchar y);
void display_temp();
static void peak_value_init(peak_value_t *peak);
static void peak_update(peak_value_t *peak,axis_info_t *cur_sample);
static void slid_update(slid_reg_t *slid,axis_info_t *cur_sample);
static char is_most_active(peak_value_t *peak);
static int detect_step(peak_value_t *peak,slid_reg_t *slid);
