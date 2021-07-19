#include "cghvpsform.h"
#include "ui_cghvpsform.h"

CGHVPSform::CGHVPSform(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CGHVPSform)
{
    ui->setupUi(this);
    LEDindicator * L;
    L = ui->ZU_LED_Control;
    L->setDiameter(20);
    L->setColorOn(Qt::red);
    L = ui->ZU_LED_chargeState;
    L->setDiameter(20);
    L->setColorOn(Qt::green);
    for(int i=0;i<8;i++)
    {
        L = this->findChild<LEDindicator *>(QString("ZU_LED_S")+QString::number(i));
        if(L != nullptr) {
            L->setDiameter(20);
            //L->setColorOff(Qt::gray);
            L->setColorOn(Qt::green);
            L->setState(false);
        }
    }
    for(int i=0;i<12;i++)
    {
        L = this->findChild<LEDindicator *>(QString("ZU_LED_E")+QString::number(i));
        if(L != nullptr) {
            L->setDiameter(20);
            //L->setColorOff(Qt::gray);
            L->setColorOn(Qt::red);
            L->setState(false);
        }
    }
    for(int i=0;i<=11;i++)
    {
        L = this->findChild<LEDindicator *>(QString("CU_LED_S")+QString::number(i));
        if(L != nullptr) {
            L->setDiameter(20);
            //L->setColorOff(Qt::gray);
            L->setColorOn(Qt::green);
            L->setState(false);
        }
    }
    for(int i=0;i<=10;i++)
    {
        L = this->findChild<LEDindicator *>(QString("CU_LED_E")+QString::number(i));
        if(L != nullptr) {
            L->setDiameter(20);
            //L->setColorOff(Qt::gray);
            L->setColorOn(Qt::red);
            L->setState(false);
        }
    }

}

CGHVPSform::~CGHVPSform()
{
    delete ui;
}

void CGHVPSform::setData(const TGHVPS D)
{
    this->data = D;
}


void CGHVPSform::on_pushButton_clicked()
{
}

