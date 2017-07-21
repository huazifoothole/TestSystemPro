#include "widget.h"
#include <QApplication>
#include <QDesktopWidget>
#include <QTextCodec>
#include <QDebug>

int main(int argc, char *argv[])
{
	//argv="x y w h 1/0
	QApplication a(argc, argv);
	QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));

	if(argc != 6)
	{
		qDebug()<<argv[0]<<" x y width height 1/0";
		return -1;
	}
	QStringList argsList = a.arguments();
	Widget w(argsList.at(1).toInt(),
			argsList.at(2).toInt(),
			argsList.at(3).toInt(),
			argsList.at(4).toInt(),
			argsList.at(5).toInt());
	w.show();
	
	return a.exec();
}
