#include "matRoutine.h"
const int matNF = 3;
const char* matFieldsData[] = {
    "timestamp",
    "F1",
    "F2"
};


QFile logFile;
QDataStream logStream;


//write uint16 arr to mat file
void writeUINT16arr(QDataStream &out, QVector<quint16> data, QString varname)
{
    QByteArray bufV;
    QDataStream buf(&bufV, QIODevice::WriteOnly);

    // MATRIX tag
    buf<<(quint32)_miMATRIX;
    // total size - will recalculate later
    buf<<(quint32)0;

    // array flags
    buf<<(quint32)_miUINT32; // data type
    buf<<(quint32)8; // number of bytes = 8
    buf<<(quint16)0; // undef
    buf<<(quint8)0; // flags
    buf<<(quint8)_mxUINT16_CLASS_arrayflags; //class
    buf<<(quint32)0; // undef

    // dimensions array
    buf<<(quint32)_miINT32;
    buf<<(quint32)8;
    buf<<(qint32)1; // rows count
    buf<<(qint32)data.size(); // cols count

    //array name
    buf<<(quint32)_miINT8;
    buf<<(quint32)varname.length();
    int sn = 8*qCeil(varname.length()/8.0); // size of name padded to N*8 bytes
    for(int i=0;i<varname.length();i++)
        buf<<(qint8)varname.toStdString()[i];
    for(int i=varname.length();i<sn;i++)
        buf<<(qint8)0;

    //data
    buf<<(quint32)_miUINT16;
    buf<<(quint32)(data.size()*sizeof(quint16));
    int sd = 4*qCeil(data.size()*sizeof(quint16)/8.0);
    for(int i=0;i<data.size();i++)
        buf<<(quint16)data[i];
    for(int i=data.size();i<sd;i++)
        buf<<(quint16)0;

    //qDebug()<<bufV.size();
    buf.device()->seek(4); // go back to position where data size should be
    buf<<(quint32)(bufV.size()-8);

    //write to file
    for(int i=0;i<bufV.size();i++)
        out<<(quint8)bufV.at(i);
}

void composeData(QVector<quint16> &arr, const TData D)
{
    // water data
    for (int i=0;i<TScount;i++)
        arr.append(D.waterData.T[i]);
    for (int i=0;i<FScount;i++)
        arr.append(D.waterData.F[i]);
    for (int i=0;i<PScount;i++)
        arr.append(D.waterData.P[i]);
    arr.append((quint16)D.waterData.state);

    // pulsegen data
    for(int i=0;i<8;i++)
        arr.append(D.pulseGenData.T[i]);
    for(int i=0;i<8;i++)
        arr.append(D.pulseGenData.D[i]);
    arr.append(D.pulseGenData.P);
    arr.append((quint16)D.pulseGenData.S);
    arr.append((quint16)(D.pulseGenData.N >> 16)); //N high word
    arr.append((quint16)(D.pulseGenData.N & 0xFFFF)); //N low word
    arr.append((quint16)D.pulseGenData.TIMER);
    arr.append((quint16)D.pulseGenData.dT);

    // GSPS data
    arr.append((quint16)D.GSPSData.I);
    arr.append((quint16)D.GSPSData.Iset);
    arr.append((quint16)D.GSPSData.U);
    arr.append((quint16)D.GSPSData.RS);
    arr.append((quint16)D.GSPSData.state);

    //GIP data
    arr.append((quint16)D.GIPData.U);
    arr.append((quint16)D.GIPData.I);
    arr.append((quint16)D.GIPData.latchedInterlock);
    arr.append((quint16)D.GIPData.S);

    //GFPS data
    arr.append((quint16)D.GFPSData.state);
    arr.append((quint16)D.GFPSData.Pset);
    arr.append((quint16)D.GFPSData.Pint);
    arr.append((quint16)D.GFPSData.Pcur);
    arr.append((quint16)D.GFPSData.Ramp);
    arr.append((quint16)D.GFPSData.Irms);

    // GHVPS data
    for(int i=0;i<12;i++)
        arr.append((quint16)D.GHVPSData.ZU[i]);
    for(int i=0;i<3;i++)
        arr.append((quint16)D.GHVPSData.BU[i]);
    arr.append((quint16)D.GHVPSData.online);

    //spellman
    arr.append((quint16)D.spellmanData.UC_URset);
    arr.append((quint16)D.spellmanData.UC_IRset);
    arr.append((quint16)D.spellmanData.Uset);
    arr.append((quint16)D.spellmanData.Iset);
    arr.append((quint16)D.spellmanData.Usetreq);
    arr.append((quint16)D.spellmanData.Isetreq);
    arr.append((quint16)D.spellmanData.OVP);
    arr.append((quint16)D.spellmanData.PR);
    arr.append((quint16)D.spellmanData.PM);
    arr.append((quint16)D.spellmanData.CBT);
    arr.append((quint16)D.spellmanData.S1);
    arr.append((quint16)D.spellmanData.S2);
    arr.append((quint16)D.spellmanData.S3);
    arr.append((quint16)D.spellmanData.UC_URreq);
    arr.append((quint16)D.spellmanData.UC_IRreq);
    arr.append((quint16)D.spellmanData.FSU);
    arr.append((quint16)D.spellmanData.FSI);
    arr.append((quint16)D.spellmanData.UM);
    arr.append((quint16)D.spellmanData.IM);
    arr.append((quint16)D.spellmanData.SF);
    arr.append((quint16)D.spellmanData.request_local_remote_mode);

    // general
    arr.append((quint16)D.GUlast);
    arr.append((quint16)D.GUdesired);
    arr.append((quint16)D.GIlast);
    arr.append((quint16)D.GIdesired);
    arr.append((quint16)D.MIlast);
    arr.append((quint16)D.interlocksIn);
    arr.append((quint16)D.latchedInterlocksIn);
    arr.append((quint16)D.interlocksOut);
    for(int i=0;i<8;i++)
        arr.append((quint16)D.latchedInterlocksOut[i]);
    arr.append((quint16)D.waterTemp);
    arr.append((quint16)D.waterFlow);
    arr.append((quint16)D.waterPressure);
}

void composeDataFast(QVector<quint16> &arr, const TDataFast D)
{
    arr.append((quint16)FAST_ADC_COUNT);
    arr.append((quint16)(FAST_ADC_TPP*100));
    for(int i=0;i<FAST_ADC_COUNT;i++)
        arr.append((quint16)D.GI[i]);
    for(int i=0;i<FAST_ADC_COUNT;i++)
        arr.append((quint16)D.GU[i]);
    for(int i=0;i<FAST_ADC_COUNT;i++)
        arr.append((quint16)D.MI[i]);
}


void logData(const TData D)
{
    QVector<quint16> arr;
    arr.append((quint16)0x0000); // data type flag
    composeData(arr,D);
    writeUINT16arr(logStream,arr,"d"+QString::number(QDateTime::currentMSecsSinceEpoch()));
}

void logDataFast(const TData D, const TDataFast DF)
{
    QVector<quint16> arr;
    arr.append((quint16)0x0001); // data type flag
    composeData(arr,D);
    composeDataFast(arr,DF);
    writeUINT16arr(logStream,arr,"d"+QString::number(QDateTime::currentMSecsSinceEpoch()));
}

void initLog()
{
    QString fname = ""+
            QDateTime::currentDateTime().toString("yyyy_MM_dd_hh_mm_ss")+
            ".mat";
    fname = "log.mat";
    logFile.setFileName(fname);
    qDebug()<<fname;
    logFile.open(QIODevice::ReadWrite);
    logStream.setDevice(&logFile);
    writeMatHeader(logStream);
    TData D;

    //logData(D);
}

void endLog()
{
    logFile.close();
}

void writeMatHeader(QDataStream &out)
{
    char header[116];
    for(int i=0;i<116;i++)
        header[i] = '\0';
    QString text = "MATLAB 5.0 MAT-file; created in SMIS37-GUI on " +
            QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss");
    strcpy(header,text.toStdString().c_str());
    out.writeRawData(header,116);

    // write "Header Subsystem Data Offset Field" (not used, i.e. write 00000000)
    out<<(quint32)0x00000000; //write_U32
    out<<(quint32)0x00000000;//write_U32

    // write "Header Flag Fields"
    out<<(quint16)0x0100; // version //write_U16
    out<<(qint8)'M'; // endian //write8
    out<<(qint8)'I'; // endian

    //test var


    //out << header;
}
