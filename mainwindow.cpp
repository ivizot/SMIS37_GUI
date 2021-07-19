#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QHostAddress>
#include <QDebug>
#include <QDateTime>

extern QVector<QVector<double>> dataT;
extern QVector<QVector<double>> dataF;
extern QVector<QVector<double>> dataP;
extern QVector<double> dataX; // time vector

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , _socket(this)
{
    ui->setupUi(this);

    GHVPSform = new CGHVPSform(this);
    connect(GHVPSform, &CGHVPSform::sendCommand,
            [=](quint8 C, quint16 arg) {ethThread.addCommand(C,arg);}
    );
    GHVPSform->show();

    setupLED(ui);

    // graphs
    setupTempGraph(ui->plotTemp);
    setupFlowGraph(ui->plotFlow);
    setupMiscGraph(ui->plotMisc);
    setupGyrGraph(ui->plotGyr);
    setupPulsesGraph(ui->plotPulses);

    // socket
    ethThread.setAddr("192.168.0.99");
    ethThread.setPort(5000);
    connect(&ethThread, &CethThread::message, this, &MainWindow::onEthMessage);
    //connect(&ethThread, SIGNAL(message()), this, SLOT(onEthMessage()));
    connect(&ethThread, &CethThread::timeout, this, &MainWindow::onEthTimeout);
    connect(&ethThread, &CethThread::connected, this, &MainWindow::onEthConnect);
    ethThread.start(QThread::NormalPriority);

    inputDialog.setOkButtonText("Save");
    inputDialog.setCancelButtonText("Cancel");
    inputDialog.setInputMode(QInputDialog::DoubleInput);
    inputDialog.setDoubleDecimals(3);
    connect(&inputDialog, &QInputDialog::accepted, this, &MainWindow::onInputDialog);

    // PulseGen ON buttons
    for(int i=0;i<8;i++)
    {
        QPushButton* H=ui->centralwidget->findChild<QPushButton*>
                (QString("PG_on")+QString::number(i));
        connect(H, SIGNAL(clicked()), this, SLOT(PG_onClicked()));
    }
    // PulseGen OFF buttons
    for(int i=0;i<8;i++)
    {
        QPushButton* H=ui->centralwidget->findChild<QPushButton*>
                (QString("PG_off")+QString::number(i));
        connect(H, SIGNAL(clicked()), this, SLOT(PG_offClicked()));
    }
    // PulseGen Time display
    for(int i=0;i<8;i++)
    {
        QSpinBox* H=ui->centralwidget->findChild<QSpinBox*>
                (QString("PG_T")+QString::number(i));
        connect(H, SIGNAL(editingFinished()), this, SLOT(PG_TvalueChanged()));
    }
    // PulseGen Duration display
    for(int i=0;i<8;i++)
    {
        QSpinBox* H=ui->centralwidget->findChild<QSpinBox*>
                (QString("PG_D")+QString::number(i));
        connect(H, SIGNAL(editingFinished()), this, SLOT(PG_DvalueChanged()));
    }
    // PulseGen plot double-click
    connect(ui->plotPulses, SIGNAL(mouseDoubleClick(QMouseEvent*)),
            this, SLOT(plotPulses_mouseDoubleClick(QMouseEvent*)));
    //pulseGen period change
    connect(ui->I_PGperiod, SIGNAL(editingFinished()), this, SLOT(I_PGperiod_editingFinished()));

    initLog();

}

MainWindow::~MainWindow()
{
    delete ui;
    endLog();
}

void MainWindow::onEthTimeout()
{
    ui->labelMCU->setText("Offline");
    setEnabled(false);
}

void MainWindow::onEthConnect()
{
    ui->labelMCU->setText("Connected");
    setEnabled(true);
}

void MainWindow::onEthMessage()
{
    quint8 C;
    QVector<quint8> D;
    ethThread.getMessage(C,D);
    //qDebug() << "eth message:" << C;
    if(C == COMM_DATA_SLOW) {
        //qDebug()<<D.size();
        memcpy(&data,D.data(),sizeof(TData));
        updateData();
        logData(data);
    }
    else if (C == COMM_DATA_ALL) {
        memcpy(&data,D.data(),sizeof(TData));
        memcpy(&dataFast,D.data()+sizeof(TData),sizeof(TDataFast));
        updateData();
        updateFastData();
        logDataFast(data,dataFast);
    }
}

void MainWindow::updateData()
{
    //qDebug() << "updateData";
    for(int i=0;i<8;i++)
    {
        LEDindicator * L;
        // Interlocks IN LEDs
        L = ui->centralwidget->findChild<LEDindicator *>(QString("LEDinterlockIn")+QString::number(i));
        if(L!=nullptr) L->setState((data.interlocksIn & (1<<i)) > 0);
        // Latched Interlocks IN LEDs
        L = ui->centralwidget->findChild<LEDindicator *>(QString("LEDlatchedInterlockIn")+QString::number(i));
        if(L!=nullptr) L->setState((data.latchedInterlocksIn & (1<<i)) > 0);
        // Interlocks OUTPUT LEDs
        L = ui->centralwidget->findChild<LEDindicator *>(QString("LEDinterlockOut")+QString::number(i));
        if(L!=nullptr) L->setState((data.interlocksOut & (1<<i)) > 0);
        // Interlocks OUTPUT TEMP
        L = ui->centralwidget->findChild<LEDindicator *>(QString("LEDlatchedInterlockOutT")+QString::number(i));
        if(L!=nullptr) L->setState((data.latchedInterlocksOut[i] & LI_WT) > 0);
        // Interlocks OUTPUT FLOW
        L = ui->centralwidget->findChild<LEDindicator *>(QString("LEDlatchedInterlockOutF")+QString::number(i));
        if(L!=nullptr) L->setState((data.latchedInterlocksOut[i] & LI_WF) > 0);
        // Interlocks OUTPUT DOORS
        L = ui->centralwidget->findChild<LEDindicator *>(QString("LEDlatchedInterlockOutS")+QString::number(i));
        if(L!=nullptr) L->setState((data.latchedInterlocksOut[i] & LI_ES) > 0);
        // Interlocks OUTPUT DEVICES
        L = ui->centralwidget->findChild<LEDindicator *>(QString("LEDlatchedInterlockOutD")+QString::number(i));
        if(L!=nullptr) L->setState((data.latchedInterlocksOut[i] & LI_DD) > 0);
    }

    // temp leds
    for(int i=0;i<TScount;i++)
    {
        LEDindicator * L;
        L = ui->centralwidget->findChild<LEDindicator *>(QString("LEDwaterTemp")+QString::number(i));
        if(L!=nullptr) L->setState((data.waterTemp & (1<<i))>0);
    }
    //flow leds
    for(int i=0;i<FScount;i++)
    {
        LEDindicator * L;
        L = ui->centralwidget->findChild<LEDindicator *>(QString("LEDwaterFlow")+QString::number(i));
        if(L!=nullptr) L->setState((data.waterFlow & (1<<i))>0);
    }
    //pressure leds
    for(int i=0;i<PScount;i++)
    {
        LEDindicator * L;
        L = ui->centralwidget->findChild<LEDindicator *>(QString("LEDwaterPressure")+QString::number(i));
        if(L!=nullptr) L->setState((data.waterPressure & (1<<i))>0);
    }

    //============= GSPS
    ui->GSPSdata_I->setText(QString::number(0.001*data.GSPSData.I,'f',3)+QString(" A"));
    ui->GSPSdata_Iset->setText(QString::number(0.001*data.GSPSData.Iset,'f',3)+QString(" A"));
    ui->GSPSdata_U->setText(QString::number(0.001*data.GSPSData.U,'f',3)+QString(" V"));
    ui->LED_GSPSinterlock->setState(data.latchedInterlocksOut[IBO_GSPS]>0);
    // GSPS Status
    // [0] online/offline
    // [1] output ON/OFF
    // [2] ERROR
    // [3] ramping
    ui->LED_GSPSout->setState(data.GSPSData.state & (1<<1));
    ui->LED_GSPSerror->setState(data.GSPSData.state & (1<<2));
    ui->LED_GSPSramping->setState(data.GSPSData.state & (1<<3));
    ui->gbGSPS->setEnabled(data.GSPSData.state & (1<<0));
    //============= /GSPS

    //============= GIP
    //GIP intelock leds
    ui->LEDlatchedInterlockOutGIPD->setState(data.GIPData.latchedInterlock & LI_DD);
    ui->LEDlatchedInterlockOutGIPS->setState(data.GIPData.latchedInterlock & LI_ES);
    // main control
    ui->GIP_U->setText(QString::number(0.001*data.GIPData.U,'f',1)+QString(" kV"));
    ui->GIP_I->setText(QString::number(0.001*data.GIPData.I,'f',1)+QString(" mA"));
    ui->LED_GIPbadVac->setState(data.GIPData.S & (1<<2));
    ui->LED_GIPlowU->setState(data.GIPData.S & (1<<1));
    ui->LED_GIPout->setState(data.GIPData.S & (1<<0));
    ui->LED_GIPinterlock->setState(data.GIPData.latchedInterlock>0);
    //============= /GIP

    //============= GFPS
    ui->GFPSdata_Irms->setText(QString::number(0.001*data.GFPSData.Irms,'f',2)+QString(" A"));
    ui->GFPSdata_Pcur->setText(QString::number(0.01*data.GFPSData.Pcur,'f',2)+QString("%"));
    ui->GFPSdata_Pset->setText(QString::number(0.01*data.GFPSData.Pset,'f',2)+QString("%"));
    // 0 LOAD relay
    // 1 interlock IN
    // 2 interlock OUT
    // 3 mode: 0 ramp power, 1 - direct control
    // 4 OVR: current overrange
    // 5 TRIAC fault
    // 6 device online (local state)
    ui->LED_GFPSload->setState(data.GFPSData.state & (1<<0));
    ui->LED_GFPSramp->setState(~(data.GFPSData.state & (1<<3)));
    ui->LED_GFPSdirect->setState(data.GFPSData.state & (1<<3));
    ui->LED_GFPStriac->setState(data.GFPSData.state & (1<<5));
    //ui->gbGFPS->setEnabled(data.GFPSData.state & (1<<6));
    //============= /GFPS

    //============= GHVPS
    ui->GHVPSdata_ZU2->setText(QString::number(data.GHVPSData.ZU[2],'f',0)+QString(" V"));
    if(!ui->GHVPSdata_ZU5->hasFocus()) {
        ui->GHVPSdata_ZU5->blockSignals(true);
        ui->GHVPSdata_ZU5->setValue(data.GHVPSData.ZU[5]);
        ui->GHVPSdata_ZU5->blockSignals(false);
    }
    //data.GHVPSData.ZU
    //============= /GHVPS

    //============= Pulse generator
    for(int i=0;i<8;i++)
    {
        LEDindicator* L = ui->centralwidget->findChild<LEDindicator*>(QString("LED_PG_enabled")+QString::number(i));
        L->setState(data.pulseGenData.S & (1<<i));

        QSpinBox* T = ui->centralwidget->findChild<QSpinBox*>(QString("PG_T")+QString::number(i));
        if(!T->hasFocus()) {
            T->blockSignals(true);
            T->setValue(10*data.pulseGenData.T[i]);
            T->blockSignals(false);
        }

        QSpinBox* D = ui->centralwidget->findChild<QSpinBox*>(QString("PG_D")+QString::number(i));
        if(!D->hasFocus()) {
            D->blockSignals(true);
            D->setValue(10*data.pulseGenData.D[i]);
            D->blockSignals(false);
        }

        QPushButton* B = ui->centralwidget->findChild<QPushButton*>(QString("PG_on")+QString::number(i));
        B->setEnabled(!(data.pulseGenData.S & (1<<i)));
        B = ui->centralwidget->findChild<QPushButton*>(QString("PG_off")+QString::number(i));
        B->setEnabled(data.pulseGenData.S & (1<<i));
    }
    ui->PG_pulsesN->setText(QString::number(data.pulseGenData.N));
    if(data.pulseGenData.P != ui->progressBar->maximum())
        ui->progressBar->setMaximum(data.pulseGenData.P);
    ui->progressBar->setValue(data.pulseGenData.P-data.pulseGenData.dT);
    if(data.pulseGenData.TIMER) {
        ui->labelPG->setText("RUNNING");
        ui->labelPG->setStyleSheet("color: rgb(15, 163, 0);");
        ui->B_PGstart->setEnabled(false);
        ui->B_PGstop->setEnabled(true);
        ui->B_PGsingle->setEnabled(false);
    } else {
        ui->labelPG->setText("STOPPED");
        ui->labelPG->setStyleSheet("color: rgb(239, 41, 41);");
        ui->B_PGstart->setEnabled(true);
        ui->B_PGstop->setEnabled(false);
        ui->B_PGsingle->setEnabled(true);
    }
    if(!ui->I_PGperiod->hasFocus()) {
        ui->I_PGperiod->blockSignals(true);
        ui->I_PGperiod->setValue(data.pulseGenData.P);
        ui->I_PGperiod->blockSignals(false);
    }


    for(int i=0;i<8;i++) {
        QCPBars* B = (QCPBars*)ui->plotPulses->plottable(i);
        B->setBaseValue(0.01*data.pulseGenData.T[i]);
        B->setBrush(QBrush((data.pulseGenData.S & (1<<i))?Qt::blue:Qt::lightGray));
        QVector<double> X,Y;
        X<<i+1;
        Y<<0.01*data.pulseGenData.D[i];
        B->setData(X,Y);
    }
    //ui->plotPulses->rescaleAxes();
    ui->plotPulses->replot();
    //============= /Pulse generator



    //status LEDs
    //qDebug()<<data.interlocksIn;
    ui->LEDtemp->setState(data.waterTemp>0);
    ui->LEDflow->setState(data.waterFlow>0);
    ui->LEDpress->setState(data.waterPressure>0);
    ui->LEDcages->setState(data.interlocksIn & (1<<IBI_CAGE0) ||
                           data.interlocksIn & (1<<IBI_CAGE1) ||
                           data.interlocksIn & (1<<IBI_CAGE2) ||
                           data.interlocksIn & (1<<IBI_CAGE3) );

    // slow graphs
    double dT = 0.001*(graphTS-QDateTime::currentDateTime().currentMSecsSinceEpoch());
    graphTS = QDateTime::currentDateTime().currentMSecsSinceEpoch();
    dataX.append(0);
    for(int i=0;i<dataX.size()-1;i++)
        dataX[i]+=dT;

    for(int i=0;i<TplotsN;i++)
        dataT[i].append(0.01*data.waterData.T[Tplots[i].ind]);
    for(int i=0;i<FplotsN;i++)
        dataF[i].append(0.01*data.waterData.F[Fplots[i].ind]);
    for(int i=0;i<PplotsN;i++)
        dataP[i].append(0.01*data.waterData.P[Pplots[i].ind]);

    //remove points beyond min_time
    if (dataX.first() < -60.0)
    {
        dataX.removeFirst();
        for(int i=0;i<TplotsN;i++)
            dataT[i].removeFirst();
        for(int i=0;i<FplotsN;i++)
            dataF[i].removeFirst();
        for(int i=0;i<PplotsN;i++)
            dataP[i].removeFirst();
    }
    for(int i=0;i<TplotsN;i++)
        ui->plotTemp->graph(i)->setData(dataX,dataT[i]);
    ui->plotTemp->replot();
    for(int i=0;i<FplotsN;i++)
        ui->plotFlow->graph(i)->setData(dataX,dataF[i]);
    for(int i=0;i<PplotsN;i++)
        ui->plotFlow->graph(FplotsN+i)->setData(dataX,dataP[i]);
    ui->plotFlow->replot();
}

void MainWindow::updateFastData()
{
    qDebug()<<"fast data";
    QVector<double>\
            GU(FAST_ADC_COUNT-1),\
            GI(FAST_ADC_COUNT-1),\
            MI(FAST_ADC_COUNT-1),\
            X(FAST_ADC_COUNT-1);
    for(int i=0;i<FAST_ADC_COUNT-1;i++)
    {
        X[i] = (0.001*FAST_ADC_TPP*i);
        GU[i] = dataFast.GU[i];
        GI[i] = dataFast.GI[i];
        MI[i] = dataFast.MI[i];
    }

    ui->plotGyr->graph(0)->setData(X,GU);
    ui->plotGyr->graph(1)->setData(X,GI);
    ui->plotGyr->graph(2)->setData(X,MI);

    // GU cursor
    double V = dataFast.GU[FAST_ADC_COUNT-1];
    ((QCPItemLine*) ui->plotGyr->item(0))->start->setCoords(ui->plotGyr->xAxis->range().lower,V);
    ((QCPItemLine*) ui->plotGyr->item(0))->end->setCoords(ui->plotGyr->xAxis->range().upper,V);
    ((QCPItemText*) ui->plotGyr->item(1))->position->setCoords(2.0, V);
    ((QCPItemText*) ui->plotGyr->item(1))->setText(QString::number(V,'f',1));

    V = dataFast.GI[FAST_ADC_COUNT-1];
    ((QCPItemLine*) ui->plotGyr->item(2))->start->setCoords(ui->plotGyr->xAxis->range().lower,V);
    ((QCPItemLine*) ui->plotGyr->item(2))->end->setCoords(ui->plotGyr->xAxis->range().upper,V);
    ((QCPItemText*) ui->plotGyr->item(3))->position->setCoords(1.7, V);
    ((QCPItemText*) ui->plotGyr->item(3))->setText(QString::number(V,'f',1));

    V = dataFast.MI[FAST_ADC_COUNT-1];
    ((QCPItemLine*) ui->plotGyr->item(4))->start->setCoords(ui->plotGyr->xAxis->range().lower,V);
    ((QCPItemLine*) ui->plotGyr->item(4))->end->setCoords(ui->plotGyr->xAxis->range().upper,V);
    ((QCPItemText*) ui->plotGyr->item(5))->position->setCoords(1.4, V);
    ((QCPItemText*) ui->plotGyr->item(5))->setText(QString::number(V,'f',1));



    //    ((QCPItemLine*) ui->plotGyr->item(2))->start->setCoords(ui->plotGyr->xAxis->range().lower,10);
    //    ((QCPItemLine*) ui->plotGyr->item(2))->end->setCoords(ui->plotGyr->xAxis->range().upper,100);

    //    ((QCPItemLine*) ui->plotGyr->item(4))->start->setCoords(ui->plotGyr->xAxis->range().lower,10);
    //    ((QCPItemLine*) ui->plotGyr->item(4))->end->setCoords(ui->plotGyr->xAxis->range().upper,100);
    ui->plotGyr->replot();
}

void MainWindow::showInputDialog(uint8_t ID)
{
    QString S;
    double min=0,max=0,val=0;
    switch (ID) {
    case COMM_GSPS_SET_I:
        S = "GSPS target current, A:";
        min = 0;
        max = 60;
        val = data.GSPSData.Iset*0.01;
        break;
    default:
        break;
    }

    inputDialog.setLabelText(S);
    inputDialog.setDoubleMinimum(min);
    inputDialog.setDoubleMaximum(max);
    inputDialog.setDoubleValue(val);
    inputDialog.show();
}

void MainWindow::setEnabled(bool v)
{
    //ui->devicesTab->setEnabled(v);
    //ui->tab_gyrotron->setEnabled(v);
    //ui->gbGSPS->setEnabled(v);
    //ui->GSPScurrent->setEnabled(v);

    //    QObject* h;
    //    foreach(h,ui->devicesTab->children()) {
    //        (QWidget*)h->setEnabled(v);
    //    }
}




void MainWindow::on_B_resetInterlocks_clicked()
{
    ethThread.addCommand(COMM_INTERLOCKS_RESET,0);
}


void MainWindow::on_B_PGsingle_clicked()
{
    ethThread.addCommand(COMM_PULSEGEN_PULSE,0);
}


void MainWindow::on_B_PGstart_clicked()
{
    ethThread.addCommand(COMM_PULSEGEN_SET_TIMER,1);
}


void MainWindow::on_B_PGstop_clicked()
{
    ethThread.addCommand(COMM_PULSEGEN_SET_TIMER,0);
}


void MainWindow::I_PGperiod_editingFinished()
{
    ui->I_PGperiod->clearFocus();
    ethThread.addCommand(COMM_PULSEGEN_SET_P,(quint16)ui->I_PGperiod->value());
}

void MainWindow::plotPulses_mouseDoubleClick(QMouseEvent * event)
{
    ui->plotPulses->xAxis->rescale();
    ui->plotPulses->replot();
}

void MainWindow::on_B_GSPSsetCurrent_clicked()
{
    showInputDialog(COMM_GSPS_SET_I);
}

void MainWindow::onInputDialog()
{
    qDebug() << inputDialog.doubleValue();
}

void MainWindow::PG_onClicked()
{
    QPushButton* H = (QPushButton*)sender();
    quint8 i = QString(H->objectName().back()).toInt();
    quint8 S = data.pulseGenData.S |= (1<<i);
    ethThread.addCommand(2,S);
}

void MainWindow::PG_offClicked()
{
    QPushButton* H = (QPushButton*)sender();
    quint8 i = QString(H->objectName().back()).toInt();
    quint8 S = data.pulseGenData.S &= ~(1<<i);
    ethThread.addCommand(2,S);
}

void MainWindow::PG_TvalueChanged()
{
    QSpinBox* H = (QSpinBox*)sender();
    H->clearFocus();
    quint8 i = QString(H->objectName().back()).toInt();
    quint16 v = H->value();
    ethThread.addCommand(6+i,(quint16)(v/10));
}

void MainWindow::PG_DvalueChanged()
{
    QSpinBox* H = (QSpinBox*)sender();
    H->clearFocus();
    quint8 i = QString(H->objectName().back()).toInt();
    quint16 v = H->value();
    ethThread.addCommand(14+i,(quint16)(v/10));
}


void MainWindow::on_GHVPSdata_ZU5_editingFinished()
{
    ui->GHVPSdata_ZU5->clearFocus();
    ethThread.addCommand(COMM_GHVPS_SET_U,ui->GHVPSdata_ZU5->value());
}

