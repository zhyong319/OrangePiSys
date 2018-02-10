
#include<GPIO.h>

/**********功能描述：GPIO 16 初始化设置为输出 ***/
/**********参数：无**/
/**********返回值：无*/
void Init_Pin(void)
{
	wiringPiSetup();
	pinMode(16, OUTPUT);
}



/**********功能描述：风扇自检，启动3秒，停5秒，共二个循环***/
/**********参数：  无********************************/
/**********返回值：无********************************/
void FanCheckSelf(void)
{
	digitalWrite(16, HIGH);
	sleep(3);
	digitalWrite(16, LOW);
	sleep(5);
	digitalWrite(16, HIGH);
	sleep(3);
	digitalWrite(16, LOW);
	sleep(1);
}



/**********功能描述：风扇启停控制***/
/**********参数：start =1 stop =0********************************/
/**********返回值：无********************************/
void FanControl(int sta)
{
	switch (sta)
	{
	case STOP:  //start
		digitalWrite(16, LOW); //start stop
		break;
	case START:  //stop
		digitalWrite(16, HIGH); //start fan
		break;
	default:
		digitalWrite(16, LOW);
		break;
	}
}

