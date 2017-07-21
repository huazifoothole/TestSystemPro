#include "testsystemui.h"
#include "ui_testsystemui.h"
#include <QDebug>
#include <QProcess>



void TestSystemUI::on_btnScanBack_clicked()
{
        //ui->label_F3->setStyleSheet("background:url(:/images/active_off.png)");
       // ui->label_F4->setStyleSheet("background:url(:/images/active_off.png)");
         ui->label_F3->setStyleSheet("background-color:rgb(0,43,84)");
        ui->label_F4->setStyleSheet("background-color:rgb(0,43,84)");
        laScanner->SetStop(true);
        ui->label_Tip->setStyleSheet("color:white");
	ui->label_Tip->setText("");
        ui->label_falsecount->hide();
        ui->label_false->hide();
      //  if(! laBCR->HscannerDisable())
       //     qDebug()<<"HscannerDisable false";
	ui->stackedWidget->setCurrentIndex(WAIT_TO_CHOICE);
        ui->labelNote->setText("");
	this->update();


}

/*void TestSystemUI::on_btnUpdateBrand_clicked()
{
	LAScanner::MakeBrandToFile(ui->leBrandContent->text(), tr(LATECH_PATH"/BrandPrint.bmp"));
}*/

void TestSystemUI::ScannerScanFinish(int errCode, QString info)
{
        qDebug()<<"scan info="<<info;
	static int iScanTestCorrect, iScanTestError, iScanTestTotal;
	static int iScanStar7Correct = 0;
	static int iScanStar7Error = 0;
	static int iScanStar7Total = 0;
        ui->label_Tip->setStyleSheet("color:white");

        //static QString strInfo;

	QString strShowCount;
        if(info == " ") return;
	if(errCode == 0)
	{
		//标识打印

		qDebug()<<laScanner->GetOnlyScanning();
                 qDebug()<<"strMarkData"<<laPrinter->strMarkData;
                //if(!laScanner->GetOnlyScanning())
                 if(ui->cbPrintDefineBrand->isChecked())
		{

                    if(ui->cbScanShowImage->isChecked())//显示图像
                    {
                        if(ProductType.contains("900"))
                      {

                         QString pathName ;
                      //   if(!ProductType.compare("la900a-02-02")||!ProductType.compare("la900a-02-03"))
                    //         pathName = "A4Grey.bmp";
                    //     else
                            pathName = "blockcode.bmp";

                         QImage sourceImage;
                         sourceImage.load(pathName);
                         QImage targetImage(sourceImage.width(),sourceImage.height(),  QImage::Format_ARGB32_Premultiplied);
                         QPainter painter(&targetImage);
                         //painter.translate(sourceImage.width()/2, sourceImage.height()/2);//中心旋转
                         painter.rotate(180);
                         painter.translate((-1)*sourceImage.width(), (-1)*sourceImage.height());
                         painter.drawImage( 0,0, sourceImage,-250,0,sourceImage.width(),sourceImage.height(),Qt::AutoColor);
                         targetImage.save("testimage.bmp");
                         QPixmap pm("testimage.bmp");
                         qreal width = pm.width();
                         qreal height = pm.height();
                          ui->label_scan_image->setPixmap(pm.scaled(width/3,height/3,Qt::KeepAspectRatio));
                          pm = QPixmap();//clear memory
                      }
                       else
                       {
                            QPixmap pm("blockcode.bmp");
                        //   ui->label_scan_image->setPixmap(pm.scaled(ui->label_scan_image->size()));
                            qreal width = pm.width();
                            qreal height = pm.height();
                            ui->label_scan_image->setPixmap(pm.scaled(width/2,height/2,Qt::KeepAspectRatio));
                            pm = QPixmap();//clear memory
                      }
                    }
                    else
                    {
                        ui->label_scan_image->setText(" ");
                    }

                        ui->label_false->hide();
                        ui->label_falsecount->hide();
                        qDebug()<<ui->cbPrintDefineBrand->isChecked();
                        //qDebug()<<"pdf:"<<pdf417Ticket.contains(info);
                        //qDebug()<<"mark:"<<markData.contains(info);
			//判断兑奖信息
                        // if(pdf417Ticket.contains(info))//随机内容比对
                          if(!pdf417fData.compare(info))//固定内容比对
			{                           
                                        laScanner->PrintBrand(0,660, 1, LATECH_PATH"/BrandPrint.bmp");
                                       // laScanner->PrintBrand(0, 800, 1, "");
				pdf417Ticket.removeOne(info);
                                ui->label_Tip->setText(tr("标记打印成功"));
                                qDebug()<<"compare info"<<info;
			}
                     //   else if(markData.contains(info))//随机内容比对

                         else if(laPrinter->strMarkData.contains(info))//固定内容比对
			{
                                qDebug()<<"contains MarkData"<<laPrinter->strMarkData;

                                        laScanner->PrintBrand(0, 660, 1, LATECH_PATH"/BrandPrint.bmp");

                                    //    laScanner->PrintBrand(0, 800, 1, "");
                            //    qDebug()<<"markData ="<<markData;
                               qDebug()<<"compare info"<<info;
                                ui->label_Tip->setText(tr("标记打印成功"));
			}

			else
			{

                                laScanner->PrintBrand(0, 660, 4, "");
                                ui->label_Tip->setStyleSheet("color:red");
                              //  ui->label_Tip->setText(tr("标记打印失败"));
                                qDebug()<<"pdf417fData="<<pdf417fData;
                                qDebug()<<"markData ="<<laPrinter->strMarkData;
                                qDebug()<<"作废info"<<info;
			}
                          ui->label_scan_content->setText(laScanner->strResult2);
			laScanner->RollBackTicket();
			return;
		}

                ui->label_false->show();
                ui->label_falsecount->show();

		//7星彩投注识别
		if(ui->cbStar7->isChecked())
		{
			if(ui->cbScanShowImage->isChecked())
			{
                            if(ProductType.contains("900"))
                          {
                              QString pathName ;
                            //  if(!ProductType.compare("la900a-02-02")||!ProductType.compare("la900a-02-03"))
                          //        pathName = "A4Grey.bmp";
                          //    else
                                 pathName = "blockcode.bmp";

                             QImage sourceImage;
                             sourceImage.load(pathName);
                             QImage targetImage(sourceImage.width(),sourceImage.height(),  QImage::Format_ARGB32_Premultiplied);
                             QPainter painter(&targetImage);
                             //painter.translate(sourceImage.width()/2, sourceImage.height()/2);//中心旋转
                             painter.rotate(180);
                             painter.translate((-1)*sourceImage.width(), (-1)*sourceImage.height());
                             painter.drawImage( 0,0, sourceImage,-250,0,sourceImage.width(),sourceImage.height(),Qt::AutoColor);
                             targetImage.save("testimage.bmp");
                             QPixmap pm("testimage.bmp");
                             qreal width = pm.width();
                             qreal height = pm.height();
                              ui->label_scan_image->setPixmap(pm.scaled(width/3,height/3,Qt::KeepAspectRatio));
                              pm = QPixmap();//clear memory
                          }
                           else
                           {
                                QPixmap pm("blockcode.bmp");
                            //   ui->label_scan_image->setPixmap(pm.scaled(ui->label_scan_image->size()));
                                qreal width = pm.width();
                                qreal height = pm.height();
                                ui->label_scan_image->setPixmap(pm.scaled(width/2,height/2,Qt::KeepAspectRatio));
                                pm = QPixmap();//clear memory
                          }
			}
                        else
                        {
                            ui->label_scan_image->setText(" ");
                        }
                        if(this->bScanTestStart7)
                        {
                                //strInfo = info;//版本1 以扫描的第一张票内容作为对比票
                                iScanStar7Correct = 0;
                                iScanStar7Error = 0;
                                iScanStar7Total = 0;
                                this->bScanTestStart7 = false;
                        }

			iScanStar7Total++;
			if(star7.compare(info))
			{
				iScanStar7Error++;
                                ui->label_scan_content->setText(tr("7星彩投注单识别不匹配")+"\n"+laScanner->strResult2);
			}
			else
			{
				iScanStar7Correct++;
                                ui->label_scan_content->setText(tr("7星彩投注单识别匹配")+"\n"+laScanner->strResult2);
			}
                        strShowCount = tr("总数: ") + QString::number(iScanStar7Total) + tr("            ")
                                        +tr("正确: ") + QString::number(iScanStar7Correct) +tr("              ")
                                         +tr("            ")
                                        + tr("正确率: ") + QString::number((iScanStar7Correct*100)/(iScanStar7Total)) + tr("%");
                        if(iScanStar7Error == 0)
                       {
                           ui->label_falsecount->setStyleSheet("color:white");
                           ui->label_false->setStyleSheet("color:white");
                           ui->label_falsecount->setText("0");
                       }
                        else //要求错误次数以红色显示
                        {
                             ui->label_falsecount->setStyleSheet("color:red");
                             ui->label_false->setStyleSheet("color:red");
                             ui->label_falsecount->setText(QString::number(iScanStar7Error));
                        }
			ui->label_Tip->setText(strShowCount);
			update();
			return;
		}

		//彩票识别
                //ui->label_scan_content->setText(info);//以X显示数据
                ui->label_scan_content->setText(laScanner->strResult2);//以*显示数据
		if(ui->cbScanShowImage->isChecked())
		{
                    if(ProductType.contains("900"))//900机型的图片较长
                    {
                        QString pathName ;
                      //  if(!ProductType.compare("la900a-02-02")||!ProductType.compare("la900a-02-03"))
                     //       pathName = "A4Grey.bmp";
                    //    else
                           pathName = "blockcode.bmp";


                    QImage sourceImage;
                    sourceImage.load(pathName);
                    QImage targetImage(sourceImage.width(),sourceImage.height(),  QImage::Format_ARGB32_Premultiplied);
                    QPainter painter(&targetImage);
                     //painter.translate(sourceImage.width()/2, sourceImage.height()/2);
                     painter.rotate(180);
                     painter.translate((-1)*sourceImage.width(), (-1)*sourceImage.height());
                     //painter.drawImage(QRect( 0,0, sourceImage.width(), sourceImage.height()), sourceImage);
                     painter.drawImage( 0,0, sourceImage,-250,0,sourceImage.width(),sourceImage.height(),Qt::AutoColor);
                     targetImage.save("testimage.bmp");
                      QPixmap pm("testimage.bmp");
                      qreal width = pm.width();
                      qreal height = pm.height();
                      if(ProductType.contains("900"))
                            ui->label_scan_image->setPixmap(pm.scaled(width/3,height/3,Qt::KeepAspectRatio));
                      else
                            ui->label_scan_image->setPixmap(pm.scaled(width,height,Qt::KeepAspectRatio));
                      pm = QPixmap();//clear memory
                    }
                    else
                    {
                         QPixmap pm("blockcode.bmp");
                     //   ui->label_scan_image->setPixmap(pm.scaled(ui->label_scan_image->size()));
                         qreal width = pm.width();
                         qreal height = pm.height();
                         ui->label_scan_image->setPixmap(pm.scaled(width/2,height/2,Qt::KeepAspectRatio));
                         pm = QPixmap();//clear memory
                   }
                }
                else
                {
                    ui->label_scan_image->setText(" ");
                }


                if(this->bScanTestStart)
		{
                        //strInfo = info;//版本1 以扫描的第一张票内容作为对比票
			iScanTestCorrect = 0;
			iScanTestError = 0;
			iScanTestTotal = 0;
			this->bScanTestStart = false;
                        //ui->label_Tip->setText(tr("第一张票已保存"));
                        //return;
		}
                //else
		{
			iScanTestTotal++;
                        qDebug()<<"bigMarkData"<<bigMarkData;
                        if(!bigMarkData.compare(info))
			{
                                iScanTestCorrect++;
			}
                        else if(!smallMarkData.compare(info))
                        {
                                iScanTestCorrect++;
                        }
                        else if(!pdf417fData.compare(info))
                        {
                                iScanTestCorrect++;
                        }
			else
			{
                                iScanTestError++;
			}
		}
                strShowCount =   tr("总数: ") + QString::number(iScanTestTotal) + tr("            ")
                                    +tr("正确: ") + QString::number(iScanTestCorrect) + tr("              ")
                                    + tr("             ")
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

                update();
		return;
	}
        else  //errcode != 0
	{
                 qDebug()<<"errInfo1 is "<<info;
                if(ui->stackedWidget->currentIndex() != SCAN_TICKET &&
		   ui->stackedWidget->currentIndex() != IDENTIFY_TICKET)
		{
			return;
		}
		if(!laScanner->GetOnlyScanning())
		{//标识打印
			ui->label_Tip->setText(info);
			return;
		}
                 qDebug()<<"errInfo2 is "<<info;
                  ui->label_scan_content->setText(info);//显示数据
                //ui->label_scan_content->setText(laScanner->strResult2);//以*显示数据
	}
}

/*void TestSystemUI::on_pushButton_OriginSanImage_clicked()
{
  //system("eog blockcode.bmp");//用system会造成阻塞 而starDetached会立即返回
     QProcess::startDetached("eog",QStringList("blockcode.bmp"));
}*/
