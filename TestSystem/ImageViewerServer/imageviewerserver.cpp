#include "imageviewerserver.h"
#include <QDebug>

ImageViewerServer::ImageViewerServer(QObject *parent) :
	QObject(parent)
{
}

ImageViewerServer::~ImageViewerServer()
{
	if(isStarted)
	{
		StopServer();
	}
	delete imgServer;
}

bool ImageViewerServer::IsStarted()
{
	return isStarted;
}

bool ImageViewerServer::StartServer(QString socket_path)
{
	isStarted = false;
	imgServer->removeServer(socket_path);
	imgServer = new QLocalServer(this);
	if(imgServer == NULL)
	{
		qDebug()<<tr("ImageViewer Server Unable alloc server memory");
		return false;
	}

	if (!imgServer->listen(socket_path)) {
		qDebug()<<tr("ImageViewer Server Unable to start: %1.").arg(imgServer->errorString());
		return false;
	}

	connect(imgServer, SIGNAL(newConnection()), this, SLOT(newConnection()));
	isStarted = true;
	return true;
}

void ImageViewerServer::newConnection()
{
	QLocalSocket *clientConnection;
	MySocketControler *mysock;
	clientConnection = imgServer->nextPendingConnection();
	if(clientConnection)
	{
		mysock = new MySocketControler();
		mysock->SetLocalSocket(clientConnection);
		connect(mysock, SIGNAL(myDisconnected(MySocketControler*)),
				this, SLOT(CheckDisconnected(MySocketControler*)));
		connect(mysock, SIGNAL(myReadyRead(MySocketControler*)),
				this, SLOT(ReceiveData(MySocketControler*)));
		sockList.append(mysock);
	}
}

void ImageViewerServer::CheckDisconnected(MySocketControler* mysock)
{
	sockList.removeAll(mysock);
	delete mysock;
	mysock = NULL;
}

void ImageViewerServer::ReceiveData(MySocketControler* mysock)
{
	char ch[2] = {0};
	QLocalSocket *w, *r = mysock->LocalSocket();

	if(1 == r->read(ch, 1))
	{
		//qDebug()<<ch;
		switch(ch[0])
		{
			case 'Q':
			{
				StopServer();
				break;
			}
			case 'N':
			case 'P':
			{
				for(int i=0; i<sockList.size(); ++i)
				{
					w = sockList.at(i)->LocalSocket();
					w->write(ch, 1);
					w->flush();
				}
				break;
			}
			default:
				break;
		}
	}
}

void ImageViewerServer::StopServer()
{
	QLocalSocket *w;
	MySocketControler *mc;
	if(isStarted == false) return;
	imgServer->close();
	isStarted = false;
	for(int i=0; i<sockList.size(); ++i)
	{
		mc = sockList.at(i);
		mc->disconnect();
		w = sockList.at(i)->LocalSocket();
		w->write("Q", 1);
		w->flush();
		w->disconnectFromServer();
		delete mc;
	}
	sockList.clear();
	emit ServerQuit();
}
