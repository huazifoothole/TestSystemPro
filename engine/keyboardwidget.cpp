#include "keyboardwidget.h"
#include "ui_keyboardwidget.h"

#include <QKeyEvent>

KeyboardWidget::KeyboardWidget(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::KeyboardWidget)
{
	ui->setupUi(this);
	key_F_count = 0;
	key_esc_count = 0;
	bKeyboardTest = false;
	SetupKeyLabel();
	connect(&timer, SIGNAL(timeout()), this, SLOT(timeoutSlot()));
	//startTimer(1000);
}

KeyboardWidget::~KeyboardWidget()
{
	delete ui;
}

bool KeyboardWidget::TestIsStarted()
{
	return bKeyboardTest;
}

void KeyboardWidget::SetupKeyLabel()//设置键盘测试
{
	static int countTable[50] = {0};
	int keyValueTable[] =
    {
        Qt::Key_F1, Qt::Key_F2, Qt::Key_F3, Qt::Key_F4, Qt::Key_F5, Qt::Key_G,
        Qt::Key_F6, Qt::Key_F7, Qt::Key_F8, Qt::Key_F9, Qt::Key_F10, Qt::Key_A,
        Qt::Key_H, Qt::Key_I, Qt::Key_J, Qt::Key_K, Qt::Key_L, Qt::Key_B,
        Qt::Key_Control, Qt::Key_M, Qt::Key_N, Qt::Key_E, Qt::Key_F, Qt::Key_Backspace,
        Qt::Key_Slash, Qt::Key_Asterisk, Qt::Key_D, Qt::Key_7, Qt::Key_8, Qt::Key_9, Qt::Key_Escape,
        Qt::Key_Tab, Qt::Key_O, Qt::Key_C, Qt::Key_4, Qt::Key_5, Qt::Key_6,
        Qt::Key_Plus, Qt::Key_Up, Qt::Key_Minus, Qt::Key_1, Qt::Key_2, Qt::Key_3, Qt::Key_Return,
        Qt::Key_Left, Qt::Key_Down, Qt::Key_Right, Qt::Key_0, Qt::Key_Period
    };
	/*
    QLabel* lk78KeyLabelTable[] =
    {
        ui->labelKeyF1, ui->labelKeyF2, ui->labelKeyF3, ui->labelKeyF4, ui->labelKeyF5,  ui->labelKeyg,
        ui->labelKeyF6, ui->labelKeyF7, ui->labelKeyF8, ui->labelKeyF9, ui->labelKeyF10, ui->labelKeya,
        ui->labelKeyh, ui->labelKeyi, ui->labelKeyj, ui->labelKeyk, ui->labelKeyl, ui->labelKeyb,
        ui->labelKeyCtrl, ui->labelKeym, ui->labelKeyn, ui->labelKeye, ui->labelKeyf, ui->labelKeybackspace,
        ui->labelKeySlash, ui->labelKeyasterisk, ui->labelKeyd, ui->labelKey7, ui->labelKey8, ui->labelKey9, ui->labelKeyESC,
        ui->labelKeyTAB, ui->labelKeyo, ui->labelKeyc, ui->labelKey4, ui->labelKey5, ui->labelKey6,
        ui->labelKeyadd, ui->labelKeyup, ui->labelKeysub, ui->labelKey1, ui->labelKey2, ui->labelKey3, ui->labelKeyEnter,
        ui->labelKeyleft, ui->labelKeydown, ui->labelKeyright, ui->labelKey0, ui->labelKeydot
    };
	*/
    QLabel* lk56KeyLabelTable[] =
    {
        ui->labelKeyF1_2, ui->labelKeyF2_2, ui->labelKeyF3_2, ui->labelKeyF4_2, ui->labelKeyF5_2,  ui->labelKeyg_2,
        ui->labelKeyF6_2, ui->labelKeyF7_2, ui->labelKeyF8_2, ui->labelKeyF9_2, ui->labelKeyF10_2, ui->labelKeya_2,
        ui->labelKeyh_2, ui->labelKeyi_2, ui->labelKeyj_2, ui->labelKeyk_2, ui->labelKeyl_2, ui->labelKeyb_2,
        ui->labelKeyCtrl_2, ui->labelKeym_2, ui->labelKeyn_2, ui->labelKeye_2, ui->labelKeyf_2, ui->labelKeybackspace_2,
        ui->labelKeySlash_2, ui->labelKeyasterisk_2, ui->labelKeyd_2, ui->labelKey7_2, ui->labelKey8_2, ui->labelKey9_2, ui->labelKeyESC_2,
        ui->labelKeyTAB_2, ui->labelKeyo_2, ui->labelKeyc_2, ui->labelKey4_2, ui->labelKey5_2, ui->labelKey6_2,
        ui->labelKeyadd_2, ui->labelKeyup_2, ui->labelKeysub_2, ui->labelKey1_2, ui->labelKey2_2, ui->labelKey3_2, ui->labelKeyEnter_2,
        ui->labelKeyleft_2, ui->labelKeydown_2, ui->labelKeyright_2, ui->labelKey0_2, ui->labelKeydot_2
    };

    const QString keyNameTable[] =
    {
        "F1", "F2", "F3", "F4", "F5", "g",
        "F6", "F7", "F8", "F9", "F10", "a",
         "h", "i", "j", "k", "l", "b",
         "左Ctrl", "m", "n", "e", "f", "backspace",
        "/", "*", "d", "7", "8", "9", "ESC",
         "tab", "o", "c", "4", "5", "6",
        "+", "↑", "-", "1", "2", "3","Enter",
         "←", "↓", "→", "0", "."

    };

    const QString keyDisplayTable[] =
    {
        "F1", "F2", "F3", "F4", "F5", "快打",
        "F6", "F7", "F8", "F9", "F10", "机选一注",
        "K1", "K2", "K3", "K4", "K5", "机选五注",
        "logo", "对阵表", "复制票", "取消票", "重选", "退格",
        "/", "游戏*", "兑奖", "7\nPQRS", "8\nTUV", "9\nWXYZ","ESC",
        "TAB", "打印\n样票", "清除\n合计", "4\nGHI", "5\nJKL", "6\nMNO",
        "+", "↑", "-", "1", "2\nABC", "3\nDEF","确定\nEnter",
        "←", "↓", "→", "0", "."

    };

	KeyInfo keyInfo;
	for(unsigned int i = 0; i < sizeof(keyValueTable)/sizeof(keyValueTable[0]); i++)
    {
        keyInfo.pLabel = lk56KeyLabelTable[i];
        keyInfo.strName = keyNameTable[i];
        keyInfo.strDisplay = keyDisplayTable[i];
		countTable[i] = 0;
		keyInfo.count = &countTable[i];
        keyInfo.pLabel->setStyleSheet("background-color: rgb(168, 168, 168);");
        mapKeyLabel.insert(keyValueTable[i], keyInfo);
    }
}

void KeyboardWidget::KeyPress(QKeyEvent *k)
{
	if(!bKeyboardTest) return;
	KeyInfo keyInfo;
    QMap<int, KeyInfo>::iterator iter;
    iter = mapKeyLabel.find(k->key());

    qDebug("%X pressed", k->key());
    if(iter == mapKeyLabel.end())
    {
        //qDebug("no found\n");
        return;
    }
    keyInfo = iter.value();
    keyInfo.pLabel->setStyleSheet("");
    keyInfo.pLabel->setText(keyInfo.strName);
    ui->labelShowInfo->setText(tr("按键值为: ") + tr("【")+keyInfo.strName+tr("】"));
	*(keyInfo.count) += 1;
	qDebug("count = %d", *(keyInfo.count));
    /*
    if(k->key() == Qt::Key_F)
    {
        key_F_count++;
        if(key_F_count >= 3)
        {
			KeyboardTestInit();
        }
    }
	*/

}

void KeyboardWidget::keyRelease(QKeyEvent *k)
{
	if(!bKeyboardTest) return;
	KeyInfo keyInfo;
	QMap<int, KeyInfo>::iterator iter;
	iter = mapKeyLabel.find(k->key());

	qDebug("%X released", k->key());
	if(iter == mapKeyLabel.end())
	{
		//qDebug("no found\n");
		return;
	}
	keyInfo = iter.value();
	keyInfo.pLabel->setStyleSheet("background-color: rgb(168, 168, 168);");
	keyInfo.pLabel->setText(keyInfo.strDisplay);
        if(*(keyInfo.count) >= 1)
	{
		*(keyInfo.count) = 0;
              //  keyInfo.pLabel->setStyleSheet("background-color: rgb(0, 85, 255);");
                keyInfo.pLabel->setStyleSheet("background-color: rgb(1, 70, 113);");
	}

	if(k->key() == Qt::Key_Escape)
    {
		qApp->processEvents();//避免界面更新延时
		if(timer.isActive()) timer.stop();
        key_esc_count++;
        if(key_esc_count >= 3)
        {
			KeyboardTestInit();
			emit KeyboardTestEnd();
			bKeyboardTest = false;
        }
		else if(key_esc_count == 2)
		{
			KeyboardTestInit();
			key_esc_count = 2;
		}
		timer.start(500);
    }
	else
	{
		key_esc_count = 0;
		timer.stop();
	}
}

bool KeyboardWidget::KeyboardTestInit()
{
	KeyInfo keyInfo;
    QMap<int, KeyInfo>::iterator iter;
    for(iter = mapKeyLabel.begin(); iter != mapKeyLabel.end(); iter++)
    {
        keyInfo = iter.value();
        keyInfo.pLabel->setStyleSheet("background-color: rgb(168, 168, 168);");
        keyInfo.pLabel->setText(keyInfo.strDisplay);
		*(keyInfo.count) = 0;
    }
    key_F_count = 0;
    key_esc_count = 0;
	this->update();
	return true;
}
/**/
void KeyboardWidget::timeoutSlot()
{
	if(key_esc_count > 0) key_esc_count--;
}

void KeyboardWidget::StartTest()
{
	KeyboardTestInit();
	bKeyboardTest = true;
}

void KeyboardWidget::StopTest()
{
	KeyboardTestInit();
	bKeyboardTest = false;
}
