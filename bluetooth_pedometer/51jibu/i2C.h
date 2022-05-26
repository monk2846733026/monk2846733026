#include"tongyong.h"

void Delay5us();
void I2C_Start();
void I2C_Stop();
void I2C_SendACK(bit ack);
bit I2C_RecvACK();
void I2C_SendByte(uchar dat);
uchar I2C_RecvByte();
void Single_WriteI2C(uchar REG_Address,uchar REG_data,uchar ADRESS);
uchar Single_ReadI2C(uchar REG_Address,uchar ADRESS);
