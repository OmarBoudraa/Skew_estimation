#include "skew_estimation.h"
#include "ui_skew_estimation.h"
#include <QtWidgets>
#ifndef QT_NO_PRINTER
#include <QPrintDialog>
#endif
Skew_estimation::Skew_estimation(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Skew_estimation)
{
    ui->setupUi(this);
        ui->imageLabel = new QLabel;
        ui->imageLabel->setBackgroundRole(QPalette::Base);
        ui->imageLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
       ui->imageLabel->setScaledContents(true);

        ui->scrollArea = new QScrollArea;
        ui->scrollArea->setBackgroundRole(QPalette::Dark);
        ui->scrollArea->setWidget(ui->imageLabel);
        setCentralWidget(ui->scrollArea);
        setWindowTitle(tr("Détection et correction d'inclainison"));
        resize(500, 400);
}

Skew_estimation::~Skew_estimation()
{
    delete ui;
}

void Skew_estimation::on_actionOuvrir_triggered()
{



    fileName = QFileDialog::getOpenFileName(this,tr("Ouvrir une image"), QDir::currentPath(),tr("Images (*.png *.tiff *.bmp *.jpg *.dib *.jpeg *.jpe *.pbm *.pgm *.sr *.ras *.tiff *.tif)"));

    if (!fileName.isEmpty()) {

        image_source=cvLoadImage(fileName.toUtf8(), CV_LOAD_IMAGE_COLOR);   // Read the file

        QImage image(fileName);
       if (image.isNull()&&!image_source) {
            QMessageBox::information(this, tr("Détection et correction d'inclainison"),
                                     tr("On ne peut pas charger %1.").arg(fileName));
           return;
        }
        if (image.isNull()) {
image=TadIplImage2QImage(image_source);
       }
       if (!image_source) {
image_source=TadQImage2IplImage(&image);
       }

//! [2] //! [3]
       ui->imageLabel->setPixmap(QPixmap::fromImage(image));
       scaleFactor = 1.0;
       ui->actionSauvegarder->setEnabled(true);
       ui->actionImprimer->setEnabled(true);
       ui->actionAjuster_la_fen_tre->setEnabled(true);
ui->actionGris->setEnabled(true);
ui->actionAIO_Otsu->setEnabled(true);
ui->actionAIO_Hybride->setEnabled(true);
ui->actionProfile_de_projection->setEnabled(true);
ui->actionProfile_de_projection_Hybride->setEnabled(true);

       updateActions();

       if (!ui->actionAjuster_la_fen_tre->isChecked())
           ui->imageLabel->adjustSize();

    }
}

void Skew_estimation::on_actionFermer_triggered()
{
exit(0);
}

void Skew_estimation::on_actionSauvegarder_triggered()
{
    QFileDialog dialogsave;QString fileSave=NULL;
         fileSave =dialogsave.getSaveFileName(this,tr("Sauvegarder une image"),QDir::currentPath());
        QFileInfo info(fileName);
               QString extension =info.suffix();
               //fileSave+=info.fileName();
               if(fileSave.isNull()){QMessageBox::information(this, tr("Détection et correction d'inclainison"),
                                                              tr("Image non sauvegardée.")); return;  }
               fileSave+="."+extension;

               bool Save=false;
                   Save=ui->imageLabel->pixmap()->save(fileSave);

        (!Save)? QMessageBox::information(this, tr("Détection et correction d'inclainison"),
                                         tr("On peut pas sauvegarder %1.").arg(fileSave)):QMessageBox::information(this, tr("Détection et correction d'inclainison"),
                                                                                                      tr("Image Sauvegardée %1.").arg(fileSave));

}

void Skew_estimation::on_actionImprimer_triggered()
{
    Q_ASSERT(ui->imageLabel->pixmap());
#if !defined(QT_NO_PRINTER) && !defined(QT_NO_PRINTDIALOG)
//! [6] //! [7]
    QPrintDialog dialog(&printer, this);
//! [7] //! [8]
    if (dialog.exec()) {
        QPainter painter(&printer);
        QRect rect = painter.viewport();
        QSize size = ui->imageLabel->pixmap()->size();
        size.scale(rect.size(), Qt::KeepAspectRatio);
        painter.setViewport(rect.x(), rect.y(), size.width(), size.height());
        painter.setWindow(ui->imageLabel->pixmap()->rect());
        painter.drawPixmap(0, 0, *ui->imageLabel->pixmap());
    }
#endif
}
void Skew_estimation::updateActions()
//! [21] //! [22]
{
   ui->actionZoomer_25->setEnabled(!ui->actionAjuster_la_fen_tre->isChecked());
    ui->actionD_zoomer_25->setEnabled(!ui->actionAjuster_la_fen_tre->isChecked());
    ui->actionTaille_normale->setEnabled(!ui->actionAjuster_la_fen_tre->isChecked());
}
void Skew_estimation::on_actionA_propos_de_QT_triggered()
{
 qApp->aboutQt();
}

void Skew_estimation::on_actionA_propos_de_DCI_triggered()
{
    QMessageBox::about(this, tr("A propos de Détection et correction de l'inclainison"),
            tr("<p>La <b> détection et la correction de l'inclainison </b> est un problème central en traitement d’images. Cela consiste à trouver l'angle d'inclainison d’une image puis procéder à une rotation par angle inverse de l'angle trouvée. Dans cette application, nous avons essayé de tirer profit de la squelettisation et la transformée probabilistique de Hough dans le traitement de ce problème. </p>"
));
}

void Skew_estimation::on_actionZoomer_25_triggered()
{
    scaleImage(1.25);
 }
 void Skew_estimation::scaleImage(double factor)
 //! [23] //! [24]
 {
     Q_ASSERT(ui->imageLabel->pixmap());
     scaleFactor *= factor;
     ui->imageLabel->resize(scaleFactor * ui->imageLabel->pixmap()->size());

     adjustScrollBar(ui->scrollArea->horizontalScrollBar(), factor);
     adjustScrollBar(ui->scrollArea->verticalScrollBar(), factor);

     ui->actionZoomer_25->setEnabled(scaleFactor < 3.0);
     ui->actionD_zoomer_25->setEnabled(scaleFactor > 0.333);
 }
 void Skew_estimation::adjustScrollBar(QScrollBar *scrollBar, double factor)
 //! [25] //! [26]
 {
     scrollBar->setValue(int(factor * scrollBar->value()
                             + ((factor - 1) * scrollBar->pageStep()/2)));
 }


void Skew_estimation::on_actionD_zoomer_25_triggered()
{
scaleImage(0.8);
}

void Skew_estimation::on_actionTaille_normale_triggered()
{
    ui->imageLabel->adjustSize();
    scaleFactor = 1.0;
}

void Skew_estimation::on_actionAjuster_la_fen_tre_triggered()
{
    bool fitToWindow = ui->actionAjuster_la_fen_tre->isChecked();
    ui->scrollArea->setWidgetResizable(fitToWindow);
    if (!fitToWindow) {
       on_actionTaille_normale_triggered();
    }
    updateActions();
}

void Skew_estimation::on_actionGris_triggered()
{
    image_gris=TadRGB2GRIS(image_source);
    versLabel(image_gris);
    ui->actionOtsu->setEnabled(true);
    ui->actionHybride->setEnabled(true);
}
void Skew_estimation::versLabel(IplImage *src)
{QImage image=TadIplImage2QImage(src);
    ui->imageLabel->setPixmap(QPixmap::fromImage(image));
ui->imageLabel->adjustSize();}

void Skew_estimation::on_actionInverser_triggered()
{
    image_bin=TadInvImgGRIS(image_bin);
    versLabel(image_bin);
}

void Skew_estimation::on_actionOtsu_triggered()
{
image_bin=TadOtsu(image_gris);
versLabel(image_bin);
ui->actionInverser->setEnabled(true);
ui->actionSquelettisation->setEnabled(true);
ui->actionSupprimer_les_pixels_isol_s->setEnabled(true);

}

void Skew_estimation::on_actionHybride_triggered()
{
    image_bin=hybridbinarization(image_gris);
    versLabel(image_bin);
    ui->actionInverser->setEnabled(true);
    ui->actionSquelettisation->setEnabled(true);
    ui->actionSupprimer_les_pixels_isol_s->setEnabled(true);


}

void Skew_estimation::on_actionSquelettisation_triggered()
{
image_sql=TadSquelettisation(image_bin);
versLabel(image_sql);
ui->actionTransform_de_Hough->setEnabled(true);
}

void Skew_estimation::on_actionSupprimer_les_pixels_isol_s_triggered()
{
    image_sql=TadIsolatedpixels(image_sql);
    versLabel(image_sql);
    ui->actionTransform_de_Hough->setEnabled(true);
}

void Skew_estimation::on_actionTransform_de_Hough_triggered()
{/*HoughTransformDialog *ht= new HoughTransformDialog(this);
    ht->exec();
if(ht->rej) return;*/
    //image_hough=TadTransfHough(image_sql,ht->thr,ht->minline,ht->maxgap);
  //  skew_angle=TadSkewAngle(image_sql,ht->thr,ht->minline,ht->maxgap);
    image_hough=TadTransfHough(image_sql,&skew_angle,image_sql->width/16,image_sql->width/4,image_sql->width/8);
    versLabel(image_hough);
   // qDebug()<<skew_angle;
    ui->actionCorrection_automatique->setEnabled(true);
    //QMessageBox::information(this, tr("Skew angle estimation using Hough transform method"),tr("Best angle = %1.").arg(skew_angle));
}

void Skew_estimation::on_actionAIO_Otsu_triggered()
{
    // Variables
      ULARGE_INTEGER tbegin,tend;
      FILETIME ttmp={0,0}; // temporary variable
      double texec=0.;

      // Start timer
      ::GetSystemTimeAsFileTime(&ttmp); // store current time in ttmp structure
      tbegin.HighPart=ttmp.dwHighDateTime; // convert ttmp to two int32
      tbegin.LowPart=ttmp.dwLowDateTime;

      /* YOUR PROGRAM HERE */
      /* YOUR PROGRAM HERE */
      /* YOUR PROGRAM HERE */
      this->on_actionGris_triggered();
          this->on_actionOtsu_triggered();
          this->on_actionInverser_triggered();
          this->on_actionSquelettisation_triggered();
          this->on_actionSupprimer_les_pixels_isol_s_triggered();
          this->on_actionTransform_de_Hough_triggered();
      // End timer
      ::GetSystemTimeAsFileTime(&ttmp); // store current time in ttmp structure
      tend.HighPart=ttmp.dwHighDateTime; // convert ttmp to two int32
      tend.LowPart=ttmp.dwLowDateTime;

     // Compute execution time
      texec=((double)((tend.QuadPart-tbegin.QuadPart)/10000))/1000.;
      qDebug()<<"Skew angle de HT using Otsu ";
      qDebug()<<-skew_angle;
      qDebug()<<"Temps d'execution de HT using Otsu ";
      qDebug()<<texec;
      //QMessageBox::information(this, tr("Skew angle estimation using Hough Transform method with Otsu"), tr("Best angle = %1.").arg(skew_angle));

}

void Skew_estimation::on_actionAIO_Hybride_triggered()
{


    // Variables
      ULARGE_INTEGER tbegin,tend;
      FILETIME ttmp={0,0}; // temporary variable
      double texec=0.;

      // Start timer
      ::GetSystemTimeAsFileTime(&ttmp); // store current time in ttmp structure
      tbegin.HighPart=ttmp.dwHighDateTime; // convert ttmp to two int32
      tbegin.LowPart=ttmp.dwLowDateTime;

      /* YOUR PROGRAM HERE */
      /* YOUR PROGRAM HERE */
      /* YOUR PROGRAM HERE */
      this->on_actionGris_triggered();
          this->on_actionHybride_triggered();
          this->on_actionInverser_triggered();
          this->on_actionSquelettisation_triggered();
          this->on_actionSupprimer_les_pixels_isol_s_triggered();
          this->on_actionTransform_de_Hough_triggered();
      // End timer
      ::GetSystemTimeAsFileTime(&ttmp); // store current time in ttmp structure
      tend.HighPart=ttmp.dwHighDateTime; // convert ttmp to two int32
      tend.LowPart=ttmp.dwLowDateTime;

     // Compute execution time
      texec=((double)((tend.QuadPart-tbegin.QuadPart)/10000))/1000.;
      qDebug()<<"Skew angle de HT using Hybride ";
      qDebug()<<-skew_angle;
      qDebug()<<"Temps d'execution de HT using Hybride ";
      qDebug()<<texec;
      //QMessageBox::information(this, tr("Skew angle estimation using Hough Transform method with Hybride"), tr("Best angle = %1.").arg(skew_angle));


}

void Skew_estimation::on_actionCorrection_automatique_triggered()
{//qDebug()<<image_source->width;qDebug()<<image_source->height;
image_corrected=RotateImage(image_source,skew_angle);
//qDebug()<<image_corrected->width;qDebug()<<image_corrected->height;
versLabel(image_corrected);


}

void Skew_estimation::on_actionProfile_de_projection_triggered()
{     // Variables
    ULARGE_INTEGER tbegin,tend;
    FILETIME ttmp={0,0}; // temporary variable
    double texec=0.;

    // Start timer
    ::GetSystemTimeAsFileTime(&ttmp); // store current time in ttmp structure
    tbegin.HighPart=ttmp.dwHighDateTime; // convert ttmp to two int32
    tbegin.LowPart=ttmp.dwLowDateTime;

    /* YOUR PROGRAM HERE */
    /* YOUR PROGRAM HERE */
    /* YOUR PROGRAM HERE */
    this->on_actionGris_triggered();
    this->on_actionOtsu_triggered();
    this->on_actionInverser_triggered();
    skew_angle=TadProjectionProfile(image_bin);
    // End timer
    ::GetSystemTimeAsFileTime(&ttmp); // store current time in ttmp structure
    tend.HighPart=ttmp.dwHighDateTime; // convert ttmp to two int32
    tend.LowPart=ttmp.dwLowDateTime;

   // Compute execution time
    texec=((double)((tend.QuadPart-tbegin.QuadPart)/10000))/1000.;
    qDebug()<<"Skew angle de PP using Otsu ";
    qDebug()<<-skew_angle;
    qDebug()<<"Temps d'execution de PP using Otsu ";
    qDebug()<<texec;
    qDebug()<<"\\\\\--------------------------------------/////";
//QMessageBox::information(this, tr("Skew angle estimation using projection profile method"), tr("Best angle = %1.").arg(skew_angle));
}

void Skew_estimation::on_actionProfile_de_projection_Hybride_triggered()
{

    // Variables
    ULARGE_INTEGER tbegin,tend;
    FILETIME ttmp={0,0}; // temporary variable
    double texec=0.;

    // Start timer
    ::GetSystemTimeAsFileTime(&ttmp); // store current time in ttmp structure
    tbegin.HighPart=ttmp.dwHighDateTime; // convert ttmp to two int32
    tbegin.LowPart=ttmp.dwLowDateTime;

    /* YOUR PROGRAM HERE */
    /* YOUR PROGRAM HERE */
    /* YOUR PROGRAM HERE */
    this->on_actionGris_triggered();
    this->on_actionHybride_triggered();
    this->on_actionInverser_triggered();
    skew_angle=TadProjectionProfile(image_bin);
    // End timer
    ::GetSystemTimeAsFileTime(&ttmp); // store current time in ttmp structure
    tend.HighPart=ttmp.dwHighDateTime; // convert ttmp to two int32
    tend.LowPart=ttmp.dwLowDateTime;

   // Compute execution time
    texec=((double)((tend.QuadPart-tbegin.QuadPart)/10000))/1000.;
    qDebug()<<"Skew angle de PP using Hybride ";
    qDebug()<<-skew_angle;
    qDebug()<<"Temps d'execution de PP using Hybride";
    qDebug()<<texec;
qDebug()<<"\\\\\--------------------------------------/////";
   // QMessageBox::information(this, tr("Skew angle estimation using projection profile method"), tr("Best angle = %1.").arg(skew_angle));
}
