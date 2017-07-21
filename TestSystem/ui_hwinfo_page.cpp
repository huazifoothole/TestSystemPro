#include "testsystemui.h"
#include "ui_testsystemui.h"
#include <QDebug>
#include <QProcess>
#include <QDesktopWidget>
#include<QDateTime>
#include<QString>

void TestSystemUI::on_btnHWInfoBack_clicked()
{
        //ui->label_F7->setStyleSheet("background:url(:/images/active_off.png)");
       ui->label_F7->setStyleSheet("background-color:rgb(0,43,84)");
        ui->label_Tip->setText("");
	ui->stackedWidget->setCurrentIndex(WAIT_TO_CHOICE);
        ui->labelNote->setText("");
        ui->pushButton_saveini->hide();
        ui->pushButton_print2d->hide();
	this->update();
}

void TestSystemUI::HWInfoTimeout()
{
       pSettings = new QSettings("./conf/hwinfo.ini",QSettings::IniFormat);
       QString QRcode;//QR码
	char info[4096];
	ui->label_Tip->setText(tr("正在检测硬件信息..."));

        QString tmpInfo = "";

        memset(info, 0, sizeof(info));
        if(laHW->hwGetCPUInfo(info, sizeof(info)))
        {
                //qDebug()<<info;
                QString strInfo = info;
                QStringList tmpList= strInfo.split('\n');

                for(int i=0; i<tmpList.size(); i++)
                {
                        QStringList  list2 = tmpList.at(i).split('=');

                        if(list2.size() == 2 && list2.at(0) == "CPUName")
                        {
                            tmpInfo += tr("CPU型号 : ")+list2.at(1)+"\n";
                                CPUType=list2.at(1);
                        }

                        if(list2.size() == 2 && list2.at(0) == "Frequency")
                        {
                                tmpInfo += tr("CPU频率 : ")+list2.at(1);
                                 CPU= list2.at(1);
                        }
                }
        }
        else
        {
                tmpInfo += tr("获取CPU信息失败");
        }
        tmpInfo += tr("\n\n");

        memset(info, 0, sizeof(info));
        if(laHW->hwGetMemoryInfo(info, sizeof(info)))
        {
            qDebug()<<"内存信息:"<<info<<"\n";
                QString strInfo = info;
                QStringList tmpList= strInfo.split('\n');

                for(int i=0; i<tmpList.size(); i++)
                {
                        QStringList  list2 = tmpList.at(i).split('=');

                        if(list2.size() == 2 && list2.at(0) == "HWVersion")
                        {
                            tmpInfo += tr("内存类型 : ")+"DDR3";
                            QRcode += tr("内存类型: ")+"DDR3";
                                MEMORYType=list2.at(1);
                        }

                        if(list2.size() == 2 && list2.at(0) == "Capacity")
                        {
                                tmpInfo += tr("               容量: ")+list2.at(1);
                                 MEMORY= list2.at(1);
                        }
                }
        }
        else
        {
                tmpInfo += tr("获取内存信息失败");
        }
        tmpInfo += tr("\n");


        memset(info, 0, sizeof(info));
        if(laHW->hwGetDiskInfo(info, sizeof(info)))
        {
            qDebug()<<"电子盘信息:"<<info<<"\n";
                QString strInfo = info;
                QStringList tmpList= strInfo.split('\n');

                for(int i=0; i<tmpList.size(); i++)
                {
                        QStringList  list2 = tmpList.at(i).split('=');

                        if(list2.size() == 2 && list2.at(0) == "ProductType")
                        {
                                DISKType=list2.at(1);
                                tmpInfo += tr("主存类型 : ")+list2.at(1);
                                qDebug()<<"电子盘类型"<<DISKType<<"\n";
                        }
                        if(list2.size() == 2 && list2.at(0) == "Capacity")
                        {
                                tmpInfo += tr("容量: ")+list2.at(1) + tr(" MB");
                                CAPACITY=list2.at(1);
                                qDebug()<<"电子盘容量"<<CAPACITY<<"\n";
                        }

                }
        }
        else
        {
                tmpInfo += tr("获取电子盘信息失败");
        }
        tmpInfo += tr("\n\n");

        memset(info, 0, sizeof(info));
        if(laHW->hwGetMainboardInfo(info, sizeof(info)))
        {
            qDebug()<<"主板信息:"<<info<<"\n";
                QString strInfo = info;
                QStringList tmpList= strInfo.split('\n');


                for(int i=0; i<tmpList.size(); i++)
                {
                        QStringList  list2 = tmpList.at(i).split('=');


                        if(list2.size() == 2 && list2.at(0) == "BIOSVersion")
                        {
                            tmpInfo += tr("BIOS版本: ")+list2.at(1)+tr("\n");
                                BIOS= list2.at(1);

                        }
                        if(list2.size() == 2 && list2.at(0) == "BIOSReleaseDate")
                        {
                            tmpInfo += tr("BIOS生产版本: ")+list2.at(1)+tr("\n");
                                BIOSReleaseDate= list2.at(1);
                        }

                       /* if(list2.size() == 2 && list2.at(0) == "HWVersion")
                         {
                                 tmpInfo += tr("主板版本: ") + list2.at(1) + "\n";
                                 QRcode += tr("主板版本: ") + list2.at(1) +"  ";
                         }
                         if(list2.size() == 2 && list2.at(0) == "SN")
                        {
                                 tmpInfo += tr("整机序列号: ") + list2.at(1) + "\n";
                                QRcode += tr("整机序列号: ") + list2.at(1)+"  ";

                        }*/
                     /*  if(list2.size() == 2 && list2.at(0) == "ProductDate")
                       {
                                 tmpInfo += tr("整机生产日期: ") + list2.at(1) + "\n";
                                  QRcode += tr("整机生产日期: ") + list2.at(1)+"  ";
                       }
                         if(list2.size() == 2 && list2.at(0) == "BIOSVendor")
                         {
                                 tmpInfo += tr("BIOS厂商: ") + list2.at(1) + "\n";
                                 QRcode += tr("BIOS厂商: ") + list2.at(1)+"  ";
                         }*/

                         if(list2.size() == 2 && list2.at(0) == "UUID")
                         {
                                 //tmpInfo += tr("主板UUID: ") + list2.at(1) + "\n\n";
                                 UUID=list2.at(1);

                                 break;
                         }
                }
        }
        else
        {
                tmpInfo += tr("获取主板信息失败");
        }
     //   tmpInfo += tr("\n");





        ui->labelHwInfo->setText(tmpInfo);
        biosProcess.start("hwclock -r", QIODevice::ReadOnly);
	//获取屏幕分辨率
#if 0
	QDesktopWidget *d=QApplication::desktop();
	int width=d->width(); //屏幕的宽度
	int height=d->height(); //屏幕的高度
	tmpInfo += tr("屏幕分辨率: ") + QString::number(width) + tr("x") + QString::number(height);
	tmpInfo += tr("\n");
#endif


	/**
	 * 三个部件硬件信息
	 */
	if(laPrinter->GetInitStatus())
	{
		memset(info, 0, sizeof(info));
                QString pShowinfo,str2="";
		if(laPrinter->GetPrinterHWInformation(info, sizeof(info)))
		{
                         qDebug()<<info<<"\n";
			QString strInfo = info;
			QStringList tmpList= strInfo.split('\n');

			for(int i=0; i<tmpList.size(); i++)
			{
				QStringList  list2 = tmpList.at(i).split('=');

                                if(list2.size() ==2 && list2.at(0) == "SN")
                                {
                                     PSN=list2.at(1);
                                }

                                if(list2.size() ==2 && list2.at(0) == "ProductDate")
                                {
                                    PD=list2.at(1);
                                }

                                if(list2.size() ==2 && list2.at(0) == "ProductType")
                                {
                                    PT=list2.at(1);
                                }
                                if(list2.size() ==2 && list2.at(0) == "HWVersion")
                                {
                                    PHV=list2.at(1);
                                }

				if(list2.size() == 2 && list2.at(0) == "FirmwareVersion")
				{
					qDebug()<<list2.at(1);
                                        char infoP[100]={0};
                                        laPrinter->PrinterGetSWVersion(infoP,100);
                                        PF=list2.at(1);
                                        PSV=QString(infoP);

                                       /* 硬件序列号（PSN）=
                                        生产日期（PD）=
                                        硬件型号（PT）=
                                        硬件版本（PHV）=
                                        固件版本（PF）=
                                        驱动版本（PSV）=*/
                                        str2="\n"+tr("驱动版本  : ")+QString(infoP)+"\n"+tr("固件版本  : ")+list2.at(1)+"\n";

				}
			}

                        pShowinfo += "\n"\
                                             +tr("硬件型号  : " )+PT+"\n"\
                                             +tr("硬件版本  : ")+PHV+"\n"\
                                             +tr("硬件序号  : ")+PSN+str2;



                        ui->labelPrinterHWInfo->setText(pShowinfo);
                        ui->labelPrinterHWInfo->setStyleSheet("color:black");

		}
		else
		{
                        ui->labelPrinterHWInfo->setText("+"+tr("获取硬件信息失败"));
                        ui->labelPrinterHWInfo->setStyleSheet("color:red");
		}
	}
	else
	{
                  ui->labelPrinterHWInfo->setText("\n"+tr("打印机未准备好"));
                  ui->labelPrinterHWInfo->setStyleSheet("color:red");
	}




        QString showcominfo;
        bool bret=true;
         //int retbios=BIOS.compare(pSettings->value("BIOS/BIOSVersion").toString()+" ");
        QBool retbios=BIOS.contains(pSettings->value("BIOS/BIOSVersion").toString());
        if(!retbios)
        {
            showcominfo +="\n"+tr("Bios 版本不匹配")+"\n";
            bret=false;
        }

        int retMemType=MEMORYType.compare(pSettings->value("MEMORY/MemoryType").toString());
        if(retMemType != 0)
        {
            showcominfo +=tr("内存类型不匹配")+"\n";
            bret=false;
        }


        int retMem=MEMORY.compare(pSettings->value("MEMORY/MemoryCapacity").toString());
        if(retMem != 0)
        {
            showcominfo +=tr("内存大小不匹配")+"\n";
            bret=false;
        }

        int retCpu=CPU.compare(pSettings->value("CPU/CPUFrequency").toString());
        if(retCpu != 0)
        {
            showcominfo +=tr("CPU主频不匹配")+"\n";
            bret=false;
        }

        int retCpuType=CPUType.compare(pSettings->value("CPU/CPUType").toString());
        if(retCpuType != 0)
        {
            showcominfo +=tr("CPU类型不匹配")+"\n";
            bret=false;
        }

         QBool retDiskType=DISKType.contains(pSettings->value("DISK/DiskType").toString());//类型=InnoDisk Corp. -
        if(!retDiskType)
        {
            showcominfo +=tr("电磁盘类型不匹配")+"\n";
            bret=false;
        }

        int retDisk =CAPACITY.compare(pSettings->value("DISK/DiskCapacity").toString());
        if(retDisk != 0)
        {
            showcominfo +=tr("电磁盘容量不匹配")+"\n";
            bret=false;
        }


        int retPF=PF.compare(pSettings->value("FVERSION/PrinterFV").toString());
        qDebug()<<"printer FV打印机"<<pSettings->value("FVERSION/PrinterFV").toString()<<"  PF=="<<PF;
        int retPT=PT.compare(pSettings->value("PRODUCTTYPE/PrinterType").toString());
        if(laPrinter->GetInitStatus())
        {
             QString info="";


                if(retPF!= 0)
             {
                 info=tr("打印机固件不匹配");
                 ui->label_Pcompare->setText(info);
                 ui->label_Pcompare->setStyleSheet("color:red");
             }
            else  if(retPT!= 0)
             {
                  info +=tr("  类型不匹配");
                 ui->label_Pcompare->setText(info);
                 ui->label_Pcompare->setStyleSheet("color:red");
             }
              else
               {
                   ui->label_Pcompare->setText("OK");
                   ui->label_Pcompare->setStyleSheet("color:black");
               }



        }

     else
           {
                ui->label_Pcompare->setText("打印机初始化未成功;");
                ui->label_Pcompare->setStyleSheet("color:red");
           }

        int retSF=SF.compare(pSettings->value("FVERSION/ScannerFV").toString());
        int retST=ST.compare(pSettings->value("PRODUCTTYPE/ScannerType").toString());
        if(laScanner->GetInitStatus())
        {
            QString info="";
            if(retSF!= 0)
            {
                info= tr("读票机固件不匹配");
                ui->label_Scompare->setText(info);
                ui->label_Scompare->setStyleSheet("color:red");
            }
          else  if(retST!= 0)
            {
                info += tr(" 类型不匹配");
                ui->label_Scompare->setText(info);
                ui->label_Scompare->setStyleSheet("color:red");
            }
            else
            {
                ui->label_Scompare->setText("OK");
                ui->label_Scompare->setStyleSheet("color:black");
            }
        }
        else
        {
            ui->label_Scompare->setText("读票机初始化未成功;");
            ui->label_Scompare->setStyleSheet("color:red");
        }


        int retHF=HF.compare(pSettings->value("FVERSION/HscannerFV").toString());
        int retHT=HT.compare(pSettings->value("PRODUCTTYPE/BCRType").toString());
        if(laBCR->GetInitStatus())
        {
            QString info="";
            if(bcrHWFlag==false)
            {
                ui->labelBCRHWInfo->setText(tr("获取硬件信息失败.请重试"));
                ui->labelBCRHWInfo->setStyleSheet("color:red");
                ui->label_Hscompare->setText("获取硬件信息失败");
                ui->label_Hscompare->setStyleSheet("color:red");
            }
           else if(retHF!= 0)
            {
                info = tr("条码枪固件不匹配");
                ui->label_Hscompare->setText(info);
                ui->label_Hscompare->setStyleSheet("color:red");
            }
            else if(retHT != 0)
            {
                info += tr(" 类型不匹配");
                ui->label_Hscompare->setText(info);
                ui->label_Hscompare->setStyleSheet("color:red");
            }
            else
            {
                ui->label_Hscompare->setText("OK");
                ui->label_Hscompare->setStyleSheet("color:black");
            }
        }
        else
        {
            ui->labelBCRHWInfo->setText("\n"+tr("条码枪未准备好"));
            ui->labelBCRHWInfo->setStyleSheet("color:red");
            ui->label_Hscompare->setText("条码枪初始化未成功;");
            ui->label_Hscompare->setStyleSheet("color:red");
        }


        if(!bret)
        {
            ui->label_Mainboardcompare->setStyleSheet("color:red");
            ui->label_Mainboardcompare->setText(showcominfo);
        }
        else
        {
            ui->label_Mainboardcompare->setStyleSheet("color:black");
            ui->label_Mainboardcompare->setText("\n"+tr("硬件信息OK"));
        }





        //硬件信息QR码
     /*  QString qrcodeCmd= "qrencode -o  HWInfo.png " +QString("\"")+QRcode+QString("\"");
        qDebug()<<qrcodeCmd;
        QByteArray str=qrcodeCmd.toLocal8Bit();
        char *cmd=str.data();
        qDebug()<<cmd;
       // QProcess::startDetached(qrcodeCmd,QStringList());
        system(cmd);

        QPixmap pm("HWInfo.png");
        qreal width = pm.width();
        qreal height = pm.height();
        ui->labelQR->resize(width/2,height/2);
        ui->labelQR->setPixmap(pm.scaled(width/2 ,height/2 ,Qt::KeepAspectRatio));
        ui->labelQR->show();
        pm = QPixmap();//clear memor*/
    //    laPrinter->PrinterPrint2DBar(15, 30, 20, 6, 3, buf, strlen(buf), 1);

        ui->label_Tip->setText(tr(" "));
}

void TestSystemUI::BiosRead(int exitCode)
{
	if(exitCode == 0)
	{
		QByteArray byteRead = biosProcess.readAllStandardOutput();
		QString strRead = QString(byteRead);
		QString uiInfo = ui->labelHwInfo->text();
                ui->labelHwInfo->setText(uiInfo+tr("BIOS时间: ")+strRead.left(strRead.indexOf("  ")-1)+"\n\n"+tr("主板UUID: ")+UUID);
	}
}

void TestSystemUI::on_pushButton_saveini_clicked()
{
    pSettings->setValue("CPU/CPUType",CPUType);
    pSettings->setValue("CPU/CPUFrequency",CPU);
    pSettings->setValue("MEMORY/MemoryType",MEMORYType);
    pSettings->setValue("MEMORY/MemoryCapacity",MEMORY);
    pSettings->setValue("DISK/DiskType",DISKType);
    pSettings->setValue("DISK/DiskCapacity",CAPACITY);
    pSettings->setValue("BIOS/BIOSVersion",BIOS);
    pSettings->setValue("FVERSION/PrinterFV",PF);
    pSettings->setValue("FVERSION/ScannerFV",SF);
    pSettings->setValue("FVERSION/HscannerFV",HF);
    pSettings->setValue("PRODUCTTYPE/BCRType",HT);
    pSettings->setValue("PRODUCTTYPE/ScannerType",ST);
    pSettings->setValue("PRODUCTTYPE/PrinterType",PT);

}

void TestSystemUI::on_pushButton_print2d_clicked()
{
    char buf[4096*3]={0};
    //QString QRcode2="M%BIOS=080015;UUID=00020003-0004-0005-0006-000700080009;CPU=2.8GHz#MEMORY=DDR2-1523MB#CAPACITY=DOM-1548;PSN=201202P903#PD=201202#PT=FP460#PHV=1.001#PF=03C#PSV=2.008;SSN=0003#SD=20130401#ST=LT200A4IIIP#SHV=42#SF=LT41212121#SSV=2.007;HSN=E5022656#HD=04/10/2013#HT=EM2028#HHV=1.0#HF=3.06.040LT#HSV=2.004";
    QString QRcode2="M%BIOS="+BIOS+";"+"UUID="+UUID+";"+"CPU="+CPU+"#"+"MEMORY="+MEMORY+"#"+"CAPACITY="+CAPACITY+";"+"PSN="+PSN+"#"+"PD="+PD+"#"+"PT="+PT+"#"+"PHV="+PHV+"#"+"PF="+PF+"#"+"PSV="+PSV+";"\
            +"SSN="+SSN+"#"+"SD="+SD+"#"+"ST="+ST+"#"+"SHV="+SHV+"#"+"SF="+SF+"#"+"SSV="+SSV+";"\
            +"HSN="+HSN+"#"+"HD="+HD+"#"+"HT="+HT+"#"+"HHV="+HHV+"#"+"HF="+HF+"#"+"HSV="+HSV;
    qDebug()<<"QRcode2="<<QRcode2;

     QByteArray str0=QRcode2.toLocal8Bit();
     strcpy(buf, str0.data());
     laPrinter->PrinterSetCutterMode(0);
    int ret =laPrinter->PrinterPrint2DBar(15,30,0, 6, 3, buf, strlen(buf), 1);
    laPrinter->PrinterFeedLine(3);
    QDateTime time = QDateTime::currentDateTime();//获取系统现在的时间
    QByteArray strTime=time.toString("yyyy-MM-dd hh:mm:ss ").toLatin1(); //设置显示格式
    laPrinter->PrinterPrintString(strTime.data());

    qDebug()<<"printPDF ret="<<ret;
    laPrinter->PrinterFeedLine(3);
    laPrinter->PrinterCutPaper();
}
