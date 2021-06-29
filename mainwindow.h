#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QTimer>
#include <QDateTime>
#include <QInputDialog>
#include "headers.h"
#include "graphs.h"
#include "eth.h"
#include "setupGUI.h"
#include "matRoutine.h"
#include "cghvpsform.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onEthTimeout();
    void onEthConnect();
    void onEthMessage();

    void on_B_resetInterlocks_clicked();

    void on_B_GSPSsetCurrent_clicked();
    void onInputDialog();


    void PG_onClicked();
    void PG_offClicked();
    void PG_TvalueChanged();
    void PG_DvalueChanged();
    void on_B_PGsingle_clicked();
    void on_B_PGstart_clicked();
    void on_B_PGstop_clicked();
    void I_PGperiod_editingFinished();
    void plotPulses_mouseDoubleClick(QMouseEvent *event);    

    void on_GHVPSdata_ZU5_editingFinished();

public slots:
    //void receiveCommand(quint8 C, quint16 arg);

private:
    Ui::MainWindow *ui;
    QTcpSocket  _socket;
    bool TCPwaiting = false; // we are waiting for answer
    //uint16_t TCPwaiting = 0; // how many bytes are we waiting
    QVector<uint8_t> TCPrx; //input buffer

    //uint8_t TCPcommand = 0;
    uint16_t TCParg = 0;
    qint64 TCPts,TCPlastQuery,TCPconnectTS;

    TData data;
    TDataFast dataFast;

    //QList<TCommand> commands;

    QInputDialog inputDialog;

    CethThread ethThread;


    qint64 graphTS; // timestamp for graphs

    CGHVPSform* GHVPSform;


    void updateData();
    void updateFastData();

    void showInputDialog(uint8_t ID);
    void setEnabled(bool v);


};
#endif // MAINWINDOW_H

