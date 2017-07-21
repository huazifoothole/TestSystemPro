#include "labcr.h"
#include "common.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <errno.h>
#include <dlfcn.h>
#include <dirent.h>

LABCR::LABCR(QObject *parent) : QThread(parent)
{
	dp_ = NULL;
	//bStop = false;

        bHscannerInit = false;
}

LABCR::~LABCR()
{
        if(dp_ != NULL) dlclose(dp_);
        if(bStop==false) bStop=true;
}

bool LABCR::LoadDriverSympol(const char *const file)
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
    dp_ = dlopen(file,RTLD_LAZY);/**/
    if( (error=dlerror()) != NULL)
    {
		printf("openso is %s\n",error);
		return false;
    }
    bool loadFlag = true;
    HscannerGetSWVersion = (GetSWVersion)dlsym(dp_, "BCRGetSWVersion");
    if((error=dlerror()) != NULL)
    {
		printf("load HscannerGetSWVersion is %s\n",error);
                //return false;
                loadFlag = false;
    }

    HscannerUserLEDOn = (UserLEDOn)dlsym(dp_, "BCRUserLEDOn");
    if((error=dlerror()) != NULL)
    {
		printf("load HscannerUserLEDOn is %s\n",error);
                //return false;
                loadFlag = false;
    }

    HscannerUserLEDOff = (UserLEDOff)dlsym(dp_, "BCRUserLEDOff");
    if((error=dlerror()) != NULL)
    {
		printf("load HscannerUserLEDOff is %s\n",error);
                //return false;
                loadFlag = false;
    }

    HscannerGetLastErrorCode = (GetLastErrorCode)dlsym(dp_, "BCRGetLastErrorCode");
    if((error=dlerror()) != NULL)
    {
		printf("load HscannerGetLastErrorCode is %s\n",error);
                loadFlag = false;
//		return false;
    }

    HscannerGetLastErrorStr = (GetLastErrorStr)dlsym(dp_, "BCRGetLastErrorStr");
    if((error=dlerror()) != NULL)
    {
		printf("load HscannerGetLastErrorStr is %s\n",error);
//		return false;
                loadFlag = false;
    }

    HscannerEnable = (Enable)dlsym(dp_, "BCREnable");
    if((error=dlerror()) != NULL)
    {
		printf("load HscannerEnable is %s\n",error);
//		return false;
                loadFlag = false;
    }

    HscannerDisable = (Disable)dlsym(dp_, "BCRDisable");
    if((error=dlerror()) != NULL)
    {
		printf("load HscannerDisable is %s\n",error);
                loadFlag = false;
//		return false;
    }

    BarcodeScannerHScannerIsReady = (int (*)())dlsym(dp_,"BCRIsReady");
    if((error=dlerror()) != NULL)
    {
		printf("load BarcodeScannerHScannerIsReady is %s\n",error);
//		return false;
                loadFlag = false;
    }

    BarcodeScannerScanIsComplete = (ScanIsComplete)dlsym(dp_,"BCRScanIsComplete");
    if((error=dlerror()) != NULL)
    {
		printf("load BarcodeScannerScanIsComplete is %s\n",error);
//		return false;
                loadFlag = false;
    }

    BarcodeScannerGetDataLength = (int (*)(unsigned int *const))dlsym(dp_,"BCRGetDataLength");
    if((error=dlerror()) != NULL)
    {
		printf("load BarcodeScannerGetDataLength is %s\n",error);
//		return false;
                loadFlag = false;
    }

    BarcodeScannerGetTicketInfo = (HscannerGetTicketInfo)dlsym(dp_,"BCRGetTicketInfo");
    if((error=dlerror()) != NULL)
    {
		printf("load BarcodeScannerGetTicketInfo is %s\n",error);
//		return false;
                loadFlag = false;
    }

    BarcodeScannerSetScanMode = (int (*)(unsigned int const))dlsym(dp_,"BCRSetScanMode");
    if((error=dlerror()) != NULL)
    {
		printf("load BarcodeScannerSetScanMode is %s\n",error);
//		return false;
                loadFlag = false;
    }

    BarcodeScannerGetScanMode = (int (*)(unsigned int *const))dlsym(dp_,"BCRGetScanMode");
    if((error=dlerror()) != NULL)
    {
		printf("load BarcodeScannerGetScanMode is %s\n",error);
//		return false;
                loadFlag = false;
    }

    BarcodeScannerStartScan = (StartScan)dlsym(dp_,"BCRStartScan");
    if((error=dlerror()) != NULL)
    {
		printf("load BarcodeScannerStartScan is %s\n",error);
//		return false;
                loadFlag = false;
    }

    BarcodeScannerStopScan = (StopScan)dlsym(dp_,"BCRStopScan");
    if((error=dlerror()) != NULL)
    {
		printf("load BarcodeScannerStopScan is %s\n",error);
//		return false;
                loadFlag = false;
    }

    BarcodeScannerGetImageSize = (GetImageSize)dlsym(dp_,"BCRGetImageSize");
    if((error=dlerror()) != NULL)
    {
		printf("load BarcodeScannerGetImageSize is %s\n",error);
//		return false;
                loadFlag = false;
    }

    BarcodeScannerGetImage = (int (*)(char* const, int const))dlsym(dp_,"BCRGetImage");
    if((error=dlerror()) != NULL)
    {
		printf("load BarcodeScannerGetImage is %s\n",error);
//		return false;
                loadFlag = false;
    }

    BarcodeScannerGetHWInformation = (GetHWInformation)dlsym(dp_,"BCRGetHWInformation");
    if((error=dlerror()) != NULL)
    {
		printf("load BarcodeScannerGetHWInformation is %s\n",error);
//		return false;
                loadFlag = false;
    }

    BarcodeScannerInit = (BCRInit)dlsym(dp_,"BCRInit");
    if((error=dlerror()) != NULL)
    {
		printf("load BarcodeScannerInit is %s\n",error);
//		return false;
                loadFlag = false;
    }

    BarcodeScannerClose = (void (*)())dlsym(dp_,"BCRClose");
    if((error=dlerror()) != NULL)
    {
		printf("load BarcodeScannerClose is %s\n",error);
//		return false;
                loadFlag = false;
    }

    HscannerAimOn = (AimOn)dlsym(dp_, "BCRAimOn");
    if((error=dlerror()) != NULL)
    {
        printf("load AimOn is %s\n", error);
//        return false;
        loadFlag = false;
    }

    HscannerAimOff = (AimOff)dlsym(dp_, "BCRAimOff");
    if((error=dlerror()) != NULL)
    {
        printf("load AimOff is %s\n", error);
//        return false;
        loadFlag = false;
    }

    HscannerEnableCode = (EnableCode)dlsym(dp_, "BCREnableCode");
    if((error=dlerror()) != NULL)
    {
        printf("load EnableCode is %s\n", error);
//        return false;
        loadFlag = false;
    }

	BCRClearBuffer = (ClearBuffer)dlsym(dp_, "BCRClearBuffer");
	if((error=dlerror()) != NULL)
	{
		printf("load BCRClearBuffer is %s\n", error);
//		return false;
                loadFlag = false;
	}

	Beep = (BCRBeep)dlsym(dp_, "BCRBeep");
	if((error=dlerror()) != NULL)
	{
		printf("load BCRBeep is %s\n", error);
//		return false;
                loadFlag = false;
	}

        if(loadFlag = false)
            return false;
         else
            return true;
}

bool LABCR::Init(const char *const input_dir, const char *const output_dir)
{

	int initCount = 0;
	int ret;
//	if(!bmutexIsOK) return false;

	while(!bHscannerInit)
	{
             qDebug()<<"BCRinit count="<<initCount;
             ret = BarcodeScannerInit(NULL, input_dir, output_dir);
                      if(initCount >= 3)//初始化3次不成功，则视为设备不存在
                     {
                            qDebug()<<"BCRinit return\n";
                            system("echo \"already init 3 count Bcr init false\" >> BCRlog ");
                             system("echo $ret>> BCRlog ");
                            return false;
                     }
			if(NO_ERROR != ret)
			{
				qDebug()<<"BarcodeScannerInit error:"<<ret;
                                ret = HscannerGetLastErrorCode();
                                qDebug()<<"HscannerGetLastErrorCode:"<<ret;

			}
			else
			{
				qDebug("BarcodeScannerInit ok.\n");
				this->bHscannerInit = true;
			}

		initCount++;
                usleep(500000);
//                sleep(1);
	}
	return true;
}

void LABCR::SetStop(bool stop)
{
	bStop = stop;
}

bool LABCR::GetInitStatus()
{
	return bHscannerInit;
}

bool LABCR::SetInitStatus(bool flag)
{
         bHscannerInit = flag;
}

bool LABCR::GetBCRHWInformation(char *info, int length)
{
	if(BarcodeScannerGetHWInformation == NULL) return false;
	return BarcodeScannerGetHWInformation(info, length);
}

bool LABCR::GetBCRStatus()
{
	bool isOK;
	while(pthread_mutex_trylock(&HscannerMutex))
	{
		usleep(200000);
	}
	if(BarcodeScannerHScannerIsReady)
	{
		if(!BarcodeScannerHScannerIsReady())
		{
			isOK = false;
			qDebug("BarcodeScannerHScannerIsReady error");
			emit BCRError(TYPE_STATUS, tr("扫描枪未准备好"));
		}
		else
		{
			isOK = true;
		}
	}
	else
	{
		qDebug("BarcodeScannerHScannerIsReady null");
		pthread_mutex_unlock(&HscannerMutex);
		emit BCRError(TYPE_API_NULL, tr("API BarcodeScannerHScannerIsReady is null"));
		return false;
	}
	pthread_mutex_unlock(&HscannerMutex);
	return isOK;
}

void LABCR::run()
{
        QString	OutResult = "";
        unsigned char buffer[2048] = {0};
        if(!BarcodeScannerHScannerIsReady())
	    {           

                        if(BarcodeScannerInit(NULL, "/home/oneu/ccpos/dist/driver/la", "/home/oneu/ccpos/dist/driver/la"))
                        emit ScanFinished(-3, tr("扫描枪未准备好"));
                        //return;
	    }

            BarcodeScannerSetScanMode(2);
		if(!BarcodeScannerStartScan())
	    {
			qDebug()<<"BarcodeScannerStartScan failed!";
			return;
	    }

        emit ScanFinished(-1, tr("扫描中..."));
	bStop = false;

        while(!bStop)
	{

            while(!bStop)
            {
               if(BarcodeScannerScanIsComplete())
                {
                    break;  //跳出该循环
                }
                usleep(500000);  //500ms
            }

          //  unsigned int length;
           // if(BarcodeScannerGetDataLength(&length))
            {

                                 memset(buffer, 0, 2048*sizeof(char)); //每次在读票之前需先清空缓存数据
                                 usleep(50000);  //50ms
                                int len = BarcodeScannerGetTicketInfo(buffer, sizeof(buffer));
                                qDebug()<<"bcr getdata len ="<<len;
				BCRClearBuffer();
                            if(len == 0)
                            {
                                        qDebug("GetTicketInfo error\n"); 
                                        continue;
                            }
                                 Beep(0x0);
				if(buffer[0] == 152)
                                {
                                        OutResult.sprintf("%s", (char*)buffer + 7);
                                        qDebug()<<"OutResult ="<<OutResult;
                                        emit ScanFinished(0, OutResult);
//                                        emit ScanFinished(0, QString(G2U((char*)buffer+7)));
                                } else{
                                    //若是双条码
                                    if(buffer[7] > 1){
                                        QString OutResult0 = "";
                                        OutResult0.sprintf("%s", (char*)buffer + 11);
                                        qDebug()<<"OutResult0+11=="<<OutResult0;
                                        OutResult = OutResult0 + OutResult.sprintf("%s", (char*)buffer + 11+buffer[9]+2);//双条码，右移第一条条码所占数据位buffer[9]+2
                                        qDebug()<<"OutResult+25=="<<OutResult<<"info[0]"<<buffer[0];
                                        emit ScanFinished(0, OutResult);
                                    }else{
                                        OutResult.sprintf("%s", (char*)buffer + 11);
                                        emit ScanFinished(0, OutResult);
    //                                    emit ScanFinished(0, QString(G2U((char*)buffer+11)));
                                    }

                                }

                                qDebug()<<"GetTicketInfo:"<<OutResult;


                        }
                        /*else
			{
				char info[4096]={0};
				if(NULL != HscannerGetLastErrorStr)
				{
					HscannerGetLastErrorStr(info, sizeof(info));
					emit ScanFinished(HscannerGetLastErrorCode(), QString(info));
				}

				usleep(300000);
				continue;
                        }*/
                     usleep(500000);  //每500ms轮询一次
                }

                //HscannerUserLEDOff();
                //HscannerAimOff();
                BarcodeScannerStopScan();

    qDebug("barcode scan thread stop");
}

