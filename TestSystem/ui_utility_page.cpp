#include "testsystemui.h"
#include "ui_testsystemui.h"
#include <QDebug>
#include <QProcess>

void TestSystemUI::BCRScanFinished(int errCode, QString info)
{

        static int iScanTestCorrect, iScanTestError, iScanTestTotal;
       // static QString strInfo;
	qDebug()<<"BCR read data: "<<info;
        if(errCode == 0)
	{
		//条码枪扫描识别
		ui->labelBCRInfo->setText(info);
		if(isFirstTicket)
		{
                       //  strInfo = info;
			iScanTestCorrect = 0;
			iScanTestError = 0;
			iScanTestTotal = 0;
			isFirstTicket = false;
                        //ui->label_Tip->setText(tr("第一张票已保存")); //比对固定内容，不需保存第一张票
                        // return;
		}

		{
			iScanTestTotal++;
                        if(pdf417fData.compare(info))
			{
				iScanTestError++;
			}
			else
			{
				iScanTestCorrect++;
			}
		}
                QString strShowCount =  tr("总数: ") + QString::number(iScanTestTotal) + tr("             ")
                                 +tr("正确: ") + QString::number(iScanTestCorrect) + tr("               ")
                                 + tr("              ")
				+ tr("正确率: ") + QString::number((iScanTestCorrect*100)/(iScanTestTotal)) + tr("%");
		ui->label_Tip->setText(strShowCount);
                if(iScanTestError == 0)//要求错误次数以红色显示
               {
                   ui->label_falsecount->setStyleSheet("color:white");
                   ui->label_false->setStyleSheet("color:white");
                   ui->label_falsecount->setText(("0"));
               }
                else
                {
                     ui->label_falsecount->setStyleSheet("color:red");
                     ui->label_false->setStyleSheet("color:red");
                     ui->label_falsecount->setText(QString::number(iScanTestError));
                }


                    ui->label_false->show();
                    ui->label_falsecount->show();

		update();
		return;
	}
	else if(errCode == -1)
	{
                ui->labelBCRInfo->setText(info);
              //  ui->label_bcrStatus->setStyleSheet("color:black");
               // ui->label_bcrStatus->setText(tr("条码枪:正常; "));
		return;
	}
         ui->labelBCRInfo->setText(info);
         //ui->label_bcrStatus->setStyleSheet("color:red");
       //  ui->label_bcrStatus->setText(tr("条码枪:异常; "));
}

void TestSystemUI::on_pushbuttonCheckNetCard_clicked()
{
        bool tip1,tip2;
	static bool isChecking = false;
	if(isChecking) return;
	isChecking = true;
        ui->label_falsecount->hide();
        ui->label_false->hide();
	ui->label_Tip->setText(tr("正在检测网卡"));

        qDebug()<<tr("正在检测网卡");

        QStringList devList = LAHW::GetNetDevName();

        if(devList.size() == 0)
        {
                ui->label_Tip->setText(tr("未找到网卡!"));
                return;
        }

        tip1 = netCheck(1);
        tip2 = netCheck(2);
        sleep(2);
        if(tip1 == 1&&tip2 == 1 )
        {
                ui->label_Tip->setText(tr("网卡测试成功"));
        }
        else if(tip1 != 1&&tip2 == 1)
        {
                ui->label_Tip->setText(tr("网卡1测试失败,网卡2成功,请重新测试"));
        }
        else if(tip1 == 1&&tip2 != 1)
        {
                ui->label_Tip->setText(tr("网卡2测试失败,网卡1成功,请重新测试"));
        }
        else
            ui->label_Tip->setText(tr("网卡1测试失败,网卡2测试失败,请重新测试"));

	isChecking = false;

}



void TestSystemUI::on_pushButtonPlayMusic_clicked()
{
	QString cmd = "mplayer -quiet -slave "LATECH_PATH"/music.mp3";
	if(musicProcess.state()) return;

        //ui->progressBarMusic->setValue(0);
	musicProcess.start(cmd);
	musicProcess.write("get_percent_pos\n");
	musicTimer.start(500);
}

void TestSystemUI::MusicToRead()
{
	char rbuf[128];
        unsigned int rlen, value;

	memset(rbuf, 0, sizeof(rbuf));
	//ANS_PERCENT_POSITION=60
	rlen = musicProcess.readLine(rbuf, sizeof(rbuf));
	qDebug()<<rbuf;
	if(rlen > 0 && rlen < sizeof(rbuf))
	{
		value = -1;
		sscanf(rbuf, "ANS_PERCENT_POSITION=%d", &value);
                //if(value > 0)
                        //ui->progressBarMusic->setValue(value);
	}
}

void TestSystemUI::MusicTimeout()
{
	if(musicProcess.state() == 0) return;
	musicProcess.write("get_percent_pos\n");
}

void TestSystemUI::MusicFinished()
{
	musicTimer.stop();
        //ui->progressBarMusic->setValue(ui->progressBarMusic->maximum());
	ui->label_Tip->setText(tr("音频测试完成"));
}

void TestSystemUI::on_pushButtonFixTouchScreen_clicked()
{
	system("./touchcfg.sh");
}
int TestSystemUI::netCheck(int net)
{

            char buff[1024]={0};
            FILE *fp;
            int char_read;
            int ret = 1;
            if(net==1)
            {
                fp = popen("./ipget.sh1","r");
                if(fp != NULL)
                {
                char_read = fread(buff,sizeof(char),sizeof(buff)-1,fp);
                if(char_read>0)
                {
                ret=1;
                }
                else
                {
                  ret = -1;
                }
                }
                else
                ret =0;

                memset(buff,0,sizeof(buff));
                pclose(fp);
            }

            if(net == 2)
            {
                fp = popen("./ipget.sh2","r");
                if(fp != NULL)
                {
                char_read = fread(buff,sizeof(char),sizeof(buff)-1,fp);
                if(char_read>0)
                {
                ret=1;
                }
                else
                {
                  ret = -1;
                }
                }
                else
                ret =0;

                memset(buff,0,sizeof(buff));
                pclose(fp);
            }


            return ret;
}

void TestSystemUI::on_pushButtonThirdVideo_clicked()
{
//    system("export DISPLAY=:0.1 && ./NewPlayer &"); //第三屏

    //第二屏
    QString playLocalFile;
    playLocalFile = QFileDialog::getOpenFileName(this, "VGA第二屏本地视频文件选择", secondVdoPath,
                                             tr("videos (*.mpg *.avi *.mp4 *.vob *.mpeg *.wma *.wmv *.rm *.rmvb *.fli *.flv *.mkv *.wma *.m4a *.m4p *.aac *.mp3)"));

    if(playLocalFile != NULL){
//        videoDlg->ExitPlay();
//        videoDlg->close();

        videoFlag = true;
        videoDlg = new VideoPlay(playLocalFile, 1024, 768, this);
        videoDlg->move(0, 768);
        videoDlg->show();
           }
}

void TestSystemUI::on_btnUtilityPageBack_clicked()
{
      //  ui->label_F6->setStyleSheet("background:url(:/images/active_off.png)");
        ui->label_F6->setStyleSheet("background-color:rgb(0,43,84)");
        if(musicProcess.state()) musicProcess.kill();
       // laBCR->SetStop(true);
        ui->label_Tip->setText("");
        ui->label_falsecount->hide();
        ui->label_false->hide();
        ui->stackedWidget->setCurrentIndex(WAIT_TO_CHOICE);
        ui->labelNote->setText("");

        ui->pushButtonPlayMusic->setStyleSheet(" ");
        ui->pushbuttonCheckNetCard->setStyleSheet(" ");
        ui->pushButtonFixTouchScreen->setStyleSheet(" ");
        ui->label_checknet->setStyleSheet("  ");
        ui->label_TouchScreen->setStyleSheet("  ");
        ui->label_PlayMusic->setStyleSheet(" ");

        if(videoFlag == true){
            qDebug("ok");
            videoDlg->ExitPlay();
            videoDlg->close();
        }

        this->update();
       //  bcrTimer.start();//开启条码接口检测
}


void TestSystemUI::on_pushButtonWeb_clicked()
{
    ui->webView->load(QUrl("http://www.baidu.com"));
    ui->webView->show();
}
