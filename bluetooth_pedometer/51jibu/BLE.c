#include"BLE.h"

/*���ڳ�ʼ������*/
void UartConfiguration()  
{
    TMOD=0x20;      //���ü�����1�Ĺ�����ʽ2
    TH1=0xfd;	    //���ü�����1�ĳ�ֵ������������
    TL1=0xfd;		//���ü�����1�ĳ�ֵ������������
    PCON=0x00;      // �����ʱ���0x00���ӱ�	 0x80�ӱ�
    SCON=0x50;		//���ù�����ʽ1 ������������
    EA=1;		    //�������ж�
    ES=1;			//�������ڽ����ж�
    TR1=1;			//������1��ʼ����
}
/*��������*/
void sendDate(uchar date)
{
    SBUF=date;		  //���յ������ݷ��뷢�ͻ���������
    while(!TI);       //�ȴ������������
    TI=0;			  //���������ɱ�־λ
}

//ʮ�������ݷ���
void IntConversionChar(uint number)
{
       uint model;	//ģ
       uint numberByte; //ʮ�������ݲ�ͬλ�ϵ�ֵ
 
     for(model=1000;model>=1;model=model/10) //model��ȡֵ����ʵ����Ҫѡ��ѡ�������˷���Դ
 
      {
            numberByte = number/model;	//���ȡ��
			//��ȡ���������ݵ����λʱ���������0x30����48��תΪ�ַ��ͺ���
            if(numberByte >=1 ) sendDate((uchar)(numberByte%10+0x30));
      		//if(numberByte >=1 ) PostChar((unsigned char)(numberByte%10+48));
	  }	
}

