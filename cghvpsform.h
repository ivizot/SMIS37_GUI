#ifndef CGHVPSFORM_H
#define CGHVPSFORM_H

#include <QDialog>
#include "../Main/FIRMWARE/SMIS37/include/headers.h"

namespace Ui {
class CGHVPSform;
}

class CGHVPSform : public QDialog
{
    Q_OBJECT

public:
    explicit CGHVPSform(QWidget *parent = nullptr);
    ~CGHVPSform();

    void setData(const TGHVPS D);

private:
    Ui::CGHVPSform *ui;

    TGHVPS data;

signals:
    void sendCommand(quint8 C, quint16 arg);
    void requestDialog();
private slots:
    void on_pushButton_clicked();
};

#endif // CGHVPSFORM_H
