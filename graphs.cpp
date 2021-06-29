#include "graphs.h"

QVector<QVector<double>> dataT(TplotsN);
QVector<QVector<double>> dataF(FplotsN);
QVector<QVector<double>> dataP(PplotsN);
QVector<double> dataX; // time vector


void setupGraph(QCustomPlot* plot)
{
    plot->setLocale(QLocale(QLocale::English, QLocale::UnitedKingdom));
    plot->legend->setVisible(true);
    plot->legend->setMargins(QMargins(5,2,2,5));
    plot->legend->setRowSpacing(-3);
    plot->legend->setFillOrder(QCPLegend::foColumnsFirst);
    plot->legend->setWrap(6);
    //plot->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignTop|Qt::AlignLeft);
    QFont legendFont = plot->legend->font();
    legendFont.setPointSize(8); // and make a bit smaller for legend
    plot->legend->setFont(legendFont);

    QCPLayoutGrid *subLayout = new QCPLayoutGrid;
    plot->plotLayout()->insertRow(0);
    plot->plotLayout()->addElement(0, 0, subLayout);
    plot->plotLayout()->setRowStretchFactor(0,0.001);
    subLayout->addElement(0, 0, plot->legend); // add legend
    //subLayout->addElement(0, 1, dummyElement); // add dummy element below legend
    //subLayout->setColumnStretchFactor(0, 0.001); // make legend cell (in row 0) take up as little vertical space as possible
    subLayout->setMargins(QMargins(0,0,2,0));
}


void setupTempGraph(QCustomPlot *plot)
{
    setupGraph(plot);
    for(int i=0;i<TplotsN;i++)
    {
        QCPGraph* H = plot->addGraph();
        QPen p;
        p.setColor(colors[i]);
        p.setWidth(2);
        H->setPen(p);
        H->setName(Tplots[i].name);
    }
    plot->yAxis->setLabel("Temperature, degC");
    plot->yAxis->setRange(-10,80);
    plot->xAxis->setLabel("Time, s");
    plot->xAxis->setRange(-60,0);
}


void setupFlowGraph(QCustomPlot *plot)
{
    setupGraph(plot);
    for(int i=0;i<FplotsN;i++)
    {
        QCPGraph* H = plot->addGraph();
        QPen p;
        p.setColor(colors[i]);
        p.setWidth(2);
        H->setPen(p);
        H->setName(Fplots[i].name);
    }
    plot->yAxis->setLabel("Flow, l/min");
    plot->yAxis->setRange(-10,30);

    for(int i=0;i<PplotsN;i++)
    {
        QCPGraph* H = plot->addGraph(plot->xAxis,plot->yAxis2);
        QPen p;
        p.setColor(colors[i]);
        p.setWidth(2);
        H->setPen(p);
        H->setName(Pplots[i].name);
    }
    plot->yAxis2->setLabel("Pressure, bar");
    plot->yAxis2->setRange(0,6);
    plot->yAxis2->setVisible(true);

    plot->xAxis->setLabel("Time, s");
    plot->xAxis->setRange(-60,0);
}



void setupMiscGraph(QCustomPlot *plot)
{
    setupGraph(plot);
    QCPGraph* H;
    QPen p;
    p.setWidth(2);

    H = plot->addGraph();
    p.setColor(colors[0]);
    H->setPen(p);
    H->setName("GIP U, V");

    H = plot->addGraph();
    p.setColor(colors[1]);
    H->setPen(p);
    H->setName("GIP I, uA");

    plot->yAxis->setLabel("Data");
    plot->yAxis->setRange(0,10000);

    plot->xAxis->setLabel("Time, s");
    plot->xAxis->setRange(-60,0);

}

void setupGyrGraph(QCustomPlot *plot)
{
    setupGraph(plot);
    QCPGraph* H;
    QCPItemLine *line;
    QCPItemText *textLabel;
    QPen p;
    p.setWidth(2);

    plot->xAxis->setLabel("Time, ms");
    plot->xAxis->setRange(-0.2,0.001*FAST_ADC_COUNT*FAST_ADC_TPP);
    QSharedPointer<QCPAxisTickerFixed> Xticker(new QCPAxisTickerFixed);
    plot->xAxis->setTicker(Xticker);
    Xticker->setTickStep(0.2);
    Xticker->setScaleStrategy(QCPAxisTickerFixed::ssNone); // and no scaling

    H = plot->addGraph();
    p.setColor(colors[0]);
    H->setPen(p);
    H->setName("Voltage, kV");
    line = new QCPItemLine(plot);
    line->start->setCoords(plot->xAxis->range().lower,0);
    line->end->setCoords(plot->xAxis->range().upper,0);
    line->setPen(QPen(p));
    textLabel = new QCPItemText(plot);
    textLabel->setPositionAlignment(Qt::AlignBottom|Qt::AlignLeft);
    textLabel->position->setCoords(2.0, 0); // place position at center/top of axis rect
    textLabel->setText("0.0");
    textLabel->setPen(p);
    textLabel->setBrush(QBrush(Qt::white));
    textLabel->setPadding(QMargins(2,2,2,2));


    H = plot->addGraph();
    p.setColor(colors[1]);
    H->setPen(p);
    H->setName("Current, A");
    line = new QCPItemLine(plot);
    line->start->setCoords(plot->xAxis->range().lower,0);
    line->end->setCoords(plot->xAxis->range().upper,0);
    line->setPen(QPen(p));
    textLabel = new QCPItemText(plot);
    textLabel->setPositionAlignment(Qt::AlignBottom|Qt::AlignLeft);
    textLabel->position->setCoords(1.7, 0); // place position at center/top of axis rect
    textLabel->setText("0.0");
    textLabel->setPen(p);
    textLabel->setBrush(QBrush(Qt::white));
    textLabel->setPadding(QMargins(2,2,2,2));

    H = plot->addGraph();
    p.setColor(colors[2]);
    H->setPen(p);
    H->setName("MF current, kA");
    line = new QCPItemLine(plot);
    line->start->setCoords(plot->xAxis->range().lower,0);
    line->end->setCoords(plot->xAxis->range().upper,0);
    line->setPen(QPen(p));
    textLabel = new QCPItemText(plot);
    textLabel->setPositionAlignment(Qt::AlignBottom|Qt::AlignLeft);
    textLabel->position->setCoords(1.4, 0); // place position at center/top of axis rect
    textLabel->setText("0.0");
    textLabel->setPen(p);
    textLabel->setBrush(QBrush(Qt::white));
    textLabel->setPadding(QMargins(2,2,2,2));

    //plot->yAxis->setLabel("Voltage, kV");
    plot->yAxis->setRange(0,256);
    QSharedPointer<QCPAxisTickerFixed> Yticker(new QCPAxisTickerFixed);
    plot->yAxis->setTicker(Yticker);
    Yticker->setTickStep(32);
    Yticker->setScaleStrategy(QCPAxisTickerFixed::ssNone); // and no scaling
    Yticker->setTickCount(8);


    //Xticker->setTickCount(8);




}

void setupPulsesGraph(QCustomPlot *plot)
{
    QPen pen(Qt::black);
    QBrush brush(Qt::blue);
    for(int i=0;i<8;i++) {
        QCPBars *B = new QCPBars(plot->yAxis, plot->xAxis);
        B->setBaseValue(0);
        B->addData(i+1,2*i+1);
        B->setPen(pen);
        B->setBrush(brush);
    }

    plot->xAxis->setRange(QCPRange(-1,20));
    plot->xAxis->setLabel("Time, ms");

    plot->yAxis->setRange(QCPRange(0,9));
    plot->yAxis->setRangeReversed(true);
    plot->yAxis->setSubTicks(false);
    QSharedPointer<QCPAxisTickerText> textTicker(new QCPAxisTickerText);
    plot->yAxis->setTicker(textTicker);
    QMap<double, QString> ticks;
    ticks[1] = "1";
    ticks[2] = "2";
    ticks[3] = "3";
    ticks[4] = "4";
    ticks[5] = "5";
    ticks[6] = "6";
    ticks[7] = "7";
    ticks[8] = "8";
    textTicker->setTicks(ticks);

    plot->setInteraction(QCP::iRangeDrag, true);
    plot->setInteraction(QCP::iRangeZoom, true);
    plot->axisRect()->setRangeDrag(Qt::Horizontal);
    plot->axisRect()->setRangeZoom(Qt::Horizontal);

}
