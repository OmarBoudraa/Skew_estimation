#include "skew_estimation.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Skew_estimation w;
    w.show();

    return a.exec();
}
