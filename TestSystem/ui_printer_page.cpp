#include "testsystemui.h"
#include "ui_testsystemui.h"
#include <QDebug>
#include <QProcess>

void TestSystemUI::PrinterTimerEvent()
{
	//if(printerTimer.isActive()) printerTimer.stop();
	if(ui->stackedWidget->currentIndex() == PRINT_TICKET)
	{

                if(laPrinter)
		{
                        ui->label_Tip->setText("正在打印测试样票 ...");
                        //新北洋无打印自检信息接口
                        laPrinter->PrintTestBlock();
                        bool flag = true;
                        if(!laPrinter->PrintSampleTicket())
			{
                                QString errTip = laPrinter->GetStatusString();
				ui->label_Tip->setText(errTip);
                                ui->label_printerStatus->setStyleSheet("color:red");
                                ui->label_printerStatus->setText(errTip);
                                flag = false;
                        }
                         if(!laPrinter->PrintNonFixLenTicket(1))
                        {
                                QString errTip = laPrinter->GetStatusString();
                                ui->label_Tip->setText(errTip);
                                ui->label_printerStatus->setStyleSheet("color:red");
                                ui->label_printerStatus->setText(errTip);
                                flag = false;
                        }
                         if (flag)
			{
                                //markData.append(laPrinter->GetMarkData());
                                markData = laPrinter->GetMarkData();
                                qDebug()<<laPrinter->GetMarkData();
				pdf417Ticket.append(laPrinter->GetPDF417Data());
                                qDebug()<<laPrinter->GetPDF417Data();
				ui->label_Tip->setText("样票打印成功!");
                        }
		}
		else
		{
                        ui->label_Tip->setText("打印失败!");
                }

	}
}

void TestSystemUI::on_btnPrinterBack_clicked()
{
        //ui->label_F2->setStyleSheet("background:url(:/images/active_off.png)");
        ui->label_F2->setStyleSheet("background-color:rgb(0,43,84)");
	ui->label_Tip->setText("");
	ui->stackedWidget->setCurrentIndex(WAIT_TO_CHOICE);
        ui->labelNote->setText("");
	this->update();
       // printerTimer.start(3000);
}

void TestSystemUI::on_btnPrinterTest_clicked()
{
#ifndef V_TEST
	laPrinter->PrintDensityBlock();
#endif
}
