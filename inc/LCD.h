
#ifndef _I2C_H
#define _I2C_H

#include<wiringPi.h>
#include<wiringPiI2C.h>

#define high 1
#define low 0
#define	Brightness	0xFF 
#define X_WIDTH 	128
#define Y_WIDTH 	64  
#define devID 0x3c   //I2C设备地址
#define DataReg 0x40 //I2C数据寄存器
#define CommandReg 0x00 //I2C命令寄存器


/***************功能描述：向I2C设备写数据 *****************/
/***************参数： fd设备描述符  IIC_Data数据 *********/
extern void OLED_WrDat(const int fd, const int IIC_Data);


/***************功能描述：向I2C设备写命令 *****************/
/***************参数： fd设备描述符  IIC_Command命令******/
extern void OLED_WrCmd(const int fd, const int IIC_Command);


/***************功能描述：设置屏幕当前指针位置 *****************/
extern void OLED_Set_Pos(const int fd, const int x, const int y);


/***************功能描述：填冲128*64像素 *****************/
extern void OLED_Fill(const int fd, const unsigned char bmp_dat);



/***************功能描述：清屏 *****************/
extern void OLED_CLS(const int fd);



/***************功能描述：获取液晶操作句柄 *****************/
/***************参数： 无*****************/
/***************返回值： 操作句柄*****************/
extern int GetLCDFD(void);




/***************功能描述：初始化 ***********************************************************************************/
extern void OLED_Init(const int fd);


/***************功能描述：自我检查  屏幕全亮 *****************/
extern void LCDCheckSelf(const int fd);


/***************功能描述：显示8*16字符 ***********************************************************************************/
/***************参数：fd设备描述符   x,y显示的坐标（x,y），y为页范围0～7   buff传入的字符数组	N数据的第几组，8*16个字符为一组****/
extern void OLED_P8x16Str(const int fd, const int x, const int y, const unsigned char* pBuff, const int N);



/***************功能描述：显示16*16字符 ***********************************************************************************/
/***************参数：fd设备描述符   x,y显示的坐标（x,y），y为页范围0～7   buff传入的字符数组	N数据的第几组，16*16个字符为一组****/
extern void OLED_P16x16Ch(const int fd, const int x, const int y, const unsigned char* pBuff, const int N);


/***********功能描述：显示显示BMP图片128×64起始点坐标(x,y),x的范围0～127，y为页的范围0～7*****************/
/***************参数：fd设备描述符   x0,y0显示的起始坐标（x,y）   x1显示图片宽度0~127  y1为页范围0～7   buff传入的字符数组	***/
extern void Draw_BMP(const int fd, const int x0, const int y0, const int x1, const int y1, const unsigned char* pBmp);

extern void DisplayInfo(int fd);

#endif
