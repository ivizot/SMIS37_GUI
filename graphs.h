#ifndef GRAPHS_H
#define GRAPHS_H
#include "qcustomplot.h"
#include "/home/ivizot/science/Projects/proteus/projects/SMIS37/Main/FIRMWARE/SMIS37/include/headers.h"

const QColor colors[12] = {\
    QColor(235,172,35),\
    QColor(184,0,88),\
    QColor(0,140,249),\
    QColor(0,110,0),\
    QColor(0,187,173),\
    QColor(209,99,230),\
    QColor(178,69,2),\
    QColor(255,146,135),\
    QColor(89,84,214),\
    QColor(0,198,248),\
    QColor(135,133,0),\
    QColor(0,167,108)};

typedef struct {
    size_t ind;
    QString name;
} TPlotName;

#define TplotsN 9
const TPlotName Tplots[TplotsN] = {
    {T_GCOLLECTOR,"Collector"},
    {T_GCAVITY,"Cavity"},
    {T_COIL1,"Coil 1"},
    {T_COIL2,"Coil 2"},
    {T_GHVPS,"GHVPS"},
    {T_MFPS,"MFPS"},
    {T_ANALYZER,"Analyzer"},
    {T_EXT,"Extraction"},
    {T_INPUT,"Input"}
};

#define FplotsN 5
const TPlotName Fplots[FplotsN] = {
    {F_GCOLLECTOR,"Collector"},
    {F_GCAVITY,"Cavity"},
    {F_COIL1,"Coil 1"},
    {F_GHVPS,"GHVPS"},
    {F_MFPS,"MFPS"},
};

#define PplotsN 1
const TPlotName Pplots[PplotsN] = {
    {P_HIGH,"Phigh"}
};

//temp graphs


void setupGraph(QCustomPlot * plot);
void setupTempGraph(QCustomPlot * plot);
void setupFlowGraph(QCustomPlot * plot);
void setupMiscGraph(QCustomPlot * plot);
void setupGyrGraph(QCustomPlot * plot);

void setupPulsesGraph(QCustomPlot * plot);

#endif // GRAPHS_H
