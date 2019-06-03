#ifndef INCLINAISON_H
#define INCLINAISON_H
#include "Skew_estimation.h"
IplImage *TadSquelettisation(IplImage* src);
IplImage *TadIsolatedpixels(IplImage* src);
IplImage *TadTransfHough(IplImage* img_src, double *angle, int threshold=50, double minLineLength=200, double maxLineGap=1000);
//double TadSkewAngle(IplImage* img_src, int threshold=50, double minLineLength=200, double maxLineGap=1000);
int nb_line_pixels( IplImage* img, CvPoint pt1, CvPoint pt2 );
IplImage* Mat2IplImage(cv::Mat mat);
IplImage* RotateImage(IplImage* img_src, double angle);
double TadProjectionProfile(IplImage* img_src);
double float_round(double val, int p);
#endif // INCLINAISON_H
