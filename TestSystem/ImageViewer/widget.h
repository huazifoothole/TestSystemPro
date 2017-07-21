#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QPixmap>
#include <QtNetwork/QLocalSocket>

#define SOCKET_PATH	"imageviewer"
#define IMAGE_DIR	"/etc/LATECH/Images"
#define INDEX_MAX	6
#define INDEX_MIN	0

namespace Ui {
	class Widget;
}

class Widget : public QWidget
{
	Q_OBJECT
	
public:
	explicit Widget(int x, int y, int w, int h, int ctrl, QWidget *parent = 0);
	~Widget();
	void NotifyToUser(QString title, QString info);

public slots:
	void Quit();
	void NextImage();
	void PrivImage();
	void ShowImage(int index);
	void ShowImage(QString imagePath);

protected:
	void keyPressEvent(QKeyEvent *e);
	void closeEvent(QCloseEvent *e);
	void SetImageToLabel();

protected slots:
	void displayError(QLocalSocket::LocalSocketError socketError);
	void ReadFromServer();
	bool WriteToServer(char ch);
	
private:
	Ui::Widget *ui;
	QPixmap imgPixmap;
	int imgIndex;
	int ctrl;
	bool bLocal;
	QString serverPath;
	QLocalSocket *socket;
};

#endif // WIDGET_H
