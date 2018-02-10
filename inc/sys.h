#ifndef __SYS_H
#define __SYS_H


//#include<stdio.h>
//#include<stdlib.h>
//#include<sys/types.h>
//#include<fcntl.h>
//#include<sys/stat.h>
//#include<unistd.h>
//#include<time.h>
//#include<unistd.h>
//#include<string.h>
//#include<wiringPi.h>


#include<sys/types.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<time.h>
#include<string.h>
#include<unistd.h>
#include<stdlib.h>
#include<stdio.h>


#define START 1
#define STOP  0

#define ZONE0 0
#define ZONE1 1

#define DAY (60*60*24)
#define HOUR (60*60)
#define MINUTE (60)

const char cpuLoadPath[] = { "/var/CPULOAD.log" };
const char logFilePath[] = { "/var/fan.log" };
const char strStart[] = { "Start...	" };
const char strStop[] =  { "Stop!		" };
const char tempFilePath0[] = { "/sys/class/thermal/thermal_zone0/temp" }; //内核0温度文件
const char tempFilePath1[] = { "/sys/class/thermal/thermal_zone1/temp" };// 内核1温度文件	
const char uptimeFilePath[] = { "/proc/uptime" };//系统启动秒数文件
const char loadavgFilePath[] = {"/proc/loadavg"};//CPU负载率文件
/**********功能描述：获取本地时间***/
/**********参数：无**/
/**********返回值：返回CHAR*类型的指针*/
extern char* GetTimeStr(void);


/**********功能描述：写入日志文件 文件位置:/var/fan.log ***/
/**********参数：stauts取值 START/STOP，**/
/**********返回值：无*/
extern void WriteSysLog(bool stauts);


/**********功能描述：获取内核温度数据 文件位置:/sys/class/thermal/thermal1/temp ***/
/**********参数：zoneN取值 ZONE0/ZONE1，**/
/**********返回值：温度数据*/
extern int GetZoneTemp(int zoneN);


/**********功能描述：获取系统自启动到现在的时间***/
/**********参数：无**/
/**********返回值：返回int类型秒数*/
extern int GetBootTime(void);


/**********功能描述：获取CPU负载数据 文件位置:/var/cpuload.log ***/
/**********参数：px,py为传入保存计算后浮点变量值**/
/**********返回值：无*/
void CPULoadToInt(double* px, double* py);


/**********功能描述：获取CPU负载数据 文件位置:/var/cpuload.log ***/
/**********参数：avg_1,avg_5为传入保存计算后浮点变量值**/
/**********返回值：无*/
void GetCPUloadAVG(double* avg_1, double* avg_5);





#endif