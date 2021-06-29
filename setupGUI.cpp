#include "setupGUI.h"
#include "../Main/FIRMWARE/SMIS37/include/headers.h"

void setupLED(Ui_MainWindow* ui) {
    // GSPS controls init;
    ui->LED_GSPSerror->setColorOn(Qt::red);
    ui->LED_GSPSerror->setDiameter(20);
    ui->LED_GSPSout->setColorOn(Qt::green);
    ui->LED_GSPSout->setDiameter(20);
    ui->LED_GSPSinterlock->setColorOn(Qt::red);
    ui->LED_GSPSinterlock->setColorOff(Qt::green);
    ui->LED_GSPSinterlock->setDiameter(20);
    ui->LED_GSPSramping->setColorOn(Qt::cyan);
    ui->LED_GSPSramping->setDiameter(20);

    // GIP controls init
    ui->LED_GIPlowU->setColorOn(Qt::red);
    ui->LED_GIPlowU->setDiameter(20);
    ui->LED_GIPbadVac->setColorOn(Qt::red);
    ui->LED_GIPbadVac->setDiameter(20);
    ui->LED_GIPinterlock->setColorOff(Qt::green);
    ui->LED_GIPinterlock->setColorOn(Qt::red);
    ui->LED_GIPinterlock->setDiameter(20);
    ui->LED_GIPout->setColorOn(Qt::green);
    ui->LED_GIPout->setDiameter(20);

    // GFPS controls init
    ui->LED_GFPSinterlock->setColorOn(Qt::green);
    ui->LED_GFPSinterlock->setColorOff(Qt::red);
    ui->LED_GFPSinterlock->setDiameter(20);
    ui->LED_GFPSload->setColorOn(Qt::green);
    ui->LED_GFPSload->setDiameter(20);
    ui->LED_GFPSovr->setColorOn(Qt::red);
    ui->LED_GFPSovr->setDiameter(20);
    ui->LED_GFPSramp->setColorOn(Qt::green);
    //ui->LED_GFPSramp->setColorOff(Qt::green);
    ui->LED_GFPSramp->setDiameter(20);
    ui->LED_GFPSdirect->setColorOn(Qt::green);
    ui->LED_GFPSdirect->setDiameter(20);
    ui->LED_GFPStriac->setColorOn(Qt::red);
    ui->LED_GFPStriac->setDiameter(20);

    // GHVPS controls init
    ui->LED_GHVPScontactor->setColorOn(Qt::green);
    ui->LED_GHVPScontactor->setDiameter(20);
    ui->LED_GHVPSready->setColorOn(Qt::green);
    ui->LED_GHVPSready->setDiameter(20);
    ui->LED_GHVPSerror->setColorOn(Qt::red);
    ui->LED_GHVPSerror->setDiameter(20);
    ui->LED_GHVPSinterlock->setColorOff(Qt::red);
    ui->LED_GHVPSinterlock->setColorOn(Qt::green);
    ui->LED_GHVPSinterlock->setDiameter(20);
    ui->LED_GHVPSchargeReady->setColorOn(Qt::green);
    ui->LED_GHVPSchargeReady->setDiameter(20);


    //status bar LEDs
    ui->LEDtemp->setColorOff(Qt::green);
    ui->LEDtemp->setColorOn(Qt::red);
    ui->LEDtemp->setDiameter(20);

    ui->LEDflow->setColorOff(Qt::green);
    ui->LEDflow->setColorOn(Qt::red);
    ui->LEDflow->setDiameter(20);

    ui->LEDpress->setColorOff(Qt::green);
    ui->LEDpress->setColorOn(Qt::red);
    ui->LEDpress->setDiameter(20);

    ui->LEDcages->setColorOff(Qt::green);
    ui->LEDcages->setColorOn(Qt::red);
    ui->LEDcages->setDiameter(20);

    //INterlocks LEDS
    LEDindicator * L;
    for(int i=0;i<8;i++)
    {
        L = ui->centralwidget->findChild<LEDindicator *>(QString("LEDinterlockIn")+QString::number(i));
        if(L != nullptr) {
            L->setDiameter(20);
            L->setColorOff(Qt::green);
            L->setColorOn(Qt::red);
            L->setState(false);
        }

        L = ui->centralwidget->findChild<LEDindicator *>(QString("LEDlatchedInterlockIn")+QString::number(i));
        if(L != nullptr) {
            L->setDiameter(20);
            L->setColorOff(Qt::green);
            L->setColorOn(Qt::red);
            L->setState(false);
        }

        L = ui->centralwidget->findChild<LEDindicator *>(QString("LEDinterlockOut")+QString::number(i));
        if(L != nullptr) {
            L->setDiameter(20);
            L->setColorOff(Qt::green);
            L->setColorOn(Qt::red);
            L->setState(false);
        }

        L = ui->centralwidget->findChild<LEDindicator *>(QString("LEDlatchedInterlockOutT")+QString::number(i));
        if(L != nullptr) {
            L->setDiameter(20);
            L->setColorOff(Qt::green);
            L->setColorOn(Qt::red);
            L->setState(false);
        }
        L = ui->centralwidget->findChild<LEDindicator *>(QString("LEDlatchedInterlockOutF")+QString::number(i));
        if(L != nullptr) {
            L->setDiameter(20);
            L->setColorOff(Qt::green);
            L->setColorOn(Qt::red);
            L->setState(false);
        }
        L = ui->centralwidget->findChild<LEDindicator *>(QString("LEDlatchedInterlockOutS")+QString::number(i));
        if(L != nullptr) {
            L->setDiameter(20);
            L->setColorOff(Qt::green);
            L->setColorOn(Qt::red);
            L->setState(false);
        }

        L = ui->centralwidget->findChild<LEDindicator *>(QString("LEDlatchedInterlockOutD")+QString::number(i));
        if(L != nullptr) {
            L->setDiameter(20);
            L->setColorOff(Qt::green);
            L->setColorOn(Qt::red);
            L->setState(false);
        }
    }
    //GIP interlocks
    L = ui->LEDlatchedInterlockOutGIPD;
    L->setDiameter(20);
    L->setColorOff(Qt::green);
    L->setColorOn(Qt::red);
    L = ui->LEDlatchedInterlockOutGIPS;
    L->setDiameter(20);
    L->setColorOff(Qt::green);
    L->setColorOn(Qt::red);
    // water Temp LEDS
    for(int i=0;i<TScount;i++)
    {
        L = ui->centralwidget->findChild<LEDindicator *>(QString("LEDwaterTemp")+QString::number(i));
        if(L!=nullptr)
        {
            L->setDiameter(20);
            L->setColorOff(Qt::green);
            L->setColorOn(Qt::red);
            L->setState(false);
        }
    }
    //water Flow LEDS
    for(int i=0;i<FScount;i++)
    {
        L = ui->centralwidget->findChild<LEDindicator *>(QString("LEDwaterFlow")+QString::number(i));
        if(L!=nullptr)
        {
            L->setDiameter(20);
            L->setColorOff(Qt::green);
            L->setColorOn(Qt::red);
            L->setState(false);
        }
    }

    // water Pressure LEDs
    for(int i=0;i<PScount;i++)
    {
        L = ui->centralwidget->findChild<LEDindicator *>(QString("LEDwaterPressure")+QString::number(i));
        if(L!=nullptr)
        {
            L->setDiameter(20);
            L->setColorOff(Qt::green);
            L->setColorOn(Qt::red);
            L->setState(false);
        }
    }


    //pulse generator LEDs
    for(int i=0;i<8;i++)
    {
        LEDindicator* H = ui->centralwidget->findChild<LEDindicator *>
                (QString("LED_PG_enabled")+QString::number(i));
        H->setColorOn(Qt::cyan);
        H->setDiameter(20);
    }




};
