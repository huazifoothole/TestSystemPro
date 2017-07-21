#include "copysysthread.h"


CopysysThread::CopysysThread() :
        QThread(),
        m_cmd(" ")
{

}

CopysysThread::~CopysysThread()
{


}

void CopysysThread::getcmd(QString cmd)
{
    m_cmd = cmd;
}

int CopysysThread::systemSh(QString s)
{

    pid_t status ;
    status =  system(s.toLatin1().data());
    if(status == -1)
    {
        qDebug("system update.sh error");
        return 1;
    }
    else
    {
         qDebug("exit status value = [0x%x]\n", status);
        if (WIFEXITED(status))
         {
            if (0 == WEXITSTATUS(status))
            {
                qDebug("run shell script successfully.\n");
                return 0;
            }
            else
             {
                qDebug("run shell script fail, script exit code: %d\n", WEXITSTATUS(status));
                return 1;
             }
         }
    else
        {
            qDebug("exit status = [%d]\n", WEXITSTATUS(status));
            return 1;
        }

}
    return 1;

}




void CopysysThread::run()
{

    qDebug()<<"in run";
    if(m_cmd != " ")
    {
        bool flag=true;
        while(flag)
        {
            int ret=systemSh(m_cmd);
            if(ret == 0)
            {
               qDebug()<<tr("复制成功");
               emit copysysfinished(0);
               flag= false;


            }
            else{
                qDebug()<<tr("复制失败");
                emit copysysfinished(1);
                flag= false;
                 }
       }
    }

    //若线程中没有自己定义的槽函数，可以不调用exec(),即不启动事件循环
}



