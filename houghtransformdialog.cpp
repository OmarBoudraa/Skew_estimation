#include "houghtransformdialog.h"
#include "ui_houghtransformdialog.h"

HoughTransformDialog::HoughTransformDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::HoughTransformDialog)
{
    ui->setupUi(this);
}

HoughTransformDialog::~HoughTransformDialog()
{
    delete ui;

}

void HoughTransformDialog::on_buttonBox_accepted()
{
    this->thr=ui->spinBox->value();
        this->minline=ui->doubleSpinBox->value();
    this->maxgap=ui->doubleSpinBox_2->value();
        this->hide();
}

void HoughTransformDialog::on_buttonBox_rejected()
{rej=true;
this->hide();
}

void HoughTransformDialog::on_HoughTransformDialog_rejected()
{
    rej=true;
    this->hide();
}
