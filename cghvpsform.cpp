#include "cghvpsform.h"
#include "ui_cghvpsform.h"

CGHVPSform::CGHVPSform(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CGHVPSform)
{
    ui->setupUi(this);
}

CGHVPSform::~CGHVPSform()
{
    delete ui;
}

void CGHVPSform::setData(const TGHVPS D)
{
    this->data = D;
}

void CGHVPSform::on_spinBox_editingFinished()
{

}

