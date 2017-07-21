#include "mysocketcontroler.h"

MySocketControler::MySocketControler(QObject *parent) :
	QObject(parent)
{
}

class MyQLocalSocket : QObject
{
	Q_OBJECT
public:
	explicit MyQLocalSocket(QObject *parent = 0) : QObject(parent)
	{
	}
};
