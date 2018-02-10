

#include <LCD.h>

/***************功能描述：向I2C设备写数据 *****************/
/***************参数： fd设备描述符  IIC_Data数据 *********/
void OLED_WrDat(const int fd, const int IIC_Data)
{
	wiringPiI2CWriteReg8(fd, DataReg, IIC_Data);
}


/***************功能描述：向I2C设备写命令 *****************/
/***************参数： fd设备描述符  IIC_Command命令******/
void OLED_WrCmd(const int fd, const int IIC_Command)
{
	wiringPiI2CWriteReg8(fd, CommandReg, IIC_Command);
}


/***************功能描述：设置屏幕当前指针位置 *****************/
void OLED_Set_Pos(const int fd, const int x, const int y)
{
	OLED_WrCmd(fd, 0xb0 + y);
	OLED_WrCmd(fd, ((x & 0xf0) >> 4) | 0x10);
	OLED_WrCmd(fd, (x & 0x0f) | 0x01);
}


/***************功能描述：填冲128*64像素 *****************/
void OLED_Fill(const int fd, const unsigned char bmp_dat) 
{
	int y, x;
	for (y = 0; y<8; y++)
	{
		OLED_WrCmd(fd, 0xb0 + y);
		OLED_WrCmd(fd, 0x00);
		OLED_WrCmd(fd, 0x10);
		for (x = 0; x<X_WIDTH; x++)
			OLED_WrDat(fd, bmp_dat);
	}
}


/***************功能描述：清屏 *****************/
void OLED_CLS(const int fd)
{
	OLED_Fill(fd, 0x00);
}


/***************功能描述：获取液晶操作句柄 *****************/
/***************参数： 无*****************/
/***************返回值： 操作句柄*****************/
int GetLCDFD(void)
{
	int fd;
	fd = wiringPiI2CSetup(devID);
	return fd;
}



/***************功能描述：初始化 ***********************************************************************************/
void OLED_Init(const int fd)
{
	delay(500);
	OLED_WrCmd(fd, 0xAE); //display off
	OLED_WrCmd(fd, 0x20);	//Set Memory Addressing Mode	
	OLED_WrCmd(fd, 0x10);	//00,Horizontal Addressing Mode;01,Vertical Addressing Mode;10,Page Addressing Mode (RESET);11,Invalid
	OLED_WrCmd(fd, 0xb0);	//Set Page Start Address for Page Addressing Mode,0-7
	OLED_WrCmd(fd, 0xc8);	//Set COM Output Scan Direction
	OLED_WrCmd(fd, 0x00); //---set low column address
	OLED_WrCmd(fd, 0x10); //---set high column address
	OLED_WrCmd(fd, 0x40); //--set start line address
	OLED_WrCmd(fd, 0x81); //--set contrast control register
	OLED_WrCmd(fd, 0xff); //亮度调节 0x00~0xff
	OLED_WrCmd(fd, 0xa1); //--set segment re-map 0 to 127
	OLED_WrCmd(fd, 0xa6); //--set normal display
	OLED_WrCmd(fd, 0xa8); //--set multiplex ratio(1 to 64)
	OLED_WrCmd(fd, 0x3F); //
	OLED_WrCmd(fd, 0xa4); //0xa4,Output follows RAM content;0xa5,Output ignores RAM content
	OLED_WrCmd(fd, 0xd3); //-set display offset
	OLED_WrCmd(fd, 0x00); //-not offset
	OLED_WrCmd(fd, 0xd5); //--set display clock divide ratio/oscillator frequency
	OLED_WrCmd(fd, 0xf0); //--set divide ratio
	OLED_WrCmd(fd, 0xd9); //--set pre-charge period
	OLED_WrCmd(fd, 0x22); //
	OLED_WrCmd(fd, 0xda); //--set com pins hardware configuration
	OLED_WrCmd(fd, 0x12);
	OLED_WrCmd(fd, 0xdb); //--set vcomh
	OLED_WrCmd(fd, 0x20); //0x20,0.77xVcc
	OLED_WrCmd(fd, 0x8d); //--set DC-DC enable
	OLED_WrCmd(fd, 0x14); //
	OLED_WrCmd(fd, 0xaf); //--turn on oled panel
}


/***************功能描述：自我检查  屏幕全亮 *****************/
void LCDCheckSelf(const int fd)
{
	OLED_Fill(fd, 0xff);
	delay(500);
	OLED_Fill(fd,0x00);
	delay(200);

}

/***************功能描述：显示8*16字符 ***********************************************************************************/
/***************参数：fd设备描述符   x,y显示的坐标（x,y），y为页范围0～7   buff传入的字符数组	N数据的第几组，8*16个字符为一组****/
void OLED_P8x16Str(const int fd, const int x, const int y, const unsigned char* pBuff, const int N)
{
	int wm = 0;
	int adder = 8 * N;
	OLED_Set_Pos(fd, x, y);
	for (wm = 0; wm < 4; wm++) //扫苗上半屏
	{
		OLED_WrDat(fd, *(pBuff + adder));
		adder += 1;
	}
	OLED_Set_Pos(fd, x, y + 1);

	for (wm = 0; wm<4; wm++)   //扫苗下半屏
	{
		OLED_WrDat(fd, *(pBuff + adder));
		adder += 1;
	}
}


/***************功能描述：显示16*16字符 ***********************************************************************************/
/***************参数：fd设备描述符   x,y显示的坐标（x,y），y为页范围0～7   buff传入的字符数组	N数据的第几组，16*16个字符为一组****/
void OLED_P16x16Ch(const int fd, const int x, const int y, const unsigned char* pBuff, const int N)
{
	int wm = 0;
	int adder = 32 * N;
	OLED_Set_Pos(fd, x, y);
	for (wm = 0; wm < 16; wm++) //扫苗上半屏
	{
		OLED_WrDat(fd, *(pBuff+adder));
		adder += 1;
	}
	OLED_Set_Pos(fd, x, y + 1);

	for (wm = 0; wm<16; wm++)   //扫苗下半屏
	{
		OLED_WrDat(fd, *(pBuff + adder));
		adder += 1;
	}
}


/***********功能描述：显示显示BMP图片128×64起始点坐标(x,y),x的范围0～127，y为页的范围0～7*****************/
/***************参数：fd设备描述符   x0,y0显示的起始坐标（x,y）   x1显示图片宽度0~127  y1为页范围0～7   buff传入的字符数组	***/
void Draw_BMP(const int fd, const int x0, const int y0, const int x1, const int y1,  const unsigned char* pBmp)
{
	int j = 0;
	int x, y;

	if (y1 % 8 == 0) y = y1 / 8;
	else y = y1 / 8 + 1;

	for (y = y0; y<y1; y++)
	{
		OLED_Set_Pos(fd, x0, y);
		for (x = x0; x<x1; x++)
		{
			OLED_WrDat(fd, *(pBmp+j)); //*(pBMP(j++)));
			j++;
		}
	}
}
