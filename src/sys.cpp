
#include<sys.h>
/**********功能描述：获取本地时间***/
/**********参数：无**/
/**********返回值：返回CHAR*类型的指针*/
char* GetTimeStr(void)
{
	time_t t;
	time(&t);
	tm* ptm;
	char* pTimeStr;
	ptm = localtime(&t);
	pTimeStr = asctime(ptm);
	return pTimeStr;
}

/**********功能描述：获取系统自启动到现在的时间***/
/**********参数：无**/
/**********返回值：返回int类型秒数*/
int GetBootTime(void)
{
	int bootValue = 0;
	char pBuff[200];
	int fd = open(uptimeFilePath, O_RDONLY);
	if (fd == -1)
	{
		printf("ERROR: Open the /proc/uptime file false!\n");
		return 0;
	}
	int readCount = read(fd, pBuff, sizeof(pBuff));
	close(fd); //关闭文件
	if (readCount <= 0)
	{
		printf("ERROR:read /proc/uptime file no any data!\n");
		return 0;
	}
	bootValue = atoi(pBuff); //将字符串转为INT类型
	return bootValue;
}


/**********功能描述：写入日志文件 文件位置:/var/fan.log ***/
/**********参数：stauts取值 START/STOP，**/
/**********返回值：无*/
void WriteSysLog(bool stauts)
{
	
	char* pTimeStr;
	pTimeStr = GetTimeStr();
	int fd = open(logFilePath, O_CREAT | O_WRONLY | O_APPEND, 0666);
	if (fd == -1)
	{
		printf("OPEN log file error!\n");
		return;
	}
	if (stauts) write(fd, strStart, sizeof(strStart));
	else write(fd, strStop, sizeof(strStop));
	write(fd, pTimeStr, strlen(pTimeStr));
	close(fd);
}


/**********功能描述：获取内核温度数据 文件位置:/sys/class/thermal/thermal1/temp ***/
/**********参数：zoneN取值 ZONE0/ZONE1，**/
/**********返回值：温度数据*/
int GetZoneTemp(int zoneN)
{
	int cpuFd;
	int temp;
	int readCount;
	char pBuff[7];
	switch(zoneN)
	{
		case ZONE0:
			cpuFd = open(tempFilePath0, O_RDONLY);
			if (cpuFd == -1)
			{
				printf("ERROR: Open the /sys/class/thermal/thermal_zone0/temp0 file false!\n");
				return 0;
			}
			break;
		case ZONE1:
			cpuFd = open(tempFilePath1, O_RDONLY);
			if (cpuFd == -1)
			{
				printf("ERROR: Open the /sys/class/thermal/thermal_zone0/temp1 file false!\n");
				return 0;
			}
			break;
		default:
			break;
	}
	readCount = read(cpuFd, pBuff, sizeof(pBuff));
	close(cpuFd); //关闭close(cpuFd)
	if (readCount <= 0)
	{
		printf("ERROR:read cpu temp file no any data!\n");
		return 0;
	}
	temp = atoi(pBuff); //将字符串转为INT类型
	return temp;	
}



/**********功能描述：获取CPU负载数据 文件位置:/var/cpuload.log ***/
/**********参数：*x,*y为传入保存计算后浮点变量值**/
/**********返回值：无*/
void CPULoadToInt(double* px,double* py)
{
	
	int fd;
	char pBuff[20];
	memset(pBuff, 0, sizeof(pBuff));
	pBuff[19] = '\0';
	fd  = open(cpuLoadPath, O_RDONLY);
	if (fd == -1)
	{
		printf("ERROR: Open the /var/cpuload.log file false!\n");
		return;
	}
	int readCount = read(fd, pBuff,16);
	close(fd); 
	if (readCount <= 0)
	{
		printf("ERROR:read cpuload  file no any data!\n");
		return;
	}
	*px = atof(&pBuff[0]);
	*py = atof(&pBuff[12]);
	//*py = atof(&pBuff[5]);
}

void GetCPUloadAVG(double* px, double* py)
{
	int cpuFd;
	int readCount;
	char pBuff[30];	
		cpuFd = open(loadavgFilePath, O_RDONLY);
		if (cpuFd == -1)
		{
			printf("ERROR: Open the /proc/loadavg file false!\n");
		}
	readCount = read(cpuFd, pBuff, sizeof(pBuff));
	close(cpuFd); //关闭close(cpuFd)
	if (readCount <= 0)
	{
		printf("ERROR:read /proc/loadavg file no any data!\n");
	}

	*px = atof(&pBuff[0]);
	*py = atof(&pBuff[5]);
}