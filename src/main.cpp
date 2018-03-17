#include<CharacterLib.h>
#include<LCD.h>
#include<GPIO.h> 
#include<stdio.h>
#include<sys.h>
//#include<math.h>
//#include<unistd.h>

#define ZONE0 0
#define ZONE1 1
int LCDFD = 0; //保存液晶文件句柄
int temp0 = 0; //保存ZONE0温度值
int temp1 = 0; //保存ZONE1温度值
int averageValue = 0; //保存CPU温度的平均值
int bootTimeNow = 0; //保存系统启动到现在的时间
int fanStatus =1; //初始变量	
double x, y; //保存CPU负载值
int tmp, num1[2], num2[2]; //分别存储CPU负载的整数值（DOUBLE类型分位存储）

void FanProg(void)
{
	
	
	temp0 = GetZoneTemp(ZONE0);
	temp1 = GetZoneTemp(ZONE1);
	averageValue = (temp0 + temp1) / 2;
	if (averageValue <= 40) return; // CPU温度低于35度，风扇停止工作
	int pwmInt = (averageValue - 35) * 10; //让PWM值在10——150之间，对应的温度为35——50度之间
	if (pwmInt > 100) pwmInt = 100;
	FanControl(pwmInt);
	
	printf("Fan PWM = %d\n",pwmInt);
    //WriteSysLog(START);

}

void DisplayInfo(int fd)
{
	//////系统运行中
	OLED_P16x16Ch(LCDFD, 16, 0, F16X16, 0);
	OLED_P16x16Ch(LCDFD, 36, 0, F16X16, 1);
	OLED_P16x16Ch(LCDFD, 56, 0, F16X16, 4);
	OLED_P16x16Ch(LCDFD, 76, 0, F16X16, 2);
	OLED_P16x16Ch(LCDFD, 96, 0, F16X16, 3);
	
	//已运行时间
	bootTimeNow = GetBootTime();
	OLED_P8x16Str(LCDFD, 0 , 2, F8X16, (bootTimeNow / DAY)/10); //XX天
	OLED_P8x16Str(LCDFD, 8, 2, F8X16, (bootTimeNow / DAY) % 10);
	OLED_P16x16Ch(LCDFD, 16, 2, F16X16, 7);
	OLED_P8x16Str(LCDFD, 32, 2, F8X16, ((bootTimeNow % DAY) / HOUR)/10);  // xx时
	OLED_P8x16Str(LCDFD, 40, 2, F8X16, ((bootTimeNow % DAY) / HOUR)%10);
	OLED_P16x16Ch(LCDFD, 48, 2, F16X16, 8);
	OLED_P8x16Str(LCDFD, 64, 2, F8X16, (((bootTimeNow % DAY) % HOUR) / MINUTE)/10);  // XX分
	OLED_P8x16Str(LCDFD, 72, 2, F8X16, (((bootTimeNow % DAY) % HOUR) / MINUTE)%10);
	OLED_P16x16Ch(LCDFD, 80, 2, F16X16, 9);
	OLED_P8x16Str(LCDFD, 96, 2, F8X16, (((bootTimeNow % DAY) % HOUR) % MINUTE)/10);  //XX秒
	OLED_P8x16Str(LCDFD, 104, 2, F8X16, (((bootTimeNow % DAY) % HOUR) % MINUTE) % 10);
	OLED_P16x16Ch(LCDFD, 112,2, F16X16, 10);
	////负载
	OLED_P16x16Ch(LCDFD, 0, 4, F16X16, 5);
	OLED_P16x16Ch(LCDFD, 16, 4, F16X16, 6);
	OLED_P8x16Str(LCDFD, 32, 4, F8X16, 11);
	
	//CPULoadToInt(&x, &y);
	GetCPUloadAVG(&x, &y);
	num1[0] = (int)x; //计算第一个浮点数的整数部分
	tmp =(int)((x-num1[0]) * 100); 
	num1[1] = tmp / 10;
	num1[2] = tmp % 10;
	
	num2[0] = (int)y;//计算第二个浮点数的整数部分
	tmp = (int)((y - num2[0]) * 100);
	num2[1] = tmp / 10;
	num2[2] = tmp % 10;
	OLED_P8x16Str(LCDFD, 40, 4, F8X16, num1[0]);
	OLED_P8x16Str(LCDFD, 48, 4, F8X16, 12);
	OLED_P8x16Str(LCDFD, 56, 4, F8X16, num1[1]);
	OLED_P8x16Str(LCDFD, 64, 4, F8X16, num1[2]);

	OLED_P8x16Str(LCDFD, 80, 4, F8X16, num2[0]);
	OLED_P8x16Str(LCDFD, 88, 4, F8X16, 12);
	OLED_P8x16Str(LCDFD, 96, 4, F8X16, num2[1]);
	OLED_P8x16Str(LCDFD, 104, 4, F8X16, num2[2]);
	////CPU温度
	OLED_P8x16Str(LCDFD, 16, 6, F8X16, 10);
	OLED_P8x16Str(LCDFD, 24, 6, F8X16, 0);
	OLED_P8x16Str(LCDFD, 32, 6, F8X16, 11);
	OLED_P8x16Str(LCDFD, 40, 6, F8X16, temp0/10);//温度值显示
	OLED_P8x16Str(LCDFD, 48, 6, F8X16, temp1%10);

	OLED_P8x16Str(LCDFD, 72, 6, F8X16, 10);
	OLED_P8x16Str(LCDFD, 80, 6, F8X16, 1);
	OLED_P8x16Str(LCDFD, 88, 6, F8X16, 11);
	OLED_P8x16Str(LCDFD, 96, 6, F8X16, temp1 / 10);//温度值显示
	OLED_P8x16Str(LCDFD, 104, 6, F8X16, temp1 % 10);
}
int main(void)
{
	daemon(0, 0);   //将程序设置为daemon模式
	printf("The SYS mornitor is running...\n");
	 
	Init_Pin(); //初始化GPIO   
	LCDFD = GetLCDFD();//猎取液晶操作句柄
	OLED_Init(LCDFD);//初始化液晶
	LCDCheckSelf(LCDFD);
	Draw_BMP(LCDFD, 0, 0, 127, 7, ubuntu);
	sleep(1);
	FanCheckSelf(); //风扇自检
	OLED_CLS(LCDFD);
	while (1)
	{

		FanProg();
		DisplayInfo(LCDFD);
		sleep(2);
	}
	printf("The SYS mornitor is stop !\n");
	return 0;
}
