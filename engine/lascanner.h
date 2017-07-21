#ifndef LASCANNER_H
#define LASCANNER_H

#include "def.h"
#include <pthread.h>
#include <QString>
#include <QThread>
#include <QObject>

//2012-3-20号新驱动库的接口。
typedef int(*InitS)(SCANNERCALLBACK scannerCallBack(int,void *), const char *input_dir, const char *output_dir);
typedef int(*QueryCapability)();
typedef bool (*GetScanDpi)(int * const widthDpi,int * const heightDpi);
typedef bool (*GetBrandDpi)(int * const widthDpi,int * const heightDpi);
typedef int (*GetLastErrorCode)();
typedef void (*GetLastErrorStr) (char* const errStr,unsigned int const length);
typedef bool (*Start)();
typedef bool (*Stop)();
typedef bool (*Enable)();
typedef bool (*Disable)();
typedef bool (*ScannerIsReadyS)();
typedef bool (*RollBack)();
typedef bool (*GetOriginImageSize)(int * const width,int * const height, int* const bufsize);
typedef int  (*GetOriginImage)(char* const image,int const bufferlen);
typedef int  (*GetTicketInfo)(unsigned char * const ticketInfo,int const bufferlen);
typedef bool (*RecognizeItem)(int posX,int posY, int width,int height,const char* const image,char* result);
//typedef int  (*PrintBrandImage)(int width, int height, const char * const image, int index,int xpos,int ypos);
typedef int  (*PrintBrandImage)(const char * const image, int index,int xpos,int ypos);
typedef bool (*Reset)();
typedef int  (*GetStatus)();
//typedef void (*CloseA)();

class LAScanner : public QThread
{
	Q_OBJECT

public:
	explicit LAScanner(QObject *parent = 0);
	~LAScanner();
	static void MakeBrandToFile(QString content, QString filename);

	bool LoadDriverSympol(const char* const file);//加载动态库驱动
	bool Init(const char* const input_dir, const char* const output_dir);//初始化
        void SetStop(bool);//停止扫描，后台线程会退出
	/**
	 * @brief SetBrandData 设置打印标识的参数
	 * @param bprint 是否打印标志,为false时,不打印标识
	 * @param xpos 打印标识的x坐标
	 * @param ypos 打印标识的y坐标
	 * @param index 内置标识,printfile长度为0时有效
	 * @param printfile 如果长度不为0,则打印该文件
	 * @return true if success else false
	 */
	bool SetBrandData(bool bprint, int xpos, int ypos, int index, QString printfile);//设置要打印的标识
	void PrintBrand(int xpos, int ypos, int index, QString pfile);//打印标识
	/**
	 * @brief GetInitStatus
	 * @return
	 * 是否初始化成功
	 */
	bool GetInitStatus();
	/**
	 * @brief SetOnlyScanning
	 * @param OnlyScanning
	 * 设置为true时,需手动rollback,请在Scanfinished接受槽里做相应处理
	 * rollback前可以辨别扫描的数据,决定是否打印标志,即使设置了SetBrandData也不会打印标志
	 */
	void SetOnlyScanning(bool OnlyScanning);
	bool GetOnlyScanning();
	void RollBackTicket();//退票
	bool GetScannerHWInformation(char* info, int length);
	bool GetScannerStatus();
        GetSWVersion ScannerGetSWVersion;
        QString strResult2;




protected:
    virtual void run();

private:
	void * dp_;
	pthread_mutex_t scannerMutex;
	bool bmutexIsOK;

	bool bScannerInit;
	bool bStop;
	bool bPrint;
    int nXpos;
    int nYpos;
	int nIndex;
    QString printFile;
	/**
	 * bool bOnlyScanning
	 * 默认只是扫描票,可以通过设置打印标致实现打印标志:SetBrandData(...)
	 * 要实现兑奖识别,调用start前设置为true,及在ScanFinished槽里调用rollback
	 */
	bool bOnlyScanning;

	InitS ScannerInit;
	QueryCapability ScannerQueryCapability;
	GetLastErrorCode ScannerGetLastErrorCode;
	GetLastErrorStr ScannerGetLastErrorStr;
	GetScanDpi ScannerGetScanDpi;
	Start ScannerStart;
        Stop ScannerStop;
	ScanIsComplete ScannerIsComplete;
	Disable ScannerDisable;
	ScannerIsReadyS ScannerIsReady;
	RollBack ScannerRollBack;
	GetOriginImageSize ScannerGetOriginImageSize;
	GetOriginImage ScannerGetOriginImage;
	GetTicketInfo ScannerGetTicketInfo;
	RecognizeItem ScannerRecognizeItem;
	GetBrandDpi ScannerGetBrandDpi;
	PrintBrandImage BrandPrintImage;
	GetHWInformation ScannerGetHWInformation;

	Reset ScannerReset;
	GetStatus ScannerGetStatus;
	//CloseA ScannerClose;
	Enable ScannerEnable;

	void ReaderPrnImage(int nXpos, int nYpos, int nIndex, const char *pPath);
        void ParseTickerinfo(char *info, QString *strResult);//解析成X _ 形式（若为标识打印则为0101形式）
        void ParseTickerinfo2(char *info, QString *strResult);//解析info成0101形式，以str输出作为内容对比


private slots:

signals:
	void ScanFinished(int errCode, QString str);//扫描完成一张票发出的信号
	void ScannerError(int errType, QString tips);
};

#endif // LASCANNER_H
