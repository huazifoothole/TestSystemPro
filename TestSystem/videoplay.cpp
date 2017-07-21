#include "videoplay.h"
#include "ui_videoplay.h"


VideoPlay::VideoPlay(QString playFile, int width, int height, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::VideoPlay)
{
    ui->setupUi(this);
    //setWindowTitle("乐得瑞体彩终端多屏演示");
    //setWindowFlags(Qt::FramelessWindowHint);
    setWindowFlags(Qt::Tool | Qt::X11BypassWindowManagerHint | Qt::WindowStaysOnTopHint);

    setGeometry(0, 0, width, height);
    ui->videoWidget->setGeometry(0, 0, width, height);

    WId wid;
    wid = ui->videoWidget->winId();
    QString cmd = "mplayer -loop 0 -slave -quiet -vf scale=" + QString::number(width) + ":" + QString::number(height) + " " + "\"" + playFile + "\"" +" -wid " + QString::number(wid);
    qDebug() << cmd;
    playProcess.start(cmd);
    playProcess.write("osd\n");
}

VideoPlay::~VideoPlay()
{
    delete ui;
}

void VideoPlay::ExitPlay()
{
    playProcess.write("quit\n");
    playProcess.close();
    playProcess.pid();
}

void VideoPlay::Closed()
{
    playProcess.write("quit\n");
}
