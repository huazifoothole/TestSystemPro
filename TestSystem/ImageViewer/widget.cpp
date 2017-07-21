#include "widget.h"
#include "ui_widget.h"
#include <QKeyEvent>
#include <QDesktopWidget>
#include <QTimer>
#include <QProcess>
#include <QSettings>
#include <QTextCodec>

Widget::Widget(int x, int y, int w, int h, int ctrl, QWidget *parent) :
	QWidget(parent),
	ui(new Ui::Widget)
{
	ui->setupUi(this);
	setWindowFlags(windowFlags()|Qt::FramelessWindowHint);
	this->ctrl = ctrl;
	setGeometry(x, y, w, h);
	ui->label->setGeometry(rect());
	QSettings pSettings("/etc/LATECH/image.ini", QSettings::IniFormat);
	pSettings.setIniCodec(QTextCodec::codecForName("UTF-8"));
	serverPath = pSettings.value("SERVER/PATH", SOCKET_PATH).toString();

	socket = new QLocalSocket(this);
	connect(socket, SIGNAL(readyRead()), this, SLOT(ReadFromServer()));
	connect(socket, SIGNAL(error(QLocalSocket::LocalSocketError)),
			this, SLOT(displayError(QLocalSocket::LocalSocketError)));

	bLocal = false;
	socket->connectToServer(serverPath);
	//if(ctrl == 0) this->setDisabled(true);
	ShowImage(0);
}

Widget::~Widget()
{
	delete ui;
}

void Widget::keyPressEvent(QKeyEvent *e)
{
	//if(!bLocal && ctrl == 0) return ;
	switch(e->key())
	{
		case Qt::Key_Escape:
		{
			if(bLocal)
				Quit();
			else
				WriteToServer('Q');
			break;
		}
		case Qt::Key_Left:
		{
			if(bLocal)
				QTimer::singleShot(0, this, SLOT(PrivImage()));
			else
				WriteToServer('P');
			break;
		}
		case Qt::Key_Right:
		{
			if(bLocal)
				QTimer::singleShot(0, this, SLOT(NextImage()));
			else
				WriteToServer('N');
			break;
		}
		default:
			break;
	}
}

void Widget::closeEvent(QCloseEvent *e)
{
	Quit();
}

void Widget::Quit()
{
	if(socket->state()== QLocalSocket::ConnectedState)
		socket->disconnectFromServer();
	qApp->quit();
}

void Widget::NextImage()
{
	imgIndex++;
	if(imgIndex > INDEX_MAX)
	{
		imgIndex = INDEX_MAX;
                // NotifyToUser("ImageView Warning", "It is the last image.");
		return;
	}
	ShowImage(imgIndex);
}

void Widget::PrivImage()
{
	imgIndex--;
	if(imgIndex < INDEX_MIN)
	{
		imgIndex = INDEX_MIN;
		//NotifyToUser("ImageView Warning", "It is the first image.");
		return;
	}
	ShowImage(imgIndex);
}

void Widget::ShowImage(int index)
{
	imgIndex = index;
	QString imagePath = IMAGE_DIR"/" + QString::number(imgIndex) + ".jpg";
	ShowImage(imagePath);
}

void Widget::ShowImage(QString imagePath)
{
	imgPixmap = QPixmap();//release the memory of old pixmap
	imgPixmap = QPixmap(imagePath);
	if(imgPixmap.isNull())
	{
		NotifyToUser("ImageViewer error", imagePath+" not exist");
		return;
	}
	SetImageToLabel();
}

void Widget::NotifyToUser(QString title, QString info)
{
	QString cmdStr = "notify-send \"" + title + "\" \"" + info + "\"";
	if(ctrl == 1)
		QProcess::execute(cmdStr);
}

void Widget::SetImageToLabel()
{
	if(!imgPixmap.isNull())
	{
		ui->label->setPixmap(imgPixmap.scaled(ui->label->size(),
										  Qt::IgnoreAspectRatio,
										  Qt::SmoothTransformation));
	}
}

void Widget::ReadFromServer()
{
	char ch[2] = {0};
	int cnt = socket->read(ch,1);
	qDebug()<<"read from server: "+QString::number(cnt) + "["+tr(ch)+"]";
	if(cnt == 1)
	{
		switch(ch[0])
		{
			case 'Q':
			{
				Quit();
				break;
			}
			case 'P':
			{
				PrivImage();
				break;
			}
			case 'N':
			{
				NextImage();
				break;
			}
			default:
				break;
		}
	}
}

bool Widget::WriteToServer(char ch)
{
	if(1 == socket->write(&ch, 1))
		return true;
	else
		return false;
}

#include <QMessageBox>
void Widget::displayError(QLocalSocket::LocalSocketError socketError)
{
	switch (socketError)
	{
		case QLocalSocket::ServerNotFoundError:
		{
			bLocal = true;
			QMessageBox::information(this, tr("ImageViewer Client"),
									 tr("The host was not found. Please check the "
										"host name and port settings."));
			break;
		}
		case QLocalSocket::ConnectionRefusedError:
		{
			bLocal = true;
			QMessageBox::information(this, tr("ImageViewer Client"),
									 tr("The connection was refused by the peer. "
										"Make sure the fortune server is running, "
										"and check that the host name and port "
										"settings are correct."));
			break;
		}
		case QLocalSocket::PeerClosedError:
		{
			bLocal = true;
			break;
		}
		default:
		{
			bLocal = true;
			QMessageBox::information(this, tr("ImageViewer Client"),
									 tr("The following error occurred: %1.")
									 .arg(socket->errorString()));
		}
	}
}
