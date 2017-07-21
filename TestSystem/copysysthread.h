#ifndef COPYSYSTHREAD_H
#define COPYSYSTHREAD_H

#include <QDebug>
#include <QThread>
#include <QMessageBox>

class CopysysThread : public QThread
{
    Q_OBJECT

public:
    CopysysThread();
    ~CopysysThread();

    virtual void run();
    void getcmd(QString );
    void copyinfomation(QString);

private:
    QString m_cmd;
     int systemSh(QString s);


signals:
   void  copysysfinished(int exitcode);



};

#endif
