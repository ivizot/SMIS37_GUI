#include "eth.h"
#include "/home/ivizot/science/Projects/proteus/projects/SMIS37/Main/FIRMWARE/SMIS37/include/headers.h"

CethThread::CethThread(QObject *parent)
{
    _timeout = 1000;

}

CethThread::~CethThread()
{
    mutex.lock();
    abort = true;
    mutex.unlock();
    wait();
}

void CethThread::addCommand(quint8 C, quint16 arg)
{
    mutex.lock();
    commands.append(TCommand{C,arg});
    //qDebug() << "command added: " <<C;
    mutex.unlock();
}

void CethThread::setAddr(QString addr)
{
    _addr = addr;
    if (isRunning())
    {
        mutex.lock();
        abort = true;
        mutex.unlock();
        wait();
        run();
    }
}

void CethThread::setPort(quint16 port)
{
    _port = port;
    if (isRunning())
    {
        mutex.lock();
        abort = true;
        mutex.unlock();
        wait();
        run();
    }
}

void CethThread::getMessage(quint8 &C, QVector<quint8> &data)
{
    mutex.lock();
    C = ansC;
    data = ansD;
    mutex.unlock();
}

void CethThread::run()
{
    QTcpSocket* socket = new QTcpSocket;
    QVector<quint8> RX;

    quint8 L = 0;

    forever
    {
        if(abort) {
            qDebug()<<"aborting now";
            socket->abort();
            delete socket;
            return;
        }

        quint64 curTime = QDateTime::currentMSecsSinceEpoch();
        static quint64 sentTime;
        //static quint8 timeoutCount = 0;

        switch (L) {
        case 0: { // connect
            mutex.lock();
            // get params
            const QString addr = this->_addr;
            const quint16 port = this->_port;
            mutex.unlock();
            socket->abort();
            socket->connectToHost(addr,port);
            qDebug()<<"Connecting to "<<addr<<":"<<port;
            if(socket->waitForConnected(_timeout)) {
                L = 1;
                qDebug()<<"Connected";
                emit connected();
            }
            else {
                qDebug()<<"Timeout";
                commands.clear();
                emit timeout();
            }
            break;
        }

        case 1: { // send message or request
            if(curTime-sentTime<50)
                break;
            mutex.lock();
            if(commands.isEmpty()) { // if no command to send, send query
                commands.append({250,0});
            }
            else {
                //qDebug()<<"command: "<< commands.first().ID;
            }
            TCommand C = commands.first();
            mutex.unlock();
            quint8 ba[3];
            ba[0] = C.ID;
            ba[1] = (quint8)(C.arg >> 8);
            ba[2] = (quint8)(C.arg & 0xff);

            RX.clear();
            socket->write((const char*)ba,3);
            //qDebug()<<"sent"<<ba[0]<<ba[1]<<ba[2];
            if(!socket->waitForBytesWritten(100)) {
                L = 0;
                break;
            }

            sentTime = curTime;
            //waiting = true;
            L = 2;
            break;
        }

        case 2: { //wait for data
            if(!socket->waitForReadyRead(_timeout))
                L = 0;
            else
                L = 3;
            break;
        }

        case 3: { //read input and check for message
            if(socket->bytesAvailable())
            {
                QByteArray A = socket->readAll();
                for(int i=0;i<A.size();i++)
                    RX.append((quint8)A[i]);
            }
            //check input for message
            if(RX.size()>0) {
                if((RX.size() == 3) || \
                        (RX[0] == COMM_DATA_SLOW && RX.size() == sizeof(TData)+1) || \
                        (RX[0] == COMM_DATA_ALL && RX.size() == sizeof(TData)+sizeof(TDataFast)+1) ) {
                    ansC = RX[0];
                    RX.removeFirst();
                    ansD = RX;
                    emit message();
                    //qDebug()<<"message";
                    mutex.lock();
                    if(ansC == commands.first().ID ||\
                            (commands.first().ID == COMM_QUERY && \
                             (ansC == COMM_DATA_SLOW || ansC == COMM_DATA_ALL) ) ) {
                        commands.removeFirst();
                    }
                    mutex.unlock();
                    //waiting = false;
                    //RX.clear();
                    L = 1;
                }
                else
                    L = 2;
            }

        }


        }
    }
}
