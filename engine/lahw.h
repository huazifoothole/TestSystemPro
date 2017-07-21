#ifndef LAHW_H
#define LAHW_H

#include "def.h"
#include <QObject>

//***********************************************************************
//						硬件查询接口相关定义
//***********************************************************************

//硬件查询接口
#ifndef NO_ERROR
#define NO_ERROR	0
#endif
#ifndef TIME_OUT
#define TIME_OUT	9001
#endif
#define GET_MAINBOARD_INFO_ERROR		9002
#define GET_CPU_INFO_ERROR				9003
#define GET_MEMORY_INFO_ERROR			9004
#define GET_DISK_INFO_ERROR				9005
#define GET_SOUNDCARD_INFO_ERROR		9006
#define GET_PRIMARY_NIC_INFO_ERROR		9007
#define GET_SECOND_NIC_INFO_ERROR		9008
#define GET_GRAPHIC_CARD_INFO_ERROR		9009
#ifndef OTHER_ERROR
#define OTHER_ERROR						9500
#endif

typedef void (*SetDirectory)(const char *input_dir, const char *output_dir);
typedef bool (*GetMainboardInfo)(char* const info, unsigned int const length);
typedef bool (*GetCPUInfo)(char* const info, unsigned int const length);
typedef bool (*GetMemoryInfo)(char* const info, unsigned int const length);
typedef bool (*GetDiskInfo)(char* const info, unsigned int const length);
typedef bool (*GetSoundCardInfo)(char* const info, unsigned int const length);
typedef bool (*GetNICInfo)(unsigned int number, char* const info, unsigned int const length);
typedef bool (*GetGraphicCardInfo)(unsigned int number, char* const info, unsigned int const length);
typedef int (*GetNICCount)();
typedef int (*GetGraphicCardCount)();
//typedef bool (*HWGetSWVersion)(char* const swVersion, unsigned int const length);
typedef int (*GetLastErrorCode)();
typedef void (*GetLastErrorStr)(char* const errStr, unsigned int const length);

struct ethtool_value {
    __uint32_t      cmd;
    __uint32_t      data;
};

class LAHW : public QObject
{
	Q_OBJECT

public:
	LAHW(QObject *parent = 0);
	~LAHW();

	bool LoadDriverSympol(const char* const file);//加载驱动动态库
	bool hwSetDirectory(const char *input_dir, const char *output_dir);
	bool hwGetMainboardInfo(char* const info, unsigned int const length);
	bool hwGetCPUInfo(char* const info, unsigned int const length);
	bool hwGetMemoryInfo(char* const info, unsigned int const length);
	bool hwGetDiskInfo(char* const info, unsigned int const length);
	bool hwGetSoundCardInfo(char* const info, unsigned int const length);
	bool hwGetNICInfo(unsigned int number, char* const info, unsigned int const length);
	bool hwGetGraphicCardInfo(unsigned int number, char* const info, unsigned int const length);
	int  hwGetNICCount();
	int  hwGetGraphicCardCount();
	bool hwGetSWVersion(char* const swVersion, unsigned int const length);
	int  hwGetLastErrorCode();
	bool hwGetLastErrorStr(char* const errStr, unsigned int const length);

public:
	static QStringList GetNetDevName();
	static QString GetLinkStatus();
	static bool GetLinkStatus(QString devName, bool &retValue);

private:
	void *dp_;

	SetDirectory HWSetDirectory;
	GetMainboardInfo HWGetMainboardInfo;
	GetCPUInfo HWGetCPUInfo;
	GetMemoryInfo HWGetMemoryInfo;
	GetDiskInfo HWGetDiskInfo;
	GetSoundCardInfo HWGetSoundCardInfo;
	GetNICInfo HWGetNICInfo;
	GetGraphicCardInfo HWGetGraphicCardInfo;
	GetNICCount HWGetNICCount;
	GetGraphicCardCount HWGetGraphicCardCount;
	GetSWVersion HWGetSWVersion;
	GetLastErrorCode HWGetLastErrorCode;
	GetLastErrorStr HWGetLastErrorStr;
};

#endif // LAHW_H
