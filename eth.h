#ifndef ETH_H
#define ETH_H

#include <stdint.h>
#include <qobject.h>
#include <QTcpSocket>
#include <QHostAddress>
#include <QTimer>
#include <QDateTime>
#include <QThread>
#include <QMutex>

#define _millis (QDateTime::currentDateTime().currentMSecsSinceEpoch())

typedef struct
{
    quint8 ID;
    quint16 arg;
} TCommand;

class CethThread : public QThread
{
    Q_OBJECT

public:
    CethThread(QObject *parent = nullptr);
    ~CethThread();
    void addCommand(quint8 C, quint16 arg);
    void setAddr(QString addr);
    void setPort(quint16 port);

    void getMessage(quint8 &C, QVector<quint8> &data);
    //void startThread();
protected:
    void run() override;
private:
    QMutex mutex;
    bool restart = false;
    bool abort = false;
    bool waiting = false;
    quint16 _port;
    QString _addr;
    quint64 _timeout;
    QList<TCommand> commands;
    quint8 ansC;
    QVector<quint8> ansD;

signals:
    void message();
    void timeout();
    void connected();
};

#endif // ETH_H
