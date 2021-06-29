#include "ledindicator.h"
#include <QPainter>
#include <QGradient>
#include <QPaintDevice>

LEDindicator::LEDindicator(QWidget *parent) :
    QWidget(parent)
{
}

bool LEDindicator::state() const
{
    return _state;
}

void LEDindicator::setState(bool state)
{
    _state = state;
    repaint();
}

QColor LEDindicator::colorOn() const
{
    return _colorOn;
}

void LEDindicator::setColorOn(const QColor color)
{
    _colorOn = color;
    repaint();
}

QColor LEDindicator::colorOff() const
{
    return _colorOff;
}

void LEDindicator::setColorOff(const QColor color)
{
    _colorOff = color;
    repaint();
}

int LEDindicator::diameter() const
{
    return _diameter;
}

void LEDindicator::setDiameter(const int diameter)
{
    _diameter = diameter;
    repaint();
}

void LEDindicator::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter p(this);

//    QRect geo = geometry();
//    int width = geo.width();
//    int height = geo.height();

    int x=2, y=2;

    QRadialGradient g(x+_diameter/2-4, y+_diameter/2-4, _diameter*0.4, _diameter*0.4, _diameter*0.4);

    g.setColorAt(0, Qt::white);
    if ( _state )
        g.setColorAt(1, _colorOn);
    else
        g.setColorAt(1, _colorOff);
    QPen pen;
    pen.setBrush(QBrush(QColor(100,100,100)));
    pen.setWidth(2);


    //p.setPen(_color);
    p.setPen(pen);
    p.setRenderHint(QPainter::Antialiasing, true);
    if(isEnabled())
        p.setBrush(QBrush(g));
    else
        p.setBrush(QBrush(Qt::lightGray));
    p.drawEllipse(x, y, _diameter-4, _diameter-4);

}


