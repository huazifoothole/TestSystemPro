#ifndef TESTSYSTEMUI_H
#define TESTSYSTEMUI_H

#include <QWidget>
#include <QKeyEvent>
#include <QVector>
#include <QProcess>
#include<QMessageBox>
#include<QHostInfo>
#include <qpainter.h>
#include "engine/keyboardwidget.h"
#include "engine/laprinter.h"
#include "engine/lascanner.h"
#include "engine/labcr.h"
#include "engine/lahw.h"
#include "ImageViewerServer/imageviewerserver.h"
#include "choiceotherpro.h"
#include "copysysthread.h"
#include "videoplay.h"
#include <QFileDialog>

//开发时无各个部件,请定义V_TEST
//#define V_TEST


enum StackWidgetPageIndex
{
	WAIT_TO_CHOICE = 0,
	PRINT_TICKET,
	SHUTDOWN,
	SCAN_TICKET,
	IDENTIFY_TICKET,
	UTILITY_PAGE,
	HWINFO_PAGE,
        IMAGE_PAGE,
        CHECK_STATUS
};

namespace Ui {
	class TestSystemUI;
}

class TestSystemUI : public QWidget
{
	Q_OBJECT
	
public:
	explicit TestSystemUI(QWidget *parent = 0);
	~TestSystemUI();
	bool SwitchToPageByKey(int keyValue);
	bool InitUI();
        bool InitBCRAgain();
        bool scannerTimerFlag;

        void setLabelStyle(int);


protected:
	void keyPressEvent(QKeyEvent *e);
	void keyReleaseEvent(QKeyEvent *e);
	bool InitImageViewer();
       int  netCheck(int);
       void checkMachineStatus();
       void getBcrHwinfo();
       void getScannerHwinfo();
      //virtual void paintEvent(QPaintEvent *);
protected slots:
	void KeyBoardTestEnd();
	void PrinterTimerEvent();
        void ShutdownBack();
	void HWInfoTimeout();
	void ScannerScanFinish(int errCode, QString info);
	void BCRScanFinished(int errCode, QString info);
	void MusicToRead();
        void MusicFinished();
	void MusicTimeout();
	void ImageShowTimeout();
	void QuitImageViewer();
	void BiosRead(int exitCode);
        void InitBCR();


	
private slots:
	void on_pushButtonExitSystem_clicked();//退出系统
	void on_btnScanBack_clicked();
        //void on_btnUpdateBrand_clicked();
	void on_btnUtilityPageBack_clicked();
	void on_btnHWInfoBack_clicked();
        void on_pushbuttonCheckNetCard_clicked();
	void on_pushButtonPlayMusic_clicked();
	void on_pushButtonFixTouchScreen_clicked();
	void on_btnPrinterBack_clicked();
	void on_btnPrinterTest_clicked();
        void shutdown();

       // void on_pushButton_OriginSanImage_clicked();
        void  volumeCtrl(int);

        void on_pushButton_plusVoice_clicked();

        void on_pushButton_lowVoice_clicked();

       void  enableBrandTest(bool);

       void on_btnCheckStatusBack_clicked();

       void reboot();
       void quitApp();

       void on_pushButton_toLATECHPro_clicked();

       void back_MainInterface();
       void choiceProgram(QString);

       void on_pushButton_saveini_clicked();

       void on_pushButton_print2d_clicked();

       void on_btnScanBack2_clicked();

       void on_pushButton_print_clicked();

       void on_pushButton_toChoieDlg_clicked();

       void on_btn_toLHSystem_clicked();

       void on_pushButtonThirdVideo_clicked();

        void on_pushButtonWeb_clicked();

        void lookedUp(const QHostInfo&);

      private:
	Ui::TestSystemUI *ui;
	LAPrinter *laPrinter;
	LAScanner *laScanner;
	LABCR *laBCR;
        LABCR *laHR200;
	LAHW *laHW;
	KeyboardWidget *kbWidget;
        choiceOtherPro *choicePro;
        VideoPlay *videoDlg;
        bool videoFlag;


	int iKeyboardPageIndex;
	int iShutdownTimeLeft;
        bool bScanTestStart;//黑标识别匹配第一张票标志
        bool bScanTestStart7;//七星彩投注单匹配第一张票标志
        bool isFirstTicket;//条码识别匹配第一张票标志
	QStringList pdf417Ticket;
        QString markData;
	QString star7;
        QTimer shutdownTimer, musicTimer, scannerStatusTimer,printerTimer,bcrTimer;//,hwInfoTimer
	QProcess musicProcess;
	QProcess biosProcess;
        bool printerInitFlag;
        bool scanInitFlag;
        bool bcrInitFlag;
        bool bcrHWFlag;
        bool m_format;
        bool m_copy;

         QString smallMarkData;
         QString bigMarkData;
         QString pdf417fData;
         QString ProductType;

         const char *secondVdoPath;


	//imageviewer
	ImageViewerServer imgServer;
	QList<QProcess*> proList;

        QSettings *pSettings;
        QString BIOS,BIOSReleaseDate,UUID,CPU,CPUType,MEMORY,MEMORYType,CAPACITY,DISKType,PSN,PD,PT,PHV,PF,PSV,SSN,SD,ST,SHV,SF,SSV,HSN,HD,HT,HHV,HF,HSV;

        QString m_disk1,m_disk2,m_disk3;

        QProcess copyProcess;
        QMessageBox copymessageBox;

};

#endif // TESTSYSTEMUI_H
