#include <QtGui/QApplication>
#include "testsystemui.h"
#include <QTextCodec>
#include <QDebug>
#include <QDir>
#include <engine/def.h>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
	QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
	QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));

	TestSystemUI w;

	w.setWindowFlags(Qt::FramelessWindowHint);
	w.setGeometry(0, 0, 1024, 768);
	w.show();
#ifndef V_TEST
	w.InitUI();
//        w.InitBCRAgain();
#endif

	return a.exec();
}
