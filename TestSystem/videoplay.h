#ifndef VIDEOPLAY_H
#define VIDEOPLAY_H

#include <QDialog>
#include <QProcess>
#include <QDebug>
#include <stdio.h>


namespace Ui {
    class VideoPlay;
}

class VideoPlay : public QDialog
{
    Q_OBJECT

public:
    explicit VideoPlay(QString playFile, int width, int height, QWidget *parent = 0);
    ~VideoPlay();

    void ExitPlay();
    void Closed();

private:
    Ui::VideoPlay *ui;

    QProcess playProcess;

};

#endif // VIDEOPLAY_H
