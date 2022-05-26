#include"tongyong.h"
#define DataPort P0		//LCD1602Êý¾Ý¶Ë¿Ú

void lcd_printf(uchar *s,int temp_data);
void delay(uint j);
void InitLcd();
void WaitForEnable(void);
void WriteCommandLCM(uchar CMD,uchar Attribc);
void WriteDataLCM(uchar dataW);
void check_busy(void);
void write_command(uchar com) ;
void write_data(uchar dat);
void string(uchar ad,uchar s);
void string_2(uchar ad,uchar *s);
void string_3(uchar ad,uchar *s);
void DisplayOneChar(uchar X,uchar Y,uchar DData);
void DisplayListChar(uchar X,uchar Y,uchar *DData,L);
