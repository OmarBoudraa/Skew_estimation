#ifndef SKEW_ESTIMATION_H
#define SKEW_ESTIMATION_H

#include <QMessageBox>
#include <QMainWindow>
#ifndef QT_NO_PRINTER
#include <QPrinter>
#endif

#include <iostream>
#include <windows.h>
#include <QMainWindow>
#include <QtGui>
#include <QProgressDialog>
#include <stdio.h>
#include <math.h>
#include <QPainterPath>
#include <QApplication>
#include <QPainterPath>
#include <QMainWindow>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <opencv/cxcore.h>
#include <opencv/cvaux.h>
#include <opencv/cvwimage.h>
#include <opencv/cxmisc.h>
#include <opencv/ml.h>
#include <QDockWidget>
#include <QFile>
#include <QFileDialog>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
//#include "opencv2/contrib/contrib.hpp"

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/core/core.hpp"

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

#include <opencv2/core/utility.hpp>
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include <opencv2/features2d.hpp>

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include "statistique.h"
#include "pretraitement.h"
#include "inclinaison.h"
#include "houghtransformdialog.h"
using namespace cv;
using namespace std;
QT_BEGIN_NAMESPACE
class QAction;
class QLabel;
class QMenu;
class QScrollArea;
class QString;
class QPrinter;
class QImage;
class QScrollBar;
class QPixmap;
QT_END_NAMESPACE

namespace Ui {
class Skew_estimation;
}

class Skew_estimation : public QMainWindow
{
    Q_OBJECT

public:
    explicit Skew_estimation(QWidget *parent = 0);
    ~Skew_estimation();

private slots:
    void on_actionOuvrir_triggered();

    void on_actionFermer_triggered();

    void on_actionSauvegarder_triggered();

    void on_actionImprimer_triggered();

    void on_actionA_propos_de_QT_triggered();

    void on_actionA_propos_de_DCI_triggered();

    void on_actionZoomer_25_triggered();

    void on_actionD_zoomer_25_triggered();

    void on_actionTaille_normale_triggered();

    void on_actionAjuster_la_fen_tre_triggered();

    void on_actionGris_triggered();

    void on_actionInverser_triggered();

    void on_actionOtsu_triggered();

    void on_actionHybride_triggered();

    void on_actionSquelettisation_triggered();

    void on_actionSupprimer_les_pixels_isol_s_triggered();

    void on_actionTransform_de_Hough_triggered();

    void on_actionAIO_Otsu_triggered();

    void on_actionAIO_Hybride_triggered();

    void on_actionCorrection_automatique_triggered();

    void on_actionProfile_de_projection_triggered();

    void on_actionProfile_de_projection_Hybride_triggered();

public:
    Ui::Skew_estimation *ui;
    void updateActions();
   void versLabel(IplImage* src);
    void scaleImage(double factor);
    void adjustScrollBar(QScrollBar *scrollBar, double factor);
#ifndef QT_NO_PRINTER
    QPrinter printer;
#endif
    double scaleFactor;
    QString fileName;
    IplImage *image_source=0;
    IplImage *image_gris=0;
    IplImage *image_bin=0;
    IplImage *image_sql=0;
    IplImage *image_hough=0;
    IplImage *image_corrected=0;
    double skew_angle=0;

};

#endif // SKEW_ESTIMATION_H
