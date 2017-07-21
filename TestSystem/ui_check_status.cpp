#include "testsystemui.h"
#include "ui_testsystemui.h"
#include <QDebug>
#include <QProcess>

void TestSystemUI::on_btnCheckStatusBack_clicked()
{
    //ui->label_F8->setStyleSheet("background:url(:/images/active_off.png)");
    ui->label_F8->setStyleSheet("background-color:rgb(0,43,84)");
    ui->stackedWidget->setCurrentIndex(WAIT_TO_CHOICE);
    ui->labelNote->setText("");
    this->update();
}

void TestSystemUI::checkMachineStatus()
{
    //打印机status
      if(printerInitFlag)
      {
            char errinfo[1024]={0};
         // int ret=laPrinter->PrinterPrintIsComplete(2);
         bool ret = laPrinter->printerIsReady();
          qDebug("printerIsready return  =%d\n",ret);
         if(ret == false)
         {
             QString pStatusStr=laPrinter->GetStatusString();
            laPrinter->PrinterGetLastErrorStr(errinfo,1024);
            qDebug("errinfo = %s\n",errinfo);
            ui->label_printerStatus->setStyleSheet("color:red");
//            ui->label_printerStatus->setText(QString(tr("未准备好 错误信息")+QLatin1String(errinfo)));
             ui->label_printerStatus->setText(pStatusStr);
            if(laPrinter->Init(SO_PATH, SO_PATH))
             {
            ui->label_printerStatus->setStyleSheet("color:black");
            ui->label_printerStatus->setText(tr("正常"));
            printerInitFlag = true;
            }
        }
        else
         {
           ui->label_printerStatus->setStyleSheet("color:black");
           ui->label_printerStatus->setText("正常");
        }

     }
  else
      {
            qDebug("printer init false");
             ui->label_printerStatus->setStyleSheet("color:red");
             ui->label_printerStatus->setText(tr("异常;"));
            if(laPrinter->Init(SO_PATH, SO_PATH))
            {
                ui->label_printerStatus->setStyleSheet("color:black");
                ui->label_printerStatus->setText(tr("正常"));
                printerInitFlag = true;
            }
      }


    //读票机
     if(scanInitFlag)
    {
        bool bStaus = laScanner->GetScannerStatus();
        if(!bStaus)
         {
           // ScannerScanFinish(-1, tr("未准备好"));
            QString str=tr("未准备好");
            ui->label_scanStatus->setStyleSheet("color:red");
            ui->label_scanStatus->setText(str);
            if(laScanner->Init(SO_PATH, SO_PATH))
            {
                ui->label_scanStatus->setStyleSheet("color:black");
                ui->label_scanStatus->setText(tr("正常"));
            }

         }
        else
        {

               ui->label_scanStatus->setStyleSheet("color:black");
               ui->label_scanStatus->setText("正常");
         }
    }
     else
     {
         ui->label_scanStatus->setStyleSheet("color:red");
         ui->label_scanStatus->setText("异常;");
         if(laScanner->Init(SO_PATH, SO_PATH))
         {
             ui->label_scanStatus->setStyleSheet("color:black");
             ui->label_scanStatus->setText(tr("正常"));
             scanInitFlag = true;
         }
         qDebug("sanner init false");
     }

    //条码枪
     if(bcrInitFlag)
     {
            bool ok = laBCR->HscannerEnable();
             if(ok)
            {
                ui->label_bcrStatus->setText(tr("正常"));
            }
            else
          {

            char errstr[1024]={0};
            laBCR->HscannerGetLastErrorStr(errstr,1024);
            qDebug()<<"BCR GetLastErr is"<<errstr;
            ui->label_bcrStatus->setStyleSheet("color:red");
            ui->label_bcrStatus->setText(tr("未准备好 错误信息")+QString(errstr));
             if(laBCR->Init(SO_PATH, SO_PATH))
             {
                ui->label_bcrStatus->setStyleSheet("color:black");
                ui->label_bcrStatus->setText(tr("正常"));
             }
          }
     }
     else
     {
          ui->label_bcrStatus->setStyleSheet("color:red");
          ui->label_bcrStatus->setText(tr("异常;"));
          if(laBCR->Init(SO_PATH, SO_PATH))
          {
              ui->label_bcrStatus->setStyleSheet("color:black");
              ui->label_bcrStatus->setText(tr("正常"));
              bcrInitFlag =true;
          }
          qDebug("BCR init false");
     }

}


