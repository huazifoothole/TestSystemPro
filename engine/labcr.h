#ifndef LABCR_H
#define LABCR_H

#include "def.h"
#include <pthread.h>
#include <QString>
#include <QThread>
#include <QObject>

typedef int (*HScannerIsReady)();
//typedef int (*ScanIsComplete)();
typedef int (*GetDataLength)(unsigned int *const Length);
typedef int (*HscannerGetTicketInfo)(unsigned char* const ticketInfo, unsigned int const bufferLen);
typedef int (*SetScanMode)(unsigned int const mode);
typedef int (*GetScanMode)(unsigned int *const mode);
typedef bool (*StartScan)();
typedef bool (*StopScan)();
typedef int (*GetImageSize)(int* const width, int* const height, int* const bufsize);
typedef int (*GetImage)(char* const image, int const bufferLen);
//typedef int (*GetHWInformation)(char *const hwInfo, int const length);
typedef int (*BCRInit)(SCANNERCALLBACK scannerCallBack, const char *input_dir, const char *output_dir);
typedef void (*Close)();
typedef bool (*AimOn)();
typedef bool (*AimOff)();
typedef bool (*Enable)();
typedef bool (*Disable)();
typedef bool (*UserLEDOn)(unsigned int * const mode);
typedef bool (*UserLEDOff)();
typedef bool (*EnableCode)(unsigned int const codeType);
typedef void (*ClearBuffer)();
typedef bool (*BCRBeep)(int const tone);

class LABCR : public QThread
{
	Q_OBJECT

public:
	explicit LABCR(QObject *parent = 0);
	~LABCR();

	bool LoadDriverSympol(const char* const file);
	bool Init(const char* const input_dir, const char* const output_dir);//初始化
	void SetStop(bool stop);
	bool GetInitStatus();
        bool SetInitStatus(bool);
	bool GetBCRHWInformation(char *info, int length);
	bool GetBCRStatus();

        Enable HscannerEnable;
        Disable HscannerDisable;
        GetLastErrorStr HscannerGetLastErrorStr;
        StopScan BarcodeScannerStopScan;
        GetSWVersion HscannerGetSWVersion;
        UserLEDOff HscannerUserLEDOff;
        AimOff HscannerAimOff;
        BCRInit BarcodeScannerInit;
        SetScanMode BarcodeScannerSetScanMode;
        EnableCode HscannerEnableCode;

protected:
	virtual void run();

private:
	void *dp_;
	bool bmutexIsOK;
	pthread_mutex_t HscannerMutex;
	bool bStop;
	bool bHscannerInit;

	HScannerIsReady BarcodeScannerHScannerIsReady;
	ScanIsComplete BarcodeScannerScanIsComplete;
	GetDataLength BarcodeScannerGetDataLength;
	HscannerGetTicketInfo BarcodeScannerGetTicketInfo;

	GetScanMode BarcodeScannerGetScanMode;
	StartScan BarcodeScannerStartScan;

	GetImageSize BarcodeScannerGetImageSize;
	GetImage BarcodeScannerGetImage;
	GetHWInformation BarcodeScannerGetHWInformation;

	Close BarcodeScannerClose;
	AimOn HscannerAimOn;



        GetLastErrorCode HscannerGetLastErrorCode;

	UserLEDOn HscannerUserLEDOn;



	ClearBuffer BCRClearBuffer;
	BCRBeep Beep;

signals:
	void HwInformation(QString info);
	void ScanFinished(int errCode, QString info);
	void ScanFinishedImage(QString imagePath);
	void BCRError(int errType, QString tips);
};

#endif // LABCR_H
