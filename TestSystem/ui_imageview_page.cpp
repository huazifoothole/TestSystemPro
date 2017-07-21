#include "testsystemui.h"
#include "ui_testsystemui.h"
#include <QMessageBox>
#include <QSettings>
#include <QTextCodec>

bool TestSystemUI::InitImageViewer()
{
	QSettings pSettings("/etc/LATECH/image.ini", QSettings::IniFormat);
	pSettings.setIniCodec(QTextCodec::codecForName("UTF-8"));
	QString sockpath = pSettings.value("SERVER/PATH", SOCKET_PATH).toString();
	if(!imgServer.StartServer(sockpath))
	{
		ui->label_Tip->setText(tr("创建图片服务出错"));
		return false;
	}

	int i;
	int scCount = pSettings.value("SCREEN/COUNT", 0).toInt();
	QStringList scList;
	for(i=1; i<=scCount; ++i)
	{
		QString sc = pSettings.value("SCREEN/SCREEN"+QString::number(i), "").toString();
		if(sc.length() > 0)
			scList.append(sc);
	}

	for(i = 0; i<scList.size(); ++i)
	{
		QString display = pSettings.value(scList.at(i)+"/DISPLAY", ":0.0").toString();
		int x = pSettings.value(scList.at(i)+"/X", 0).toInt();
		int y = pSettings.value(scList.at(i)+"/Y", 0).toInt();
		int w = pSettings.value(scList.at(i)+"/WIDTH", 400).toInt();
		int h = pSettings.value(scList.at(i)+"/HEIGHT", 300).toInt();
		int m = pSettings.value(scList.at(i)+"/MAIN", 1).toInt();
		QProcess *process = new QProcess();
		proList.append(process);
		QStringList evnList = QProcess::systemEnvironment();
		int index = evnList.indexOf(QRegExp("DISPLAY=(*.)"));
		if(index != -1)
		{
			evnList.replace(index, "DISPLAY="+display);
		}
		else
		{
			evnList.append("DISPLAY="+display);
		}
		process->setEnvironment(evnList);
		process->start("/etc/LATECH/APP/ImageViewer "+QString::number(x)+" "+QString::number(y)
					  +" "+QString::number(w)+" "+QString::number(h)+" "+QString::number(m));
	}
	if(i<1)
	{
		ui->label_Tip->setText(tr("图片配置文件出错"));
		return false;//配置文件出错
	}
	return true;
}

void TestSystemUI::QuitImageViewer()
{
	for(int i=0; i<proList.size(); ++i)
	{
		if(proList.at(i)->state()) proList.at(i)->kill();
		delete proList.at(i);
	}
	proList.clear();
	imgServer.StopServer();
	ui->stackedWidget->setCurrentIndex(WAIT_TO_CHOICE);
        ui->labelNote->setText("");
}
