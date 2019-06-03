#ifndef HOUGHTRANSFORMDIALOG_H
#define HOUGHTRANSFORMDIALOG_H

#include <QDialog>

namespace Ui {
class HoughTransformDialog;
}

class HoughTransformDialog : public QDialog
{
    Q_OBJECT

public:
    explicit HoughTransformDialog(QWidget *parent = 0);
    ~HoughTransformDialog();
public:
    int thr=50;
     double minline=200;
     double maxgap=1000;
     bool rej=false;
private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

    void on_HoughTransformDialog_rejected();

private:
    Ui::HoughTransformDialog *ui;
};

#endif // HOUGHTRANSFORMDIALOG_H
