#ifndef LEDINDICATOR_H
#define LEDINDICATOR_H
#include <QtUiPlugin/QDesignerExportWidget>
#include <QWidget>


class QDESIGNER_WIDGET_EXPORT LEDindicator : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(bool state READ state WRITE setState)
    Q_PROPERTY(QColor colorOn READ colorOn WRITE setColorOn)
    Q_PROPERTY(QColor colorOff READ colorOff WRITE setColorOff)
    Q_PROPERTY(int diameter READ diameter WRITE setDiameter)

public:
    LEDindicator(QWidget * parent = 0);

    bool state() const;
    void setState(bool state);

    QColor colorOn() const;
    void setColorOn(const QColor color);

    QColor colorOff() const;
    void setColorOff(const QColor color);

    int diameter() const;
    void setDiameter(const int diameter);

private:
    bool _state = false;
    int _diameter = 10;
    QColor _colorOn = Qt::green;
    QColor _colorOff = Qt::black;
    //QRadialGradient _gradient;

protected:
    void paintEvent(QPaintEvent* event) override;
};

#endif // LEDINDICATOR_H
