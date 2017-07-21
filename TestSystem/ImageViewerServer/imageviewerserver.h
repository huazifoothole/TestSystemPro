#ifndef IMAGEVIEWERSERVER_H
#define IMAGEVIEWERSERVER_H

#include <QObject>
#include <QtNetwork/QLocalServer>
#include <QtNetwork/QLocalSocket>
#include "mysocketcontroler.h"

#define SOCKET_PATH	"imageviewer"

class ImageViewerServer : public QObject
{
	Q_OBJECT
public:
	explicit ImageViewerServer(QObject *parent = 0);
	~ImageViewerServer();
	bool StartServer(QString socket_path);
	void StopServer();
	bool IsStarted();

public slots:

protected slots:
	void newConnection();
	void ReceiveData(MySocketControler* mysock);
	void CheckDisconnected(MySocketControler* mysock);

private:
	QLocalServer *imgServer;
	QList<MySocketControler*> sockList;
	bool isStarted;

signals:
	void ServerQuit();
};

#endif // IMAGEVIEWERSERVER_H
