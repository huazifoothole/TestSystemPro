#include "choiceotherpro.h"
#include "ui_choiceotherpro.h"


choiceOtherPro::choiceOtherPro(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::choiceOtherPro)
{
    ui->setupUi(this);

    connect(this,SIGNAL(finished(int)),this,SLOT(changeLabelStyle(int)));
}

choiceOtherPro::~choiceOtherPro()
{
    delete ui;
}


void choiceOtherPro::on_pushButton_clicked()
{
     close();
}

void choiceOtherPro::on_pushButton_sc_clicked()
{
    emit endPro("sc");emit backMainInterface();
     delete ui;
}

void choiceOtherPro::on_pushButton_kaoji_clicked()
{
    emit endPro("kaoji");
     delete ui;
}

void choiceOtherPro::on_pushButton_guide_clicked()
{
    emit endPro("LATECHTest");
     delete ui;
}

void choiceOtherPro::changeLabelStyle(int i)
{
    emit backMainInterface();
    qDebug("close choiceDialog");
}
