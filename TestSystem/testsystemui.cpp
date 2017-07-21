#include "testsystemui.h"
#include "ui_testsystemui.h"
#include <QDebug>
#include <QProcess>
#include<QFile>
#include <QtWebKit/QWebView>
#include <QDesktopServices>
#include <QNetworkConfigurationManager>
#include <QHostInfo>


TestSystemUI::TestSystemUI(QWidget *parent) :
	QWidget(parent),
        m_disk1(" "),
        m_disk2(" "),
        m_disk3(" "),
        m_format(false),
        m_copy(false),
	ui(new Ui::TestSystemUI)
{
        ui->setupUi(this);
        ui->label_falsecount->hide();
        ui->label_false->hide();
        ui->pushButton_toLATECHPro->setFocusPolicy(Qt::NoFocus);
        QPalette palette,palette2;
        palette.setColor(QPalette::WindowText,Qt::white);
        palette2.setColor(QPalette::ButtonText,Qt::white);
        ui->labelNote->setPalette(palette);//设置字体为白色
        ui->label_Tip->setPalette(palette);
        ui->pushButton_print2d->setPalette(palette2);
        ui->pushButton_saveini->setPalette(palette2);
//        ui->label_printblack->hide();
        ui->label_false->hide();
        ui->pushButton_saveini->hide();
        ui->pushButton_print2d->hide();

        kbWidget = new KeyboardWidget(this);
        connect(kbWidget, SIGNAL(KeyboardTestEnd()), this, SLOT(KeyBoardTestEnd()));
        iKeyboardPageIndex = ui->stackedWidget->addWidget(kbWidget);

        //判断机型
        QSettings TypeSetting(SO_PATH"/terminal.ini", QSettings::IniFormat);
        QFont font; font.setPointSize(18);//font.setBold(true);
         ui->label_machineType->setPalette(palette);
        ui->label_machineType->setFont(font);
        ProductType =TypeSetting.value("BASIC/product").toString();
        if( ProductType.compare("la900a-01-02") == 0 ) {

            ui->label_machineType->setText("LA900A-01-02");

        }else if( ProductType.compare("la900a-02-02") == 0 ) {

            ui->label_machineType->setText("LA900A-02-02");

        }else if( ProductType.compare("la900a-02-07") == 0 ){

            ui->label_machineType->setText("LA900A-02-07");

        }else if( ProductType.compare("la900a-02-05") == 0 ) {

            ui->label_machineType->setText("LA900A-02-05");
        }else if( ProductType.compare("la900a-02-06") == 0 ) {

            ui->label_machineType->setText("LA900A-02-06");

        }else if( ProductType.compare("la900a-02-03") == 0 ) {

            ui->label_machineType->setText("LA900A-02-03");

        }else if( ProductType.compare("la900a-02-04") == 0 ) {

            ui->label_machineType->setText("LA900A-02-04");
        }else if( ProductType.compare("la900a-02-08") == 0 ) {

            ui->label_machineType->setText("LA900A-02-08");
        }



        system("insmod /home/oneu/ccpos/dist/driver/la/modules/usbreader_fc14.ko");
        system("insmod /home/oneu/ccpos/dist/driver/la/modules/scanner_f14.ko");
        system("insmod /home/oneu/ccpos/dist/driver/la/modules/usb_datapipe.ko");
        system("insmod /home/oneu/ccpos/dist/driver/la/modules/cdc-acm.ko");
        system("sh ./hr200.sh");


        connect(ui->pbt_shutdownBack, SIGNAL(clicked()), this, SLOT(ShutdownBack()));
	connect(&musicProcess, SIGNAL(readyReadStandardOutput()), this, SLOT(MusicToRead()));
	connect(&musicProcess, SIGNAL(finished(int)), this, SLOT(MusicFinished()));
	connect(&musicTimer, SIGNAL(timeout()), this, SLOT(MusicTimeout()));
        connect(&imgServer, SIGNAL(ServerQuit()), this, SLOT(QuitImageViewer()));
	connect(&biosProcess, SIGNAL(finished(int)), this, SLOT(BiosRead(int)));
        connect(ui->pushButton_shutdown,SIGNAL(clicked()),this,SLOT(shutdown()));
        connect(ui->pushButton_reboot,SIGNAL(clicked()),this,SLOT(reboot()));
        connect(ui->pushButton_quitApp,SIGNAL(clicked()),this,SLOT(quitApp()));
        connect(ui->volumeSlider, SIGNAL(valueChanged(int)), this, SLOT(volumeCtrl(int)));
        connect(ui->cbPrintDefineBrand,SIGNAL(toggled(bool)),this,SLOT(enableBrandTest(bool)));
        choicePro = new choiceOtherPro(this);
        connect(choicePro,SIGNAL(backMainInterface()),this,SLOT(back_MainInterface()));
        connect(choicePro,SIGNAL(endPro(QString)),this,SLOT(choiceProgram(QString)));



         laPrinter = new LAPrinter();
         laScanner = new LAScanner();
         laBCR = new LABCR();
         laHW = new LAHW();


	if(laScanner)
	{
                connect(laScanner, SIGNAL(ScanFinished(int,QString)), this, SLOT(ScannerScanFinish(int,QString)));

	}
	if(laBCR)
	{
		connect(laBCR, SIGNAL(ScanFinished(int,QString)), this, SLOT(BCRScanFinished(int, QString)));
	}

        laHR200 = NULL;
        QFile mFile1("./HR200");
        if(mFile1.exists())
        {
            laHR200 = new LABCR();
            qDebug()<<"HR200 exsit ";
        }
        else
        {
            laHR200 = NULL;
            qDebug()<<"HR200 not exist  ";
        }

        if(laHR200)
        {
            connect(laHR200, SIGNAL(ScanFinished(int,QString)), this, SLOT(BCRScanFinished(int, QString)));
        }



	pdf417Ticket.clear();
        //
         // 填写7星彩X点阵.格式如下:
         // 涂上的点用"X "表示,注意X后面跟一个空格
         // 没涂上的点用"_ "表示,_后面跟一个空格

	star7 =
			"\nX _ X X _ X _ _ _ X _ _ X _ _ _ \n"
			"X _ _ _ _ _ _ _ _ _ _ _ X _ _ _ \n"
			"_ X _ _ _ _ _ _ _ _ _ X _ _ _ _ \n"
			"_ _ X _ _ _ _ _ _ _ X _ _ _ _ _ \n"
			"_ _ _ X _ _ _ _ _ X _ _ _ _ _ _ \n"
			"_ _ _ _ X _ _ _ X _ _ _ _ _ _ _ \n"
			"_ _ _ _ _ X _ X _ _ _ _ _ _ _ _ \n"
			"_ _ _ _ _ _ X _ _ _ _ _ _ _ _ _ \n"
			"_ _ _ _ _ X _ X _ _ _ _ _ _ _ _ \n"
			"_ _ _ _ X _ _ _ X _ _ _ _ _ _ _ \n"
			"_ _ _ X _ _ _ _ _ X _ _ _ _ _ _ \n"
			"_ _ X _ _ _ _ _ _ _ X _ _ _ _ _ \n"
			"_ X _ _ _ _ _ _ _ _ _ X _ _ _ _ \n"
			"X _ _ _ _ _ _ _ _ _ _ _ X _ _ _ \n"
			"_ X _ _ _ _ _ _ _ _ _ X _ _ _ _ \n"
			"_ _ X _ _ _ _ _ _ _ X _ _ _ _ _ \n"
			"_ _ _ X _ _ _ _ _ X _ _ _ _ _ _ \n"
			"_ _ _ _ X _ _ _ X _ _ _ _ _ _ _ \n"
			"_ _ _ _ _ X _ X _ _ _ _ _ _ _ _ \n"
			"_ _ _ _ _ _ X _ _ _ _ _ _ _ _ _ \n"
			"_ _ _ _ _ X _ X _ _ _ _ _ _ _ _ \n"
			"_ _ _ _ X _ _ _ X _ _ _ _ _ _ _ \n"
			"_ _ _ X _ _ _ _ _ X _ _ _ _ _ _ \n"
			"_ _ X _ _ _ _ _ _ _ X _ _ _ _ _ \n"
			"_ X _ _ _ _ _ _ _ _ _ X _ _ _ _ \n"
			"X _ _ _ _ _ _ _ _ _ _ _ X _ _ _ \n"
			"_ X _ _ _ _ _ _ _ _ _ X _ _ _ _ \n"
			"_ _ X _ _ _ _ _ _ _ X _ _ _ _ _ \n"
			"_ _ _ X _ _ _ _ _ X _ _ _ _ _ _ \n"
			"_ _ _ _ X _ _ _ X _ _ _ _ _ _ _ \n"
			"_ _ _ _ _ X _ X _ _ _ _ _ _ _ _ \n"
			"_ _ _ _ _ _ X _ _ _ _ _ _ _ _ _ \n"
			"_ _ _ _ _ X _ X _ _ _ _ _ _ _ _ \n"
			"_ _ _ _ X _ _ _ X _ _ _ _ _ _ _ \n"
			"_ _ _ X _ _ _ _ _ X _ _ _ _ _ _ \n"
			"_ _ X _ _ _ _ _ _ _ X _ _ _ _ _ ";

         bigMarkData =
                                            "\n_ X _ X _ X _ X _ X _ X _ _ _ _ \n"
                                            "_ _ _ _ _ _ X _ _ _ _ _ _ _ _ _ \n"
                                            "_ _ _ _ _ X X X _ _ _ _ _ _ _ _ \n"
                                            "_ _ _ _ X X _ X X _ _ _ _ _ _ _ \n"
                                            "_ _ _ X X _ _ _ X X _ _ _ _ _ _ \n"
                                            "_ _ X X _ _ _ _ _ X X _ _ _ _ _ \n"
                                            "_ X X _ _ _ _ _ _ _ X X _ _ _ _ \n"
                                            "X X X X X X X X X X X X _ _ _ _ ";

         smallMarkData =
                                            "\nX X _ _ _ _ _ X X X _ _ _ _ _ X X X _ _ _ _ _ X \n"
                                            "_ X X _ _ _ X X _ X X _ _ _ X X _ X X _ _ _ X X \n"
                                            "_ _ X X _ X X _ _ _ X X _ X X _ _ _ X X _ X X _ \n"
                                            "_ _ _ X X X _ _ _ _ _ _ X X _ _ _ _ _ X X X _ _ ";
         pdf417fData =   "35073110603460810054   d2WIvi   64319331\n"
                 "06034 06073 2006/03/22\n"
                 "39 21 35 03 28+05 03\n"
                 "23 20 03 14 18+29 07\n"
                 "34 11 04 16 03+21 09\n"
                 "20 34 05 16 27+19 25\n"
                 "05 13 18 27 33+17 10\n";

        ui->stackedWidget->setCurrentIndex(WAIT_TO_CHOICE);

        secondVdoPath = "./videos/";
        videoFlag = false;



}

TestSystemUI::~TestSystemUI()
{
        if(laPrinter)
        {
            delete laPrinter;
            laPrinter=NULL;
        }
        if(laScanner)
        {
            delete laScanner;
            laScanner=NULL;
        }
        if(laBCR)
        {
            delete laBCR;
            laBCR=NULL;
        }
        if(laHW)
        {
            delete laHW;
            laHW=NULL;
        }
	pdf417Ticket.clear();
        if(musicProcess.state()) musicProcess.kill();

        videoDlg->ExitPlay();
        videoDlg->close();

	delete ui;
}

bool TestSystemUI::InitUI()
{
        QString errinfo="";
        /*init printer*/
        while(laPrinter != NULL)
	{
                if(!laPrinter->LoadDriverSympol(SO_PATH"/HWILatechPrinter.so"))
		{

                    printerInitFlag = false;
                    ui->label_printerStatus->setStyleSheet("color:red");
                    ui->label_printerStatus->setText(tr("加载驱动失败"));
                    errinfo +=tr("打印机加载驱动失败");
                    ui->label_Tip->setStyleSheet("color:red");
                    ui->label_Tip->setText(errinfo);
                       break;
		}
                sleep(1);
                if(!laPrinter->Init(SO_PATH, SO_PATH))
		{
                    printerInitFlag = false;
                    char errinfo1[1024]={0};
                    laPrinter->PrinterGetLastErrorStr(errinfo1,1024);
                    qDebug("printer init == %s\n",errinfo1);
                    sleep(1);
                    for(int i=0;i<3;i++){
                        if(laPrinter->Init(SO_PATH, SO_PATH)){
                            printerInitFlag = true;
                            ui->label_printerStatus->setStyleSheet("color:black");
                            ui->label_printerStatus->setText(tr("正常"));
                            break;
                        }else {
                            sleep(1);
                        }
                    }

                    if(printerInitFlag == true)
                        break;
                    else
                        printerInitFlag = false;

                    ui->label_printerStatus->setStyleSheet("color:red");
                    ui->label_printerStatus->setText(tr("异常"));                 
                    errinfo +=tr(" 打印机异常");
                    ui->label_Tip->setStyleSheet("color:red");
                    ui->label_Tip->setText(errinfo);
                    break;
		}
                printerInitFlag = true;
                ui->label_printerStatus->setStyleSheet("color:black");
                ui->label_printerStatus->setText(tr("正常"));
               // printerTimer.start(4000);
                break;
	}

	//init scanner
	while(laScanner != NULL)
        {
		if(!laScanner->LoadDriverSympol(SO_PATH"/HWILatechScanner.so"))
		{
                    scanInitFlag = false;
                    ui->label_scanStatus->setStyleSheet("color:color");
                    ui->label_scanStatus->setText(tr("加载驱动失败"));
                    errinfo +=tr(" 读票机加载驱动失败");
                    ui->label_Tip->setStyleSheet("color:red");
                    ui->label_Tip->setText(errinfo);
			break;
		}
		if(!laScanner->Init(SO_PATH, SO_PATH))
		{
                    scanInitFlag = false;
                    ui->label_scanStatus->setStyleSheet("color:red");
                    ui->label_scanStatus->setText(tr("异常"));
                    errinfo +=tr(" 读票机异常");
                    ui->label_Tip->setStyleSheet("color:red");
                    ui->label_Tip->setText(errinfo);
                    break;
		}

                 scanInitFlag = true;
                 ui->label_scanStatus->setStyleSheet("color:black");
                 ui->label_scanStatus->setText(tr("正常"));
                 getScannerHwinfo();
                 //scannerStatusTimer.start(5000);
		break;
	}

	//init bcr


	while(laBCR != NULL)
	{
		if(!laBCR->LoadDriverSympol(SO_PATH"/HWILatechBCR.so"))
		{

                     bcrInitFlag = false;
                      ui->label_bcrStatus->setText(tr("条码枪加载驱动未完成"));
                      errinfo +=tr(" 条码枪加载驱动未完成");
                      ui->label_Tip->setStyleSheet("color:red");
                      ui->label_Tip->setText(errinfo);
		}
//                sleep(1);
                InitBCR();
                break;
        }

        while(laHR200 != NULL)
        {
                if(!laHR200->LoadDriverSympol(SO_PATH"/HWILatechHR200.so"))
                {

                      ui->label_bcrStatus->setText(tr("加载外置条码枪驱动失败"));
                      errinfo +=tr(" 加载外置条码枪驱动失败");
                      ui->label_Tip->setStyleSheet("color:red");
                      ui->label_Tip->setText(errinfo);
                      break;

                }

                if(!laHR200->Init(SO_PATH, SO_PATH))
                {

                         qDebug()<<"hr200 init false";
                          ui->label_bcrStatus->setStyleSheet("color:red");
                          ui->label_bcrStatus->setText(tr("外置条码枪异常"));
                          errinfo +=tr(" 外置条码枪异常");
                          ui->label_Tip->setStyleSheet("color:red");
                          ui->label_Tip->setText(errinfo);
                          break;
                }
                else
                    qDebug()<<"hr200 init ok";
                  break;
        }


	//init HW
	while(laHW != NULL)
	{
		if(!laHW->LoadDriverSympol(SO_PATH"/HWILatechHWINFO.so"))
		{
			break;
		}
		if(!laHW->hwSetDirectory(SO_PATH, SO_PATH))
		{
			break;
		}
		break;
	}


	return true;
}

void TestSystemUI::InitBCR()
{
    while(laBCR != NULL)
    {

           if(!laBCR->Init(SO_PATH, SO_PATH))
           {
               bcrInitFlag = false;
               if(bcrInitFlag == false)
               {
                   ui->label_bcrStatus->setStyleSheet("color:red");
                   ui->label_bcrStatus->setText(tr("异常"));
                   QString errinfo = ui->label_Tip->text();
                   errinfo +=tr(" 条码枪异常");
                   ui->label_Tip->setStyleSheet("color:red");
                   ui->label_Tip->setText(errinfo);
                   break;
               }
           }else {
               bcrInitFlag = true;
               system("echo \" Bcr init success\" >> BCRlog ");
           }

              laBCR->HscannerEnableCode(57);
              bcrInitFlag = true;
              ui->label_bcrStatus->setStyleSheet("color:black");
              ui->label_bcrStatus->setText(tr("正常"));
              getBcrHwinfo();
              break;
    }
}
bool TestSystemUI::InitBCRAgain(){
    if(!bcrInitFlag){
            if(laBCR->Init(SO_PATH, SO_PATH)){
                laBCR->HscannerEnableCode(57);
                bcrInitFlag = true;
                ui->label_bcrStatus->setStyleSheet("color:black");
                ui->label_bcrStatus->setText(tr("正常"));
                getBcrHwinfo();
            }
    }
    return true;
}

bool TestSystemUI::SwitchToPageByKey(int keyValue)
{
	if(ui->stackedWidget->currentIndex() != WAIT_TO_CHOICE) return false;
	switch(keyValue)
	{
		case Qt::Key_F1:
		{
			//键盘测试
			ui->stackedWidget->setCurrentIndex(iKeyboardPageIndex);
                        setLabelStyle(0);
			this->update();
			kbWidget->StartTest();
                        QString keyTip =  tr("  ")+tr("连续按2次 [ESC] 键—重新开始测试")+"     "+tr("连续按3次 [ESC] 键—退出键盘测试");
                        ui->labelNote->setText(keyTip);
                        ui->label_esc->hide();
			break;
		}
		case Qt::Key_F2:
		{
			//彩票打印
			ui->stackedWidget->setCurrentIndex(PRINT_TICKET);
                        setLabelStyle(1);
                        QString printerTip =  tr("  ")+tr("[Enter]—再次打印样票");
                        qApp->processEvents();
                        ui->labelNote->setText(printerTip);
			this->update();
                     //   QString printerTip = tr("[F2]:按一次将依次打印自检票一张、条码两张，长票一张.");
                      //  ui->labelNote->setText(printerTip);
                      //  int ret = laPrinter->PrinterPrintIsComplete(1);
                        if(printerInitFlag ==true)
                        {
                            if(TRUE == laPrinter->printerIsReady())
                            QTimer::singleShot(10, this, SLOT(PrinterTimerEvent()));
                             else
                             {
                               printerInitFlag = false;
                               char errinfo[1024]={0};
                               laPrinter->PrinterGetLastErrorStr(errinfo,1024);
                               qDebug("errinfo = %s\n",errinfo);
                               ui->label_printerStatus->setStyleSheet("color:red");
                               ui->label_printerStatus->setText(tr("打印机未准备好"));
                               ui->label_Tip->setText("打印机未准备好");
                               if(laPrinter->Init(SO_PATH, SO_PATH))
                               {
                                   ui->label_printerStatus->setStyleSheet("color:black");
                                   ui->label_printerStatus->setText(tr("正常"));
                                   printerInitFlag = true;
                               }
                            }

                       }

                        else
                        {
                            ui->label_printerStatus->setStyleSheet("color:red");
                            ui->label_Tip->setStyleSheet("color:red");
                            ui->label_printerStatus->setText(tr("打印机初始化失败"));
                            ui->label_Tip->setText("打印机初始化失败");
                            if(laPrinter->Init(SO_PATH, SO_PATH))
                            {
                                ui->label_printerStatus->setStyleSheet("color:black");
                                 ui->label_Tip->setStyleSheet("color:black");
                                ui->label_printerStatus->setText(tr("正常"));
                                printerInitFlag = true;
                            }
                        }			 
			break;
		}
		case Qt::Key_F3:
		{
			//读票识别
			ui->stackedWidget->setCurrentIndex(SCAN_TICKET);
                        setLabelStyle(2);
                        ui->label_Tip->setStyleSheet("color:white");
                        ui->label_scan_content->setText("");
			ui->label_Tip->setText("");
                        if(ui->cbPrintDefineBrand->isChecked())
                            laScanner->SetOnlyScanning(false);
                        else
                            laScanner->SetOnlyScanning(true);

                        this->bScanTestStart = true;//重置统计数
                        this->bScanTestStart7 =true;
                         laScanner->SetStop(false);

                        laScanner->start();
                   //     QString scannerTip = tr("  ")+tr("按 [K1] 键:投注单对比")+tr("  ")+tr("按 [K2] 键:显示原图")+"  "+tr("按 [K3] 键:自定义标记打印");
                    //    ui->labelNote->setText(scannerTip);

                       QString  strShowCount = tr("总数: 0")  +  tr("            ")
                                        +tr("正确: 0 ") +    tr("              ")
                                        +   tr("             ")
                                        + tr("正确率: 0%");
                        ui->label_false->show();
                        ui->label_falsecount->show();
                        ui->label_Tip->setText(strShowCount);
                        ui->label_falsecount->setText("0");
                        ui->label_false->setText("错误:");
                        ui->label_false->setStyleSheet("color:white");
                        ui->label_falsecount->setStyleSheet("color:white");
                        this->update();
			break;
		}
		case Qt::Key_F4:
		{
                        // 条码识别
			ui->stackedWidget->setCurrentIndex(IDENTIFY_TICKET);
                        setLabelStyle(3);
                        ui->label_Tip->setStyleSheet("color:white");
			ui->label_Tip->setText("");
                        ui->btnScanBack2->setFocus();
			this->update();

                        ui->labelBCRInfo->setText("");
                       // laBCR->HscannerEnable();
                        laBCR->SetStop(false);

                        laBCR->start();

                        if((laHR200 != NULL))
                        {
                        laHR200->HscannerEnable();
                        laHR200->BarcodeScannerSetScanMode(1);//外置条码枪设为手动模式
                        laHR200->SetStop(false);
                        laHR200->start();
                        }
                        isFirstTicket = true;//重置统计数
                        QString scannerTip = tr("  按 [F4] 键开始条码识别");
                        ui->labelNote->setText(scannerTip);

                        QString  strShowCount = tr("  总数: 0")  + tr("          ")
                                         +tr("正确: 0 ") +   tr("              ")
                                         +  tr("            ")
                                         + tr("正确率: 0%") ;
                         ui->label_false->show();
                         ui->label_falsecount->show();
                         ui->label_Tip->setText(strShowCount);
                         ui->label_falsecount->setText("0");
                         ui->label_false->setText("错误:");
                         ui->label_false->setStyleSheet("color:white");
                         ui->label_falsecount->setStyleSheet("color:white");
			break;
		}
                case Qt::Key_F5:
		{
			//图片显示
			ui->stackedWidget->setCurrentIndex(IMAGE_PAGE);
			ui->label_Tip->setText("");
			QTimer::singleShot(10, this, SLOT(ImageShowTimeout()));
                        ui->labelNote->setText("屏幕检查");
			break;
		}
		case Qt::Key_F6:
		{
			//辅助功能
			ui->stackedWidget->setCurrentIndex(UTILITY_PAGE);
                        setLabelStyle(5);
                        int value = ui->volumeSlider->value();
                        ui->label_voiceValue->setText(QString::number(value)+"%");
                        ui->label_Tip->setStyleSheet("color:white");
			QStringList devList = LAHW::GetNetDevName();
			if(devList.size() == 0)
			{
				ui->label_Tip->setText(tr("未找到网卡!"));
			}
			else
			{
				ui->label_Tip->setText("");
			}
                     //   QString scannerTip = tr("  ")+tr(" 按 [K1] 键:网络测试  ") +tr("  ")+tr("按 [K2] 键:触摸屏校准  ")+tr("  ")+tr("按 [K4] 键:音频测试.")+tr("\n")+tr("   [+]  [-] 调节音量");
                        QString scannerTip = tr("  ")+tr("[+]—加大音量   [-]—减小音量");
                        ui->labelNote->setText(scannerTip);


                              QHostInfo::lookupHost("www.baidu.com", this, SLOT(lookedUp(QHostInfo)));
                              ui->webView->load(QUrl("http://sports.sina.com.cn"));
                              ui->webView->show();

			this->update();      
			break;
		}
                case Qt::Key_F7:
		{
			//硬件信息
			ui->stackedWidget->setCurrentIndex(HWINFO_PAGE);
                        setLabelStyle(6);
                        qDebug()<<"bcrHWFlag =="<<bcrHWFlag;
                         if(bcrHWFlag == false)
                        {
                            if(laBCR->BarcodeScannerInit(NULL, SO_PATH, SO_PATH))
                                qDebug()<<"hwinfo init bcr false";
                            else
                                 qDebug()<<"hwinfo init bcr ok";
                                getBcrHwinfo();
                        }


			ui->label_Tip->setText("");
			this->update();
			QTimer::singleShot(10, this, SLOT(HWInfoTimeout()));
                        QString scannerTip = tr("  ")+tr("按 [K1] 打印硬件信息二维码");
                        ui->labelNote->setText(scannerTip);
//                        ui->pushButton_saveini->show();
                        ui->pushButton_print2d->show();
                        //hwInfoTimer.start(1000);
                        break;
		}
                case Qt::Key_F8://检查部件状态
		{
                        ui->stackedWidget->setCurrentIndex(CHECK_STATUS);
                        setLabelStyle(7);
                        checkMachineStatus();
                        QString scannerTip = tr("查看各部件状态");
                        ui->labelNote->setText(scannerTip);
                        break;
		}
                case Qt::Key_F9:
                {
                     /*QProcess::startDetached("./choiceDialog",QStringList());
                     if(laPrinter) delete laPrinter;
                     if(laScanner) delete laScanner;
                     if(laBCR) delete laBCR;
                     if(laHW) delete laHW;
                     pdf417Ticket.clear();
                     if(musicProcess.state()) musicProcess.kill();
                     qApp->quit();*/
                    setLabelStyle(8);
                    choicePro->show();

                     break;
                }
                case Qt::Key_F10://关机
                 {
                 ui->stackedWidget->setCurrentIndex(SHUTDOWN);
                 setLabelStyle(9);
                 QString scannerTip = tr("按组合键 [体彩+0] 切换至演示程序并作为下次开机启动程序");
                 ui->labelNote->setText(scannerTip);
                 break;
                 }
                default:
                    break;
                                ;
	}
	return true;
}

void TestSystemUI::lookedUp(const QHostInfo &host){
    if(host.error() != QHostInfo::NoError )
    {
        qDebug("web false");
        ui->textEdit_web->show();
    }else {
         qDebug("web true");
         ui->textEdit_web->hide();
    }
}

void TestSystemUI::keyPressEvent(QKeyEvent *e)
{

	if(kbWidget->TestIsStarted())
	{
		kbWidget->KeyPress(e);
		return;
	}
	else if(ui->stackedWidget->currentIndex() == UTILITY_PAGE)
	{

                if(e->key() == Qt::Key_H)
                {
                    ui->pushbuttonCheckNetCard->setStyleSheet("background-color:rgb(172, 195, 219);");
                 //   ui->label_checknet->setStyleSheet("background-color:rgb(172, 195, 219); border-style: outset;");
                    ui->label_checknet->setStyleSheet("background-color:rgb(172, 195, 219);");
                    ui->pushButtonFixTouchScreen->setStyleSheet(" ");
                    ui->pushButtonPlayMusic->setStyleSheet(" ");
                    ui->label_TouchScreen->setStyleSheet("  ");
                    ui->label_PlayMusic->setStyleSheet("  ");
                }
                if(e->key() == Qt::Key_I)
                {
                     ui->pushButtonFixTouchScreen->setStyleSheet("background-color: rgb(172, 195, 219);");
                     ui->label_TouchScreen->setStyleSheet("background-color:rgb(172, 195, 219);");
                     ui->pushbuttonCheckNetCard->setStyleSheet(" ");
                     ui->pushButtonPlayMusic->setStyleSheet(" ");
                     ui->label_checknet->setStyleSheet("  ");
                     ui->label_PlayMusic->setStyleSheet("  ");

                }
                if(e->key() == Qt::Key_J)
                {
                    ui->pushButtonPlayMusic->setStyleSheet("background-color: rgb(172, 195, 219);");
                    ui->label_PlayMusic->setStyleSheet("background-color:rgb(172, 195, 219);");
                    ui->pushbuttonCheckNetCard->setStyleSheet(" ");
                    ui->pushButtonFixTouchScreen->setStyleSheet(" ");
                    ui->label_checknet->setStyleSheet("  ");
                    ui->label_TouchScreen->setStyleSheet("  ");
                }

		return;
	}

	else
	{
		if(!SwitchToPageByKey(e->key()))
		{
#ifdef V_TEST
			qDebug()<<"ignore press event";
#endif
			e->ignore();
			return;
		}
	}
	return;
}

void TestSystemUI::keyReleaseEvent(QKeyEvent *e)
{

	if(kbWidget->TestIsStarted())
	{
		kbWidget->keyRelease(e);
	}
	else if(ui->stackedWidget->currentIndex() == UTILITY_PAGE)
	{

                if(e->key() == Qt::Key_H)
                {
                    ui->pushbuttonCheckNetCard->setStyleSheet("background-color:rgb(172, 195, 219);");
                  //  ui->label_checknet->setStyleSheet("background-color:rgb(172, 195, 219); border-style: outset;");
                    ui->label_checknet->setStyleSheet("background-color:rgb(172, 195, 219); border-style: outset;");
                    ui->pushButtonFixTouchScreen->setStyleSheet(" ");
                    ui->pushButtonPlayMusic->setStyleSheet(" ");
                    ui->label_TouchScreen->setStyleSheet("  ");
                    ui->label_PlayMusic->setStyleSheet("  ");
                }
                if(e->key() == Qt::Key_I)
                {
                    ui->pushButtonFixTouchScreen->setStyleSheet("background-color: rgb(172, 195, 219);");
                    ui->label_TouchScreen->setStyleSheet("background-color:rgb(172, 195, 219); border-style: outset;");
                    ui->pushbuttonCheckNetCard->setStyleSheet(" ");
                    ui->pushButtonPlayMusic->setStyleSheet(" ");
                    ui->label_checknet->setStyleSheet("  ");
                    ui->label_PlayMusic->setStyleSheet("  ");
                }
                if(e->key() == Qt::Key_J)
                {
                    ui->pushButtonPlayMusic->setStyleSheet("background-color: rgb(172, 195, 219);");
                    ui->label_PlayMusic->setStyleSheet("background-color:rgb(172, 195, 219); border-style: outset;");
                    ui->pushbuttonCheckNetCard->setStyleSheet(" ");
                    ui->pushButtonFixTouchScreen->setStyleSheet(" ");
                    ui->label_checknet->setStyleSheet("  ");
                    ui->label_TouchScreen->setStyleSheet("  ");
                }

		return;
	}
	else
	{
		return;
	}
}

void TestSystemUI::KeyBoardTestEnd()
{
        //ui->label_F1->setStyleSheet("background:url(:/images/active_off.png)");
        ui->label_F1->setStyleSheet("background-color:rgb(0,43,84)");
       // ui->label_Tip->setText("键盘测试结束!");
	ui->stackedWidget->setCurrentIndex(WAIT_TO_CHOICE);
        ui->label_esc->show();
        ui->labelNote->setText("");
}

void TestSystemUI::ShutdownBack()
{
       // ui->label_F10->setStyleSheet("background:url(:/images/active_off.png)");
        ui->label_F10->setStyleSheet("background-color:rgb(0,43,84)");
        ui->labelNote->setText("");
        ui->stackedWidget->setCurrentIndex(WAIT_TO_CHOICE);
        this->update();

}

void TestSystemUI::ImageShowTimeout()
{
	if(!InitImageViewer()) QuitImageViewer();
}

void TestSystemUI::on_pushButtonExitSystem_clicked()
{
    if(laPrinter) delete laPrinter;
    if(laScanner) delete laScanner;
    if(laBCR) delete laBCR;
    if(laHW) delete laHW;
    pdf417Ticket.clear();
    if(musicProcess.state()) musicProcess.kill();
    close();
}



void TestSystemUI::shutdown()
{
    QMessageBox msgBox;
    int msg=msgBox.information(this,"warning", "确认关机？", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
  //  msgBox.setButtonText(QMessageBox::Yes,"确定");
  //  msgBox.setButtonText(QMessageBox::No,"取消");
    if(msg == QMessageBox::Yes)
    {
        qDebug()<<"yes";
        system("shutdown -h now");
        qApp->quit();

    }
    else
        qDebug()<<"no";
}

void TestSystemUI::reboot()
{
    QMessageBox msgBox;
    int msg=msgBox.information(this,"warning", "确认重启？", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
  //  msgBox.setButtonText(QMessageBox::Yes,"确定");
  //  msgBox.setButtonText(QMessageBox::No,"取消");
    if(msg == QMessageBox::Yes)
    {
        system("reboot");
        qApp->quit();

    }
    else
        qDebug()<<"no";

}

void TestSystemUI::quitApp()
{
    QMessageBox msgBox;
    int msg=msgBox.information(this,"warning", "确认退出软件？", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
  //  msgBox.setButtonText(QMessageBox::Yes,"确定");
  //  msgBox.setButtonText(QMessageBox::No,"取消");
    if(msg == QMessageBox::Yes)
    {
        if(laPrinter)
        {
            delete laPrinter;
            laPrinter=NULL;
        }
        if(laScanner)
        {
            delete laScanner;
            laScanner=NULL;
        }
        if(laBCR)
        {
            delete laBCR;
            laBCR=NULL;
        }
        if(laHW)
        {
            delete laHW;
            laHW=NULL;
        }
        pdf417Ticket.clear();
        if(musicProcess.state()) musicProcess.kill();
        qApp->closeAllWindows();

    }
    else
        qDebug()<<"no";


}

void  TestSystemUI::volumeCtrl(int value)
{
    //value = ui->volumeSlider->value();
    ui->label_voiceValue->setText(QString::number(value));
    QString volumeCmd;
    volumeCmd = "amixer set Master " + QString::number(value) +"%";
    ui->label_voiceValue->setText(QString::number(value)+"%");
    system(volumeCmd.toLatin1().data());
}

void TestSystemUI::on_pushButton_plusVoice_clicked()
{
    int value = ui->volumeSlider->value();
    value +=5;
    ui->volumeSlider->setValue(value);
    QString volumeCmd;
    volumeCmd = "amixer set Master " + QString::number(8) +"%+";
    system(volumeCmd.toLatin1().data());
}

void TestSystemUI::on_pushButton_lowVoice_clicked()
{
    int value = ui->volumeSlider->value();
    value -=5;
    if(value>=0)
    ui->volumeSlider->setValue(value);
    QString volumeCmd;
    volumeCmd = "amixer set Master " + QString::number(5) +"%-";
    system(volumeCmd.toLatin1().data());
}

void TestSystemUI::enableBrandTest(bool on)
{
    if(ui->cbPrintDefineBrand->isChecked())
    {

        laScanner->SetOnlyScanning(false);//设置了后记得手动rollback,打印标识调用printbrand

    }
    else
    {
          laScanner->SetOnlyScanning(true);
    }
}



void TestSystemUI::on_pushButton_toLATECHPro_clicked()
{
    QMessageBox msgBox;
    int msg=msgBox.information(this,"warning", "下次开机将进入<<市场演示程序>>", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);


    if(msg == QMessageBox::Yes)
    {

        system("ln -s /root/TestSystem/changeLATECH.sh  /root/TestSystem/localsetup  -f");
        qApp->closeAllWindows();

         exit(2);
    }
    else
        qDebug()<<"no";
}

void TestSystemUI::back_MainInterface()
{
  //  ui->label_F9->setStyleSheet("background:url(:/images/active_off.png)");
    ui->label_F9->setStyleSheet("background-color:rgb(0,43,84)");
    ui->stackedWidget->setCurrentIndex(WAIT_TO_CHOICE);
    ui->labelNote->setText("");
}

void TestSystemUI::choiceProgram(QString str)
{
        qApp->closeAllWindows();
        if(str.compare("sc") == 0 ) {
            exit(3);
//        qDebug("setup shenchan Pro");
//        QProcess::startDetached("/root/TestSystem/startTestSystem.sh",QStringList());
//        qApp->processEvents();
    }
     else if(str.compare("kaoji") == 0 ) {
          qDebug("setup kaoji ==");
          exit(1);
     }
     else if(str.compare("LATECHTest") == 0 ) {
         qDebug("setup LATECHTest Pro");
          exit(2);
     }
     else
     {
           QMessageBox msgBox;
            msgBox.information(this,tr("出错"), "未找到相应程序", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
     }

//     qApp->quit();

}

void TestSystemUI::setLabelStyle(int i)
{
    QLabel  *labelUiTabel[] =
       {
          ui->label_F1, ui->label_F2, ui->label_F3, ui->label_F4, ui->label_F5, ui->label_F6,
          ui->label_F7, ui->label_F8, ui->label_F9, ui->label_F10
       };
  //  labelUiTabel[i]->setStyleSheet("background:url(:/images/active_on.png)");
      labelUiTabel[i]->setStyleSheet("background-color:rgb(234,120,8)");
}



void TestSystemUI::on_btnScanBack2_clicked()
{

   // ui->label_F3->setStyleSheet("background:url(:/images/active_off.png)");
  //  ui->label_F4->setStyleSheet("background:url(:/images/active_off.png)");
    ui->label_F3->setStyleSheet("background-color:rgb(0,43,84)");
    ui->label_F4->setStyleSheet("background-color:rgb(0,43,84)");
    laBCR->SetStop(true);
    ui->label_Tip->setText("");
    ui->label_falsecount->hide();
    ui->label_false->hide();
    ui->stackedWidget->setCurrentIndex(WAIT_TO_CHOICE);
    ui->labelNote->setText("");
    this->update();
}

void TestSystemUI::on_pushButton_print_clicked()
{
    if(printerInitFlag ==true)
    {
        if(TRUE == laPrinter->printerIsReady())
       {
           QTimer::singleShot(10, this, SLOT(PrinterTimerEvent()));
            qApp->processEvents();
       }
         else
         {
           printerInitFlag = false;
           char errinfo[1024]={0};
           laPrinter->PrinterGetLastErrorStr(errinfo,1024);
           qDebug("errinfo = %s\n",errinfo);
           ui->label_printerStatus->setStyleSheet("color:red");
           ui->label_printerStatus->setText(tr("打印机未准备好"));
           ui->label_Tip->setText("打印机未准备好");
           if(laPrinter->Init(SO_PATH, SO_PATH))
           {
               ui->label_printerStatus->setStyleSheet("color:black");
               ui->label_printerStatus->setText(tr("正常"));
               printerInitFlag = true;
           }
        }

   }

    else
    {
        ui->label_printerStatus->setStyleSheet("color:red");
        ui->label_Tip->setStyleSheet("color:red");
        ui->label_printerStatus->setText(tr("打印机初始化失败"));
        ui->label_Tip->setText("打印机初始化失败");
        if(laPrinter->Init(SO_PATH, SO_PATH))
        {
            ui->label_printerStatus->setStyleSheet("color:black");
             ui->label_Tip->setStyleSheet("color:black");
            ui->label_printerStatus->setText(tr("正常"));
            printerInitFlag = true;
        }
    }
    this->update();
}

void TestSystemUI::getBcrHwinfo()
{
    char info[4096];
    memset(info, 0, sizeof(info));

    char infoBCR[1024]={0};

    {
            if(!laBCR->HscannerGetSWVersion(infoBCR,1024))
            {
                laBCR->BarcodeScannerInit(NULL, "/home/oneu/ccpos/dist/driver/la", "/home/oneu/ccpos/dist/driver/la");
                qDebug()<<"get SWvesion false";
            }

            else
                qDebug()<<"get SWvesion ok";

            bool hwflag = laBCR->GetBCRHWInformation(info, sizeof(info));
            {
                if(!hwflag)//再次初始化
                {
                     laBCR->BarcodeScannerInit(NULL, "/home/oneu/ccpos/dist/driver/la", "/home/oneu/ccpos/dist/driver/la");
                     hwflag = laBCR->GetBCRHWInformation(info, sizeof(info));
                }
            }
            if(hwflag)
            {

                    QString strInfo = info;
                    QStringList tmpList= strInfo.split('\n');
                    QString HShowinfo,str2="";
                    for(int i=0; i<tmpList.size(); i++)
                    {
                            QStringList  list2 = tmpList.at(i).split('=');

                            if(list2.size() ==2 && list2.at(0) =="SN")
                            {
                                HSN=list2.at(1);
                            }
                            if(list2.size() ==2 && list2.at(0) == "ProductDate")
                            {
                                HD=list2.at(1);
                            }

                            if(list2.size() ==2 && list2.at(0) == "ProductType")
                            {
                                HT=list2.at(1);
                            }
                            if(list2.size() ==2 && list2.at(0) == "HWVersion")
                            {
                                HHV=list2.at(1);
                            }

                            if(list2.size() == 2 && list2.at(0) == "FirmwareVersion")
                            {
                                    char infoBCR[100]={0};
                                    laBCR->HscannerGetSWVersion(infoBCR,100);
                                    HF=list2.at(1);
                                    HSV=QString(infoBCR);
                                     str2="\n"+tr("驱动版本  : ")+QString(infoBCR)+"\n"+tr("固件版本  : ")+list2.at(1)+"\n";

                            }

                    }

                    if(HF == " ")
                    {
                        bcrHWFlag=false;
                    }
                    else
                        bcrHWFlag=true;

                    HShowinfo +="\n"\
                               +tr("硬件型号  : " )+HT+"\n"\
                               +tr("硬件版本  : ")+HHV+"\n"\
                               +tr("硬件序号  : ")+HSN+str2;

                     ui->labelBCRHWInfo->setText(HShowinfo);
                     ui->labelBCRHWInfo->setStyleSheet("color:black");

            }
            else
            {
                    bcrHWFlag=false;
                    qDebug()<<"GetBCRHWInformation failed";
                     ui->labelBCRHWInfo->setText(tr("获取硬件信息失败"));
                     ui->labelBCRHWInfo->setStyleSheet("color:red");
            }
    }

}

void TestSystemUI::getScannerHwinfo()
{
    if(laScanner->GetInitStatus())
    {
           char info[4096];
            memset(info, 0, sizeof(info));
            QString SShowinfo,str2="";
            if(laScanner->GetScannerHWInformation(info, sizeof(info)))
            {
                    qDebug()<<info<<"\n";
                    QString strInfo = info;
                    QStringList tmpList= strInfo.split('\n');
                    for(int i=0; i<tmpList.size(); i++)
                    {
                            QStringList  list2 = tmpList.at(i).split('=');

                            if(list2.size() ==2 && list2.at(0) == "SN")
                            {
                                SSN=list2.at(1);
                                qDebug()<<"SSN="<<SSN<<"\n";
                            }
                            if(list2.size() ==2 && list2.at(0) == "ProductDate")
                            {
                                SD=list2.at(1);
                            }

                            if(list2.size() ==2 && list2.at(0) == "ProductType")
                            {
                                ST=list2.at(1);
                            }
                            if(list2.size() ==2 && list2.at(0) == "HWVersion")
                            {
                                SHV=list2.at(1);
                            }

                            if(list2.size() == 2 && list2.at(0) == "FirmwareVersion")
                            {
                                    char info[100]={0};
                                    laScanner->ScannerGetSWVersion(info,100);
                                    SF=list2.at(1);
                                    SSV=QString(info);
                                    str2="\n"+tr("驱动版本  : ")+QString(info)+"\n"+tr("固件版本  : ")+list2.at(1)+"\n"+"\n";
                            }
                    }

                            SShowinfo += "\n"\
                                    +tr("硬件型号  : " )+ST+"\n"\
                                    +tr("硬件版本  : ")+SHV+"\n"\
                                    +tr("硬件序号  : ")+SSN  +str2;


                    ui->labelScannerHWInfo->setText(SShowinfo);
                    ui->labelScannerHWInfo->setStyleSheet("color:black");
            }
            else
            {
                    ui->labelScannerHWInfo->setText("\n"+tr("获取硬件信息失败"));
                    ui->labelScannerHWInfo->setStyleSheet("color:red");
            }
    }
    else
    {
            ui->labelScannerHWInfo->setText("\n"+tr("读票机未准备好"));
            ui->labelScannerHWInfo->setStyleSheet("color:red");

    }
}


void TestSystemUI::on_pushButton_toChoieDlg_clicked()
{
    QMessageBox msgBox;
    int msg=msgBox.information(this,"warning", "下次开机将进入<<选择引导界面>>", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
  //  msgBox.setButtonText(QMessageBox::Yes,"确定");
  //  msgBox.setButtonText(QMessageBox::No,"取消");

    if(msg == QMessageBox::Yes)
    {

           system("ln -s /root/TestSystem/changeChoice.sh  /root/TestSystem/localsetup  -f");
           qApp->closeAllWindows();
           exit(4);
    }
    else
        qDebug()<<"no";
}

void TestSystemUI::on_btn_toLHSystem_clicked()
{
    QMessageBox msgBox;
    int msg=msgBox.information(this,"warning", "下次开机将进入<<老化烤机程序>>", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);

    if(msg == QMessageBox::Yes)
    {
        system("ln -s /root/TestSystem/changeMachine.sh  /root/TestSystem/localsetup  -f");
        qApp->closeAllWindows();
         exit(1);
    }
    else
        qDebug()<<"no";
}







