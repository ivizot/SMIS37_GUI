#ifndef MATROUTINE_H
#define MATROUTINE_H

#include <QDateTime>
#include <QDataStream>
#include <QFile>
#include <QDebug>
#include <QtMath>
#include "../Main/FIRMWARE/SMIS37/include/headers.h"

#define _mxCHAR_CLASS_CLASS_arrayflags 0x00000004
#define _mxDOUBLE_CLASS_arrayflags 0x00000006
#define _mxSINGLE_CLASS_arrayflags 0x00000007
#define _mxINT8_CLASS_arrayflags 0x00000008
#define _mxUINT8_CLASS_arrayflags 0x00000009
#define _mxINT16_CLASS_arrayflags 0x0000000A
#define _mxUINT16_CLASS_arrayflags 0x0000000B
#define _mxINT32_CLASS_arrayflags 0x0000000C
#define _mxUINT32_CLASS_arrayflags 0x0000000D
#define _mxINT64_CLASS_arrayflags 0x0000000E
#define _mxUINT64_CLASS_arrayflags 0x0000000F
#define _mxUINT8_LOGICAL_CLASS_arrayflags (_mxUINT8_CLASS_arrayflags+(0x0002<<8))


#define _miINT8 1
#define _miUINT8 2
#define _miINT16 3
#define _miUINT16 4
#define _miINT32 5
#define _miUINT32 6
#define _miSINGLE 7
#define _miDOUBLE 9
#define _miINT64 12
#define _miUINT64 13
#define _miMATRIX 14
#define _miCOMPRESSED 15
#define _miUTF8 16
#define _miUTF16 17
#define _miUTF32 18

void initLog();
void endLog();
void logData(const TData D);
void logDataFast(const TData D, const TDataFast DF);

void writeMatHeader(QDataStream& out);


#endif // MATROUTINE_H
