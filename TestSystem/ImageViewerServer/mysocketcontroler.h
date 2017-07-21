#ifndef MYSOCKETCONTROLER_H
#define MYSOCKETCONTROLER_H

#include <QObject>
#include <QtNetwork/QLocalSocket>

class MySocketControler : public QObject
{
	Q_OBJECT
public:
	explicit MySocketControler(QObject *parent = 0);

	~MySocketControler()
	{
		localSocket->abort();
		localSocket->disconnect();
		//delete localSocket;
               //localSocket == NULL;
	}

	bool SetLocalSocket(QLocalSocket *sock)
	{
		if(sock == NULL) return false;
		localSocket = sock;
		connect(localSocket, SIGNAL(readyRead()), this, SLOT(emitReadyRead()));
		connect(localSocket, SIGNAL(disconnected()), this, SLOT(emitDisconnected()));
		return true;
	}

	QLocalSocket* LocalSocket()
	{
		return localSocket;
	}

protected slots:
	void emitReadyRead()
	{
		emit myReadyRead(this);
	}

	void emitDisconnected()
	{
		emit myDisconnected(this);
	}

private:
	QLocalSocket *localSocket;

signals:
	void myReadyRead(MySocketControler *mysock);
	void myDisconnected(MySocketControler *mysock);
};

#endif // MYSOCKETCONTROLER_H
