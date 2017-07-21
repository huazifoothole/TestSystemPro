#ifndef LAPRINTER_H
#define LAPRINTER_H

#include "def.h"
#include <QString>
#include <QSettings>
#include <QObject>

typedef int (*InitP)(const char *input_dir, const char *output_dir);
typedef bool (*SetCutterMode)(int const mode);
typedef bool (*GetCutterMode)(int* const mode);
typedef bool (*SetFont)(unsigned char const index, unsigned char const size, unsigned char const alignment);
typedef bool (*SetFontEmpha)(unsigned char const n);
typedef bool (*SetLineSpace)(unsigned char const size);
typedef bool (*SetCharSpace)(unsigned char const size);
typedef bool (*SetLeftMargin)(int const size);
typedef void (*SetAreaWidth)(int const width);
typedef bool (*PrinterIsReady)();
typedef int (*PrintString)(const char* const str);
typedef void (*FeedLine)(unsigned char const lineCount);
typedef bool (*CutPaper)();
typedef int (*Print2DBar)(unsigned char const dotWidth, unsigned char const dotHeight, int dataRows, int dataColumns,
                         int errCorrectLevel, const char* const str, int length, int mode);
typedef bool (*SetUserChar)(unsigned char c1,unsigned char c2,unsigned char m, unsigned char * data,int length);
typedef bool (*UnsetUserChar)(unsigned char c1,unsigned char c2);
typedef bool (*PrintUserBitmap)(int xPos,int yPos, unsigned char  width,unsigned char height, const char* const bitmap);
typedef bool (*PrintBlackMark)(const char* const barCode, unsigned int const length);
typedef bool (*GetTopMargin)(int* const topMargin);
typedef int (*GetLastErrorCode)();
typedef void (*GetLastErrorStr)(char* const errStr, unsigned int const length);
typedef bool (*GetDpi)(int* const widthDpi,int* const heightDpi);
typedef int (*QueryCapability)();
typedef int (*PrintIsComplete)(int const time_out);
typedef bool (*SetAngle)(unsigned int const angle);
typedef long GetPrintLength();
typedef int (*PrintDiskImage)(int xPos,int yPos, const char* imagePath);
typedef bool (*LoadLogoImage)(int imgCount, char *imgList[]);
typedef bool (*LoadDiskLogo)(int imgCount, char* imgFileList[]);
typedef bool (*PrintLogo)(unsigned int const xpos, unsigned int const ypos, int const index);
typedef bool (*SetBottomMargin)(int const margin);
typedef bool (*Exec_ESC_POS)(char* const command, int const strLen);
typedef bool (*SetPageMode)(int width, int height, int x, int y);
typedef bool (*SetLineMode)();
typedef int (*PrintPage)();


//非体彩标准接口
//typedef int (*Print1DBar)(unsigned char width, unsigned char height, const char* str, char codeType,char setType);
//typedef int (*PrintBigMark)(char *barCode,int length);
//typedef int (*PrintSmallMark)(char *barCode, int length);
//typedef void (*CutPaperByType)(int type);
//typedef unsigned int (*PGetStatus)();
//typedef int (*WriteSerial)(const char* serial);
//typedef bool (*SetPassword)(const char* passwd);
//typedef bool (*UnsetPassword)();
//typedef bool (*GetComplement)();
//typedef bool (*Unlock)();
typedef bool (*PrinterStatusPrint)();
typedef bool (*PSetPrintDensity)(int density);



#define FLAG_ISSET(v, f) ((v) & (f))

enum report_status
{
	ST_OK = 0,
    ST_PAPER_NEAR_END = 0x1,
    ST_CUTER_ERR = 0x2,
    ST_PLATEN_OPEN = 0x4,
    ST_MECHANICAL_ERR = 0x8,
    ST_UNRECOVERABLE_ERR = 0x10,
    ST_OFFLINE = 0x20,
    ST_PAPER_END = 0x40
};

class LAPrinter : public QObject
{
	Q_OBJECT

public:
	explicit LAPrinter(QObject *parent = 0);
	~LAPrinter();

	bool LoadDriverSympol(const char* const file);//加载驱动动态库
	bool Init(const char* const input_dir, const char* const output_dir);//初始化
	bool PrintSampleTicket();//打印样票
	bool PrintTest();//打印测试
	bool PrintLengthTicket();//非定长打印
	bool PrintBitmap();//打印位图
	bool AnglePrint();//旋转打印
	bool FullCutAndHalfCut();//全切和半切
	bool PrintNTickets(char *buf, int len, int Num, bool bCut);//打印N张票
	bool PrintNLines(char *buf, unsigned lines);//打印N行
	long PrintNSeconds(char *buf, int len, int sec, bool bCut);//打印N秒钟
	unsigned int GetStatus();//获取打印机状态
	const QString& GetStatusString();
	QString GetMarkData();//获取最后打印的一张黑标数据
	QString GetPDF417Data();//获取最后打印的一张二维条码的数据
	bool GetInitStatus();
	bool GetPrinterHWInformation(char *buf, int length);
	void PrintTestBlock();
	void PrintDensityBlock();
	bool PrintBigMarkTicket();
	bool PrintSmallMarkTicket();

        bool PrintFixPDFTicket(QString &pstr);
        bool PrintNonFixLenTicket(int flag);



        InitP PrinterInit;
        PrinterIsReady printerIsReady;
        GetLastErrorStr PrinterGetLastErrorStr;
        PrintIsComplete PrinterPrintIsComplete;
        GetSWVersion PrinterGetSWVersion;
        Print2DBar PrinterPrint2DBar;
        FeedLine PrinterFeedLine;
        CutPaper PrinterCutPaper;
        QString strMarkData;
        SetCutterMode PrinterSetCutterMode;
        PrintString PrinterPrintString;



protected:
	//需要先获取锁
	bool PrintTicketHeader();//打印样票头
	//需要先获取锁
	bool PrintPDFTicketHeader(QString &pstr);//打印二维条码样票头
	//需要先获取锁
	void PrintTestStr();//打印一行字符串

private:
	void * dp_;
	pthread_mutex_t printerMutex;
	bool bmutexIsOK;
	bool bPrinterInit;
	bool bPrintInterupt;


	QString strPDF417Data;
	QString strErrorInfo;



	GetCutterMode PrinterGetCutterMode;
	SetFont PrinterSetFont;
	SetFontEmpha PrinterSetFontEmpha;
	SetLineSpace PrinterSetLineSpace;
	SetCharSpace PrinterSetCharSpace;
	SetLeftMargin PrinterSetLeftMargin;
	SetAreaWidth PrinterSetAreaWidth;

	SetUserChar PrinterSetUserChar;
	UnsetUserChar PrinterCancelUserDefChar;
	PrintUserBitmap PrinterPrintUserBitmap;
	PrintBlackMark PrinterPrintMark;
	GetTopMargin PrinterGetTopMargin;
	GetLastErrorCode PrinterGetLastErrorCode;

//        PrintBigMark PrinterPrintBigMark;
//        PrintSmallMark PrinterPrintSmallMark;
//        PGetStatus PrinterGetStatus;
//        WriteSerial PrinterWriteSerial;
//        SetPassword PrinterSetPassword;
//        UnsetPassword PrinterUnsetPassword;
//        GetComplement PrinterGetComplement;
//         Unlock PrinterUnlock;
        PrinterStatusPrint PrinterPrintStatus;
        PSetPrintDensity PrinterSetDensity;//打印浓度
//          CutPaperByType PrinterCutPaperByType;
	GetDpi PrinterGetDpi;
	QueryCapability PrinterQueryCapability;

	GetHWInformation PrinterGetHWInformation;
	SetAngle PrinterSetAngle;
	PrintDiskImage printDiskImage;
	Exec_ESC_POS PrinterExec_ESC_POS;
	SetPageMode PrinterSetPageMode;
	SetLineMode PrinterSetLineMode;
	PrintPage PrinterPrintPage;





};

#endif // LAPRINTER_H
