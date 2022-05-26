#include"MPU6050.h"
int step_cnt=0;
uchar dis_temp[2];
uchar dis[4];
int	Temperature,Temp_h,Temp_l;
//****************************************
// ����MPU6050�ڲ���ַ
//****************************************
/*#define DataPort P0		//LCD1602���ݶ˿�
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
#define	SlaveAddress	0xD0	//IICд��ʱ�ĵ�ַ�ֽ����ݣ�+1Ϊ��ȡ*/


/*int step_cnt=0;*/
//**************************************
//��ʼ��MPU6050
//**************************************
void InitMPU6050()
{
	Single_WriteI2C(PWR_MGMT_1, 0x00,SlaveAddress);	//�������״̬ ��Դ���� 0x00����
	Single_WriteI2C(SMPLRT_DIV, 0x07,SlaveAddress); 	//�����ǲ����� Ĭ��0x07	125HZ
	Single_WriteI2C(CONFIG, 0x06,SlaveAddress);		//��ͨ�˲�Ƶ�� ����ֵ 0x06 5HZ
	Single_WriteI2C(GYRO_CONFIG, 0x18,SlaveAddress);	//�������Լ켰������Χ������ֵ��0x18(���Լ죬2000deg/s)
	//Single_WriteI2C(ACCEL_CONFIG, 0x01);
    Single_WriteI2C(ACCEL_CONFIG, 0x03,SlaveAddress); //���ټ��Լ졢������Χ����ͨ�˲�Ƶ�ʣ�����ֵ��0x01(���Լ죬2G��5Hz)???
}
//**************************************
//�ϳ�����
//**************************************
int GetData(uchar REG_Address)//��ȡ�ߵͼĴ����ϳ�����
{
	char H,L;
	H=Single_ReadI2C(REG_Address,SlaveAddress);
	L=Single_ReadI2C(REG_Address+1,SlaveAddress);
	return (H<<8)+L;/*�ƺ����ݵĻ���е�����*/   //�ϳ�����
}
//**************************************
//��1602����ʾ10λ����
//**************************************
void Display10BitData(int value,uchar x,uchar y) //������
{
	value/=64;							//ת��Ϊ10λ���� ��������Ϊ16λ����Ҫת��Ϊ10λ���������64
	lcd_printf(dis, value);			//ת��������ʾ
	DisplayListChar(x,y,dis,4);/*�ɸ�����ʾ���ݴ�С�ʵ��ı䣬ͬ�����������鳤�ȼ�lcd_printf������������һͬ��*/	//��ʼ�У��У���ʾ���飬��ʾ���� 
}
//**************************************
//��ʾ�¶�ע��ò���������Ϊ���϶� ����Ҫʹ�����϶�ת��
//**************************************
void display_temp()
{ 
	Temp_h=Single_ReadI2C(TEMP_OUT_H,SlaveAddress); //��ȡ�¶�
	Temp_l=Single_ReadI2C(TEMP_OUT_L,SlaveAddress); //��ȡ�¶�
	Temperature=Temp_h<<8|Temp_l;     //�ϳ��¶�
	Temperature = 35+ ((double) (Temperature + 13200)) / 280; // ������¶�
    Temperature=(Temperature-32)/1.8;
    dis_temp[0]=Temperature/10;
    dis_temp[1]=Temperature%10;
    string(0xC9,dis_temp[0]);
    string(0xCa,dis_temp[1]);
	//lcd_printf(dis,Temperature);     //ת��������ʾ
	//DisplayListChar(11,1,dis,4);     //��ʼ�У��У���ʾ���飬��ʾλ��
}
//*********************************************************
//������
//*********************************************************
// ************��ʼ�� �����ṹ���ֵ���������С����С������***********
static void peak_value_init(peak_value_t *peak)//��̬��ֵ��ʼ��
{
      peak->newmax.x=0;
      peak->newmax.y=0;
      peak->newmax.z=0;
     
      peak->newmin.x=2048;
      peak->newmin.y=2048;
      peak->newmin.z=2048;
     
}
static void peak_update(peak_value_t *peak,axis_info_t *cur_sample)//�ռ�����������ȡֵ 
{
       uint sample_size=0;
       sample_size ++;
      if(sample_size>SAMPLE_SIZE)
      {
            sample_size=0;/*����50�κ����ʮһ��Ӧ����������ģΪ1�����������ٸ�*/
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
//*********8*****��̬����********

static void slid_update(slid_reg_t *slid,axis_info_t *cur_sample)//���ڹ��˸�Ƶ����	 ΪʲôҪ���˸�Ƶ����
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