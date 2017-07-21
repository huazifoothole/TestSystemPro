#ifndef KEYBOARDWIDGET_H
#define KEYBOARDWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QQueue>
#include <QMap>
#include <QTimer>

struct KeyInfo
{
    QLabel *pLabel;
    QString strName;
    QString strDisplay;
	int *count;
};

namespace Ui {
	class KeyboardWidget;
}

class KeyboardWidget : public QWidget
{
	Q_OBJECT
	
public:
	explicit KeyboardWidget(QWidget *parent = 0);
	~KeyboardWidget();

	void KeyPress(QKeyEvent *k);//传入按下键
	void keyRelease(QKeyEvent *k);//传入释放键
	void StartTest();//开始测试
	void StopTest();//停止测试，一般不用调用，在按esc退出时自动停止
	bool TestIsStarted();//是否开始了键盘测试

protected:
	void SetupKeyLabel();//关联按键值
	bool KeyboardTestInit();//初始化
	
private:
	Ui::KeyboardWidget *ui;
	QQueue<KeyInfo> queueLabel;
    QMap<int, KeyInfo> mapKeyLabel;
    int key_F_count;//暂时不用（或者弃用）
    int key_esc_count;
	bool bKeyboardTest;
	QTimer timer;

protected slots:
	void timeoutSlot();

signals:
	void KeyboardTestEnd();//测试结束信号
};

#endif // KEYBOARDWIDGET_H
