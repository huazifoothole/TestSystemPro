#include "lahw.h"
#include "common.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <errno.h>
#include <dlfcn.h>
#include <dirent.h>
#include <string.h>

#include <net/if.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>

LAHW::LAHW(QObject *parent) : QObject(parent)
{
	dp_ = NULL;
}

LAHW::~LAHW()
{
}

bool LAHW::LoadDriverSympol(const char *const file)
{
	if(dp_ != NULL) dlclose(dp_);
	char *error = NULL;
    struct stat stat_buf;

    if(0 != stat(file, &stat_buf))
    {
		printf("%s not exsist\n", file);
		return false;
    }
    printf("open %s\n",file);
    dlerror();
    dp_ = dlopen(file,RTLD_LAZY);
    if( (error=dlerror()) != NULL)
    {
		printf("openso is %s\n",error);
		return false;
    }

	HWSetDirectory = (SetDirectory)dlsym(dp_, "HWInfoSetDirectory");
    if((error=dlerror()) != NULL)
    {
        printf("load HWInfoSetDirectory is %s\n", error);
        return false;
    }

    HWGetMainboardInfo = (GetMainboardInfo)dlsym(dp_, "GetMainboardInfo");
    if((error=dlerror()) != NULL)
    {
        printf("load GetMainboardInfo is %s\n", error);
        return false;
    }

    HWGetCPUInfo = (GetCPUInfo)dlsym(dp_, "GetCPUInfo");
    if((error=dlerror()) != NULL)
    {
        printf("load GetCPUInfo is %s\n", error);
        return false;
    }

    HWGetMemoryInfo = (GetMemoryInfo)dlsym(dp_, "GetMemoryInfo");
    if((error=dlerror()) != NULL)
    {
        printf("load GetMemoryInfo is %s\n", error);
        return false;
    }

    HWGetDiskInfo = (GetDiskInfo)dlsym(dp_, "GetDiskInfo");
    if((error=dlerror()) != NULL)
    {
        printf("load GetDiskInfo is %s\n", error);
        return false;
    }

    HWGetSoundCardInfo = (GetSoundCardInfo)dlsym(dp_, "GetSoundCardInfo");
    if((error=dlerror()) != NULL)
    {
        printf("load GetSoundCardInfo is %s\n", error);
        return false;
    }

    HWGetNICInfo = (GetNICInfo)dlsym(dp_, "GetNICInfo");
    if((error=dlerror()) != NULL)
    {
        printf("load GetNICInfo is %s\n", error);
        return false;
    }

    HWGetGraphicCardInfo = (GetGraphicCardInfo)dlsym(dp_, "GetGraphicCardInfo");
    if((error=dlerror()) != NULL)
    {
        printf("load GetGraphicCardInfo is %s\n", error);
        return false;
    }

    HWGetNICCount = (GetNICCount)dlsym(dp_, "GetNICCount");
    if((error=dlerror()) != NULL)
    {
        printf("load GetNICCount is %s\n", error);
        return false;
    }

    HWGetGraphicCardCount = (GetGraphicCardCount)dlsym(dp_, "GetGraphicCardCount");
    if((error=dlerror()) != NULL)
    {
        printf("load GetGraphicCardCount is %s\n", error);
        return false;
    }
/**/
    HWGetSWVersion = (GetSWVersion)dlsym(dp_, "GetSWVersion");
    if((error=dlerror()) != NULL)
    {
        printf("load GetSWVersion is %s\n", error);
        return false;
    }

    HWGetLastErrorCode = (GetLastErrorCode)dlsym(dp_, "GetLastErrorCode");
    if((error=dlerror()) != NULL)
    {
        printf("load GetLastErrorCode is %s\n", error);
        return false;
    }

    HWGetLastErrorStr = (GetLastErrorStr)dlsym(dp_, "GetLastErrorStr");
    if((error=dlerror()) != NULL)
    {
        printf("load GetLastErrorStr is %s\n", error);
        return false;
    }
	return true;
}

bool LAHW::hwSetDirectory(const char *input_dir, const char *output_dir)
{
	if(HWSetDirectory == NULL) return false;
	HWSetDirectory(input_dir, output_dir);
	return true;
}

bool LAHW::hwGetMainboardInfo(char *const info, const unsigned int length)
{
	if(HWGetMainboardInfo == NULL) return false;
	return HWGetMainboardInfo(info, length);
}

bool LAHW::hwGetCPUInfo(char *const info, const unsigned int length)
{
	if(HWGetCPUInfo == NULL) return false;
	return HWGetCPUInfo(info, length);
}

bool LAHW::hwGetMemoryInfo(char *const info, const unsigned int length)
{
	if(HWGetMemoryInfo == NULL) return false;
	return HWGetMemoryInfo(info, length);
}

bool LAHW::hwGetDiskInfo(char *const info, const unsigned int length)
{
	if(HWGetDiskInfo == NULL) return false;
	return HWGetDiskInfo(info, length);
}

bool LAHW::hwGetSoundCardInfo(char *const info, const unsigned int length)
{
	if(HWGetSoundCardInfo == NULL) return false;
	return HWGetSoundCardInfo(info, length);
}

bool LAHW::hwGetNICInfo(unsigned int number, char *const info, const unsigned int length)
{
	if(HWGetNICInfo == NULL) return false;
	return HWGetNICInfo(number, info, length);
}

bool LAHW::hwGetGraphicCardInfo(unsigned int number, char *const info, const unsigned int length)
{
	if(HWGetGraphicCardInfo == NULL) return false;
	return HWGetGraphicCardInfo(number, info, length);
}

int LAHW::hwGetNICCount()
{
	if(HWGetNICCount == NULL) return -1;
	return HWGetNICCount();
}

int LAHW::hwGetGraphicCardCount()
{
	if(HWGetGraphicCardCount == NULL) return -1;
	return HWGetGraphicCardCount();
}

bool LAHW::hwGetSWVersion(char *const swVersion, const unsigned int length)
{
	if(HWGetSWVersion == NULL) return false;
	return HWGetSWVersion(swVersion, length);
}

int LAHW::hwGetLastErrorCode()
{
	if(HWGetLastErrorCode == NULL) return -1;
	return HWGetLastErrorCode();
}

bool LAHW::hwGetLastErrorStr(char *const errStr, const unsigned int length)
{
	if(HWGetLastErrorStr == NULL) return false;
	HWGetLastErrorStr(errStr, length);
	return true;
}

bool LAHW::GetLinkStatus(QString devName, bool &retValue)
{
	struct ethtool_value edata;
	struct ifreq ifr;
	int err = 0;

	int netfd = socket(AF_INET, SOCK_DGRAM, 0);
	if(netfd < 0)
	{
		retValue = false;
		return false;
	}

	memset(&ifr, 0, sizeof(ifr));
	strcpy(ifr.ifr_name, devName.toStdString().c_str());
	edata.cmd = 0x0000000a;
	ifr.ifr_data = (caddr_t)&edata;
	err = ioctl(netfd, 0x8946, &ifr);
	close(netfd);
	if (err == 0)
	{
		retValue = edata.data ? true : false;
	}
	else if (errno != EOPNOTSUPP)
	{
		retValue = false;
		return false;
	}
	return true;
}

QString LAHW::GetLinkStatus()
{
	int netfd = socket(AF_INET, SOCK_DGRAM, 0);
	if(netfd < 0)
    {
        return tr("无法检测网卡");
	}

    QString result = "";
	struct ethtool_value edata;
    struct ifreq ifr;
	int err = 0;
	int i;

	QStringList devList = GetNetDevName();

	if(0 == devList.size())
	{
		return tr("无法检测网卡");
	}

	for(i=0; i<devList.size(); i++)
	{
		memset(&ifr, 0, sizeof(ifr));
		strcpy(ifr.ifr_name, devList.at(i).toStdString().c_str());
		edata.cmd = 0x0000000a;
		ifr.ifr_data = (caddr_t)&edata;
		err = ioctl(netfd, 0x8946, &ifr);
		if (err == 0)
		{
			result += edata.data ? (devList.at(i)+tr(":连接 "))
								 : (devList.at(i)+tr(":断开 "));
		}
		else if (errno != EOPNOTSUPP)
		{
			result += devList.at(i)+tr(":检测失败");
		}
	}
	close(netfd);
	return result;
}

QStringList LAHW::GetNetDevName()
{
	QStringList retList;
	FILE *fp = NULL;
	char buf[128], ethName[16], *p, *p1;

	retList.clear();
	fp = fopen("/proc/net/dev", "r");
	if(!fp)
	{
		return retList;
	}
	while(!feof(fp))
	{
		memset(buf, 0, sizeof(buf));
		memset(ethName, 0, sizeof(ethName));
                fgets(buf, sizeof(buf), fp);
		if((p1 = strchr(buf, ':')) && !strstr(buf, "lo"))
		{

			if((p = strstr(buf, "eth")))
			{
				strncpy(ethName, p, p1-p);
				retList<<QString(ethName);
			}

		}
	}
	fclose(fp);
	return retList;
}
