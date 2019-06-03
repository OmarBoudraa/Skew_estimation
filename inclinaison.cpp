#include "inclinaison.h"
IplImage *TadSquelettisation(IplImage* src){
    IplImage * img=cvCloneImage(src);
    IplImage * skel=cvCloneImage(src);
    cvSetZero(skel);
    cv::Mat temp(img->height, img->width, CV_8UC1);
    cv::Mat element = cv::getStructuringElement(cv::MORPH_CROSS, cv::Size(3, 3));
    bool done;
    do
    {
    cv::morphologyEx(cv::cvarrToMat(img), temp, cv::MORPH_OPEN, element);//cv::Mat(img)
    cv::bitwise_not(temp, temp);
    cv::bitwise_and(cv::cvarrToMat(img), temp, temp);//cv::Mat(img)
    cv::bitwise_or(cv::cvarrToMat(skel), temp, cv::cvarrToMat(skel));//cv::Mat(skel)
    cv::erode(cv::cvarrToMat(img), cv::cvarrToMat(img), element);//cv::Mat(img)
    double max;
    cv::minMaxLoc(cv::cvarrToMat(img), 0, &max);//cv::Mat(img)
    done = (max == 0);
    } while (!done);
return skel;

}
IplImage *TadIsolatedpixels(IplImage* src){
    IplImage* dst=cvCloneImage(src); bool isolated=true;
        for( int i = 0; i <src->width; i++)
        {for(int  j = 0; j < src->height; j++)
            {isolated=true;
                if (i-1>=0&&cvGetReal2D(src,j,i-1)==255.0){isolated=false;}
if (j-1>=0&&cvGetReal2D(src,j-1,i)==255.0){isolated=false;}
if (i+1<src->width&&cvGetReal2D(src,j,i+1)==255.0){isolated=false;}
if (j+1<src->height&&cvGetReal2D(src,j+1,i)==255.0){isolated=false;}
if (i-1>=0&&j-1>=0&&cvGetReal2D(src,j-1,i-1)==255.0){isolated=false;}
if (i-1>=0&&j+1<src->height&&cvGetReal2D(src,j+1,i-1)==255.0){isolated=false;}
if (j-1>=0&&i+1<src->width&&cvGetReal2D(src,j-1,i+1)==255.0){isolated=false;}
if (i+1<src->width&&j+1<src->height&&cvGetReal2D(src,j+1,i+1)==255.0){isolated=false;}

            if (isolated){cvSetReal2D(dst,j,i,0);}
            }}
      // dst= TadInvImgGRIS(dst);
                                                            return dst;
}
IplImage * TadTransfHough(IplImage* img_src, double *angle, int threshold, double minLineLength, double maxLineGap){

cv::Mat dst, cdst;
dst=cv::cvarrToMat(img_src,true);//cv::Mat(img_src,true);
//IplImage* tmp= TadInvImgGRIS(Mat2IplImage(dst));
cvtColor(dst, cdst, CV_GRAY2BGR);
vector<Vec4i> lines;
*angle=0.0;
int max_accumulator=0;
//HoughLinesP(dst, lines, 1, CV_PI/180, 50, 50, 10 );
HoughLinesP(dst, lines, 1, CV_PI/1800, threshold, minLineLength, maxLineGap );

Point p1, p2,t1,t2;
for( size_t i = 0; i < lines.size(); i++ )
{
Vec4i l = lines[i];
line( cdst, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0,0,255));

p1=Point(l[0], l[1]);
p2=Point(l[2], l[3]);
if (max_accumulator<nb_line_pixels( img_src, p1, p2 )){

//calculate angle in radian, if you need it in degrees just do angle * 180 / PI
 *angle = float_round((atan2(p2.y-p1.y ,p2.x - p1.x)*180/CV_PI),1);
 max_accumulator=nb_line_pixels( img_src, p1, p2 );
 t1=p1;t2=p2;
}
}
line( cdst, t1, t2, Scalar(255,0,0),3);
if (*angle>=45.0){*angle=-90.0+*angle;}else{if (*angle<=-45.0){*angle=90.0+*angle;}}

IplImage *img_Hough=Mat2IplImage(cdst);
return img_Hough;
}
/*double TadSkewAngle(IplImage* img_src, int threshold, double minLineLength, double maxLineGap){

cv::Mat dst;
dst=cv::Mat(img_src,true);
double angle=0.0;int max_accumulator=0;
vector<Vec4i> lines;
//HoughLinesP(dst, lines, 1, CV_PI/180, 50, 50, 10 );
HoughLinesP(dst, lines, 1, CV_PI/1800, threshold, minLineLength, maxLineGap );
for( size_t i = 0; i < lines.size(); i++ )
{
Vec4i l = lines[i];
Point p1, p2;
p1=Point(l[0], l[1]);
p2=Point(l[2], l[3]);
if (max_accumulator<nb_line_pixels( img_src, p1, p2 )){

//calculate angle in radian, if you need it in degrees just do angle * 180 / PI
 angle = float_round((atan2(p2.y-p1.y ,p2.x - p1.x)*180/CV_PI),1);
 max_accumulator=nb_line_pixels( img_src, p1, p2 );
}
}

//qDebug()<<angle;
return angle;
}*/
int nb_line_pixels( IplImage* img, CvPoint pt1, CvPoint pt2 )
{
CvLineIterator iterator;
int nb = 0;
int count = cvInitLineIterator( img, pt1, pt2, &iterator, 8 );
for( int i = 0; i < count; i++ ){if (iterator.ptr[0]==255.0){nb++;}

CV_NEXT_LINE_POINT(iterator);

}
return nb;
}
IplImage* Mat2IplImage(cv::Mat mat) {
IplImage* image;
image = cvCreateImage(cvSize(mat.cols, mat.rows), 8, mat.channels());
IplImage ipltemp = mat;
cvCopy(&ipltemp, image);
return image;
}
IplImage* RotateImage(IplImage* img_src, double angle) {
   /* cv::Mat src = cv::Mat(img_src,true);
    cv::Mat dst;
    cv::Point2f pc(src.cols/2., src.rows/2.);
    cv::Mat r = cv::getRotationMatrix2D(pc, angle, 1.0);
    cv::warpAffine(src, dst, r, cvSize(src.cols,src.rows)); // what size I should use?
    IplImage *returned=Mat2IplImage(dst);
    return returned;*/
    Mat img = cv::cvarrToMat(img_src,true);//cv::Mat(img_src,true);
    double offsetX, offsetY;
    double width = img.size().width;
    double height = img.size().height;
    Point2d center = Point2d (width / 2, height / 2);
    //qDebug()<<width;qDebug()<<height;
    Rect bounds = RotatedRect (center, img.size(), angle).boundingRect();
    //qDebug()<<bounds.width;qDebug()<<bounds.height;
    //RotatedRect bounds = RotatedRect (center, img.size(), angle);
    Mat resized = Mat::zeros(bounds.size(), img.type());
    offsetX = (bounds.width-width) / 2;
    offsetY = (bounds.height-height) / 2;
     Rect roi = Rect (offsetX, offsetY, width, height);
    img.copyTo (resized(roi));
    //qDebug()<<resized.cols;qDebug()<<resized.rows;
     center += Point2d (offsetX, offsetY);
    Mat M = getRotationMatrix2D (center, angle, 1.0);
     Mat dst;
     cv::warpAffine (resized, dst, M, resized.size());
     IplImage *returned=Mat2IplImage(dst);
     //cvThreshold(returned,returned,127,255,CV_THRESH_BINARY);
        // qDebug()<<returned->width;qDebug()<<returned->height;
     return returned;
}
double TadProjectionProfile(IplImage* img_src){
    double angle; int taille=img_src->height;double best_angle=0.0,best_var=0.0;
    int PP[taille];
    double valeur,mean,sum;

    for( angle = -20.0; angle <= 20.0; angle=angle+1 ){
         IplImage *tempo=RotateImage(img_src, angle);
           // Pour assurer que les valeurs de l'image soient 1 ou 0 seulement
           cvThreshold(tempo,tempo,127,255,CV_THRESH_BINARY);
           int PP[taille];
           mean=0.0;sum=0.0;
           for( int i = 0; i < taille; i++ ){PP[i]=0;}
           for( int i = 0; i <img_src->width; i++){
           for(int  j = 0; j < img_src->height; j++){if (cvGetReal2D(tempo,j,i)==255.0){PP[j]++;}}}

           for( int i = 0; i < taille; i++ ){
               valeur=PP[i];
               mean+=valeur;
               sum+=pow(valeur,2);
              }
          mean/=taille;
          if (((sum/taille)-pow(mean,2))>best_var){best_angle=angle;best_var=((sum/taille)-pow(mean,2));}
       cvReleaseImage(&tempo);  }

for( angle = best_angle-0.9; angle <= best_angle+0.9; angle=angle+0.1 ){

    IplImage *tempo=RotateImage(img_src, angle);
      // Pour assurer que les valeurs de l'image soient 1 ou 0 seulement
      cvThreshold(tempo,tempo,127,255,CV_THRESH_BINARY);
      int PP[taille];
      mean=0.0;sum=0.0;
      for( int i = 0; i < taille; i++ ){PP[i]=0;}
      for( int i = 0; i <img_src->width; i++){
      for(int  j = 0; j < img_src->height; j++){if (cvGetReal2D(tempo,j,i)==255.0){PP[j]++;}}}

      for( int i = 0; i < taille; i++ ){
          valeur=PP[i];
          mean+=valeur;
          sum+=pow(valeur,2);
         }
     mean/=taille;
     if (((sum/taille)-pow(mean,2))>best_var){best_angle=angle;best_var=((sum/taille)-pow(mean,2));}
  cvReleaseImage(&tempo);



}
    return best_angle;

    /*   for( angle = -20.0; angle <= 20.0; angle=angle+1 ){
     IplImage *tempo=RotateImage(img_src, angle);
       // Pour assurer que les valeurs de l'image soient 1 ou 0 seulement
       cvThreshold(tempo,tempo,127,255,CV_THRESH_BINARY);
       int PP[taille];for( int i = 0; i < taille; i++ ){PP[i]=0;}
       for( int i = 0; i <img_src->width; i++){
       for(int  j = 0; j < img_src->height; j++){if (cvGetReal2D(tempo,j,i)==255.0){PP[j]++;}}}
       for( int i = 0; i < taille; i++ ){
       if (peak<PP[i]){best_angle=angle;peak=PP[i];}}
   cvReleaseImage(&tempo);
       }
       for( angle = best_angle-0.9; angle <= best_angle+0.9; angle=angle+0.1 ){
       IplImage *tempo=RotateImage(img_src, angle);
       // Pour assurer que les valeurs de l'image soient 1 ou 0 seulement
       cvThreshold(tempo,tempo,127,255,CV_THRESH_BINARY);
       for( int i = 0; i < taille; i++ ){PP[i]=0;}
       for( int i = 0; i <img_src->width; i++){for(int  j = 0; j < img_src->height; j++){if (cvGetReal2D(tempo,j,i)==255.0){PP[j]++;}}}
       for( int i = 0; i < taille; i++ ){if (peak<PP[i]){best_angle=angle;peak=PP[i];}}
   cvReleaseImage(&tempo);
       }*/
}
double float_round(double val, int p)
{
double t=pow(10.0,p);

return (double)((int) trunc((val*t)+((val*t)>=0.0?0.5:-0.5)))/t;
}
