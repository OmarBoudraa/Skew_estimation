#include "pretraitement.h"

IplImage*  TadFiltreGauss(IplImage *src){IplImage*  img_filtre=cvCloneImage(src);
cvSmooth( src, img_filtre,CV_GAUSSIAN);
return img_filtre;}
IplImage*  TadFiltreMoyen(IplImage *src){
IplImage*  img_filtre=cvCloneImage(src);
cvSmooth( src, img_filtre,CV_BLUR);
return img_filtre;
}
IplImage*  TadFiltreMedian(IplImage *src){IplImage*  img_filtre=cvCloneImage(src);
cvSmooth( src, img_filtre,CV_MEDIAN);
return img_filtre;}
IplImage*  TadFiltreWiener(IplImage *src){
QProgressDialog pd("Patientez SVP...", "Annuler", 0, 2*src->width);
            pd.setWindowModality(Qt::WindowModal);
            pd.setWindowTitle("filtrage");
         pd.setFixedWidth(pd.width()*2);
    double var_tot,valeur,mean_loc,var_loc,mean,sum;
          IplImage* img_wiener=cvCloneImage(src);
            int i,j,k,l;
            CvMat *var=cvCreateMat(src->height,src->width,CV_64FC1);

                                               for( i = 0; i <src->width; i++)
                               {for( j = 0; j < src->height; j++)
                                   { mean=0;sum=0;
                  for(int d = i-1;d<i+2; d++)
                               {for(int  s= j-1;s<j+2; s++){
                  if(d<0||d>=src->width){l=2*i-d;}else{l=d;}
                  if(s<0||s>=src->height){k=2*j-s;}else{k=s;}
              valeur=cvGetReal2D(src,k,l);
              mean+=valeur;
              sum+=pow(valeur,2);
             }}
         mean/=9;sum=(sum/9)-pow(mean,2);
             cvSetReal2D(var,j,i,sum);


                         } if (pd.wasCanceled()) break;
                             pd.setValue((i));
                       }

              for( i = 0; i <src->width; i++)
                      {for( j = 0; j < src->height; j++)
                          { mean_loc=0;var_loc=0;var_tot=0;
         for(int d = i-1;d<i+2; d++)
                      {for(int  s= j-1;s<j+2; s++){
         if(d<0||d>=src->width){l=2*i-d;}else{l=d;}
         if(s<0||s>=src->height){k=2*j-s;}else{k=s;}
    valeur=cvGetReal2D(src,k,l);
mean_loc+=valeur;
var_loc+=pow(valeur,2);
var_tot+=cvGetReal2D(var,k,l);
    }}
mean_loc/=9;var_tot/=9;
var_loc=((var_loc/9)-pow(mean_loc,2));
valeur=cvGetReal2D(src,j,i);
double val_wiener=mean_loc+((var_loc-var_tot)*(valeur-mean_loc)/var_loc);
 cvSetReal2D(img_wiener,j,i,val_wiener);
                } if (pd.wasCanceled()) break;
                    pd.setValue(i+src->width);
              }

         if (pd.wasCanceled())
              { pd.setValue(src->width); return src;} else{return img_wiener;}}
IplImage*  TadFiltreMedianIter(int ite, IplImage *src){IplImage*  img_filtre=cvCloneImage(src);
for( int i = 0; i <ite; i++){cvSmooth( img_filtre, img_filtre,CV_MEDIAN);}
return img_filtre;}
IplImage* TadSeuillageGlobFixe(IplImage *src){IplImage* img_fixe=cvCloneImage(src);
cvThreshold(src,img_fixe,127,255,CV_THRESH_BINARY); return img_fixe;}
int TadOtsuSeuil(IplImage *src){


                                     // on récupère la dimension de l'image
                int w = src->width;
                int h = src->height;

                                    //  maxVal et minVal contient les valeur extrimum des niveaux de gris
                int maxVal = 0;
                int minVal = 255;
                for (int i = 0; i<h;i++)
                {
                  for (int j = 0; j < w; j++)
                  {
                  int u = (unsigned char)src->imageData[ i*w + j];
                  if(u < minVal)
                    minVal = u;
                  if(u > maxVal)
                    maxVal = u;
                  }
                }

                double T = 0.5 * (minVal + maxVal);

                bool done = false;
                while(!done)  // Calcule du seuil
                {
                                  // mean1 et mean2 contien les moyennes des deux classes
                    double   mean1 = 0;
                    double   mean2 = 0;
                    int num1 = 0; // la cardianilité de la premiere classe
                    for (int i = 0; i < h; i++)
                    {
                      for (int j = 0; j < w; j++)
                      {
                        int u = (unsigned char)src->imageData[i*w+j];
                        if( u < T)
                        {
                          mean1 += u;
                          num1++;
                        }
                        else
                          mean2 += u;
                      }
                    }
                    mean1 = mean1*1.0/num1;
                    mean2 = mean2*1.0/(w*h - num1) ;


                    double Tnext = 0.5 * (mean1 + mean2);

                    done = (abs(T-Tnext) < 0.5);  // nouveau état
                    T = Tnext;
                }

 return T;}
IplImage* TadOtsu(IplImage *src){IplImage* img_otsu=cvCloneImage(src);
cvThreshold(src,img_otsu,TadOtsuSeuil(src),255,CV_THRESH_OTSU); return img_otsu;}
IplImage *TadNiblack(IplImage *src){
QProgressDialog pd("Patientez SVP...", "Annuler", 0, src->width);
            pd.setWindowModality(Qt::WindowModal);
            pd.setWindowTitle("binarisation");
         pd.setFixedWidth(pd.width()*2);
    double valeur,mean,sum,seuil; IplImage *img_nib=cvCloneImage(src);
            int i,j,k,l;
              for( i = 0; i <src->width; i++)
                      {for( j = 0; j < src->height; j++)
                          { mean=0;sum=0;
         for(int d = i-12;d<i+13; d++)
                      {for(int  s= j-12;s<j+13; s++){
         if(d<0||d>=src->width){l=2*i-d;}else{l=d;}
         if(s<0||s>=src->height){k=2*j-s;}else{k=s;}
     valeur=cvGetReal2D(src,k,l);
     mean+=valeur;
     sum+=pow(valeur,2);
    }}
mean/=625;
seuil=(mean-(0.2*sqrt(((sum/625)-pow(mean,2)))));
         valeur=cvGetReal2D(src,j,i);
         (valeur<=seuil)?cvSetReal2D(img_nib,j,i,0):cvSetReal2D(img_nib,j,i,255);
                } if (pd.wasCanceled()) break;
                    pd.setValue(i);
              }

if (pd.wasCanceled())
              { pd.setValue(src->width); return src;}else{return img_nib;} }
IplImage* TadBernsen(IplImage *src){
    QProgressDialog pd("Patientez SVP...", "Annuler", 0, src->width);
                pd.setWindowModality(Qt::WindowModal);
                pd.setWindowTitle("binarisation");
         pd.setFixedWidth(pd.width()*2);
    double valeur,seuil;IplImage* img_ber=cvCloneImage(src);


           int sup,inf,os=TadOtsuSeuil(src);
            int i,j,k,l;
              for( i = 0; i <src->width; i++)
                      {for( j = 0; j < src->height; j++)
                          { sup=0;inf=255;
         for(int d = i-7;d<i+8; d++)
                      {for(int  s= j-7;s<j+8; s++){
         if(d<0||d>=src->width){l=2*i-d;}else{l=d;}
         if(s<0||s>=src->height){k=2*j-s;}else{k=s;}
     valeur=cvGetReal2D(src,k,l);
     if(sup<valeur)sup=valeur;
     if(inf>valeur)inf=valeur;
    }}
         if((sup-inf)>=15){seuil=(double)(sup+inf)/2;}else{seuil=os;}
         valeur=cvGetReal2D(src,j,i);
         (valeur<=seuil)?cvSetReal2D(img_ber,j,i,0):cvSetReal2D(img_ber,j,i,255);
                } if (pd.wasCanceled()) break;
                    pd.setValue(i);
              }

if (pd.wasCanceled())
              { pd.setValue(src->width); return src;} else{return img_ber;}}
IplImage* TadSauvola(IplImage *src){
    QProgressDialog pd("Patientez SVP...", "Annuler", 0, src->width);
                pd.setWindowModality(Qt::WindowModal);
                pd.setWindowTitle("binarisation");
         pd.setFixedWidth(pd.width()*2);
    double valeur,mean,sum,seuil;IplImage* img_sauv=cvCloneImage(src);
            int i,j,k,l;
              for( i = 0; i <src->width; i++)
                      {for( j = 0; j < src->height; j++)
                          { mean=0;sum=0;
         for(int d = i-7;d<i+8; d++)
                      {for(int  s= j-7;s<j+8; s++){
         if(d<0||d>=src->width){l=2*i-d;}else{l=d;}
         if(s<0||s>=src->height){k=2*j-s;}else{k=s;}
     valeur=cvGetReal2D(src,k,l);
     mean+=valeur;
     sum+=pow(valeur,2);
    }}
mean/=225;sum=(double)((sum/225)-pow(mean,2));seuil=(((((sqrt(sum)/-128)+1)*-0.5)+1)*mean);
         valeur=cvGetReal2D(src,j,i);
         (valeur<=seuil)?cvSetReal2D(img_sauv,j,i,0):cvSetReal2D(img_sauv,j,i,255);
                } if (pd.wasCanceled()) break;
                    pd.setValue(i);
              }

if (pd.wasCanceled())
              { pd.setValue(src->width); return src;} else{return img_sauv;}}
IplImage* TadWolf(IplImage *src){
    QProgressDialog pd("Patientez SVP...", "Annuler", 0, 2*src->width);
                pd.setWindowModality(Qt::WindowModal);
                pd.setWindowTitle("binarisation");
         pd.setFixedWidth(pd.width()*2);
    double valeur,mean,sum,seuil;IplImage* img_wolf=cvCloneImage(src);
            int i,j,k,l;double n=0, q=255;
   CvMat *moy , *var;moy =var=cvCreateMat(src->height,src->width,CV_64FC1);

                                      for( i = 0; i <src->width; i++)
                      {for( j = 0; j < src->height; j++)
                          { mean=0;sum=0;
         for(int d = i-5;d<i+6; d++)
                      {for(int  s= j-5;s<j+6; s++){
         if(d<0||d>=src->width){l=2*i-d;}else{l=d;}
         if(s<0||s>=src->height){k=2*j-s;}else{k=s;}
     valeur=cvGetReal2D(src,k,l);
     mean+=valeur;
     sum+=pow(valeur,2);
         if(q>valeur) q= valeur;
    }}
mean/=121;sum=sqrt((sum/121)-pow(mean,2));
    cvSetReal2D(moy,j,i,mean);cvSetReal2D(var,j,i,sum);
if(n<sum) n=sum;

                } if (pd.wasCanceled()) break;
                    pd.setValue((i));
              }

                     for( i = 0; i <src->width; i++)
                      {for( j = 0; j < src->height; j++)
                          {mean=cvGetReal2D(moy,j,i);sum=cvGetReal2D(var,j,i);
seuil=((sum*0.5*(mean-q))/n)+(0.5*q)+(mean*0.5);
         valeur=cvGetReal2D(src,j,i);
         (valeur<=seuil)?cvSetReal2D(img_wolf,j,i,0):cvSetReal2D(img_wolf,j,i,255);
                } if (pd.wasCanceled()) break;
                    pd.setValue(src->width+i);
              }

if (pd.wasCanceled())
              { pd.setValue(2*src->width); return src;} else{return img_wolf;}}
IplImage* TadNick(IplImage *src){
   /* QProgressDialog pd("Patientez SVP...", "Annuler", 0, src->width);
                pd.setWindowModality(Qt::WindowModal);
                pd.setWindowTitle("binarisation");
         pd.setFixedWidth(pd.width()*2);*/
    double valeur,mean,sum,seuil;IplImage* img_nick=cvCloneImage(src);
            int i,j,k,l;
              for( i = 0; i <src->width; i++)
                      {for( j = 0; j < src->height; j++)
                          { mean=0;sum=0;
         for(int d = i-9;d<i+10; d++)
                      {for(int  s= j-9;s<j+10; s++){
         if(d<0||d>=src->width){l=2*i-d;}else{l=d;}
         if(s<0||s>=src->height){k=2*j-s;}else{k=s;}
     valeur=cvGetReal2D(src,k,l);
     mean+=valeur;
     sum+=pow(valeur,2);
    }}
mean/=361;
seuil=(mean-(0.2*sqrt(((sum)-pow(mean,2))/361)));
         valeur=cvGetReal2D(src,j,i);
         (valeur<=seuil)?cvSetReal2D(img_nick,j,i,0):cvSetReal2D(img_nick,j,i,255);
                } /*if (pd.wasCanceled()) break;
                    pd.setValue(i);*/
              }

//if (pd.wasCanceled()) { pd.setValue(src->width); return src;} else{return img_nick;}

return img_nick;
}

// Reserved to Hybrid Binarization

double  TadLocalContrast(IplImage *src){
    double contrast=0.0,mean=0.0, maxv=0.0,minv=255.0,valeur;
            int i,j,k,l;
                                               for( i = 0; i <src->width; i++)
                               {for( j = 0; j < src->height; j++)
                                   {  maxv=0.0;minv=255.0;
                  for(int d = i-1;d<i+2; d++)
                               {for(int  s= j-1;s<j+2; s++){
                  if(d<0||d>=src->width){l=2*i-d;}else{l=d;}
                  if(s<0||s>=src->height){k=2*j-s;}else{k=s;}
                  valeur=cvGetReal2D(src,k,l);
                  if (maxv<valeur){maxv=valeur;}
                   if (minv>valeur){minv=valeur;}
                      }}
             if (maxv+minv>0){contrast=(maxv-minv)/(maxv+minv);}else{contrast=0.0;}

mean=mean+contrast;
                         }
                       }
                                               mean=(double) mean/(src->width*src->height);

return mean;
}
IplImage*  TadConditionnalCLAHE(IplImage *src, double CL, CvSize GS, double Seuil, double contrast){
    IplImage * img_clahe=cvCloneImage(src);
    if (contrast<=Seuil){
    Ptr<CLAHE> clahe = createCLAHE();

        clahe->setClipLimit(CL);
        clahe->setTilesGridSize(GS);
        clahe->apply(cv::cvarrToMat(src), cv::cvarrToMat(img_clahe));
}
    return img_clahe;
}
CvSeq *TadFindCC(IplImage *src){
    IplImage *img_not=cvCloneImage(src);
     cvNot(img_not, img_not);
     Mat IA=cvarrToMat(img_not,true);
     Mat OA;
     int nLabels=cv::connectedComponents(IA,OA,8,CV_32S);
     CvSeq* cvsequence[nLabels]; int label=0;CvMemStorage* storage;
     CvPoint *pnti= new CvPoint();
for(int i = 0; i < nLabels; i++){
    storage  = cvCreateMemStorage(32768);
     cvsequence[i]=cvCreateSeq( CV_32SC2, sizeof(CvSeq), sizeof(CvPoint), storage );}
     for(int r = 0; r < OA.rows; r++){
             for(int c = 0; c < OA.cols; c++){
                  label = OA.at<int>(r, c);
                   pnti->x=c;pnti->y=r;
                   cvSeqPush( cvsequence[label], pnti );
             }
     }

     if(nLabels>1){
         cvsequence[0]->h_next=cvsequence[1];cvsequence[nLabels-1]->h_prev=cvsequence[nLabels-2];}
     else {cvsequence[0]->h_next=NULL;}
     cvsequence[0]->h_prev=NULL;
cvsequence[nLabels-1]->h_next=NULL;

for(int i = 1; i < nLabels-1; i++){
 cvsequence[i]->h_next=cvsequence[i+1];
cvsequence[i]->h_prev=cvsequence[i-1];}

return cvsequence[0];
}
IplImage* TadMLVOtsu(IplImage *src, double contrast){
     IplImage* img_otsu=cvCloneImage(src);
    if (contrast<=0.03){ double W0K, W1K, W2K, W3K, M0, M1, M2, M3, currVarB, maxBetweenVar, M0K, M1K, M2K, M3K,
                MT;
                                             int histogram[256] = { 0 };int value;
                                                                              int N = src->width * src->height;
                                                                              for (int y = 0; y < src->height; y++)
                                                                              {
                                                                              for (int x = 0; x < src->width; x++)
                                                                              {
                                                                              value = (int) round(cvGetReal2D(src,y,x));
                                                                              histogram[value]++;
                                                                              }
                                                                              }
                W0K = 0;
                W1K = 0;
                M0K = 0;
                M1K = 0;
                MT = 0;
                                             int optimalThresh1 = 0;
                                             int optimalThresh2 = 0;
                                             int optimalThresh3 = 0;
                maxBetweenVar = 0;
                for (int k = 0; k <= 255; k++) {
                MT += k * (histogram[k] / (double) N);
                }
                for (int t1 = 0; t1 <= 255; t1++)
                {
                W0K += histogram[t1] / (double) N; //Pi
                M0K += t1 * (histogram[t1] / (double) N); //i * Pi
                M0 = M0K / W0K; //(i * Pi)/Pi
                W1K = 0;
                M1K = 0;
                for (int t2 = t1 + 1; t2 <= 255; t2++)
                {
                W1K += histogram[t2] / (double) N; //Pi
                M1K += t2 * (histogram[t2] / (double) N); //i * Pi
                M1 = M1K / W1K; //(i * Pi)/Pi
                W2K = 1 - (W0K + W1K);
                M2K = MT - (M0K + M1K);
                if (W2K <= 0) break;
                M2 = M2K / W2K;
                W2K = 0;
                M2K = 0;
                for (int t3 = t2 + 1; t3 <= 255; t3++)
                {
                W2K += histogram[t3] / (double) N; //Pi
                M2K += t3 * (histogram[t3] / (double) N); // i*Pi
                M2 = M2K / W2K; //(i*Pi)/Pi
                W3K = 1 - (W0K + W1K + W2K);
                M3K = MT - (M0K + M1K + M2K);
                M3 = M3K / W3K;
                currVarB = W0K * (M0 - MT) * (M0 - MT) + W1K * (M1 - MT) * (M1 - MT) + W2K *
                (M2 - MT) * (M2 - MT) + W3K * (M3 - MT) * (M3 - MT);
                if (maxBetweenVar < currVarB)
                {
                maxBetweenVar = currVarB;
                optimalThresh1 = t1;
                optimalThresh2 = t2;
                optimalThresh3 = t3;
                }
                }
                }
                }double T0=cvThreshold(src,img_otsu,0,255,CV_THRESH_OTSU | CV_THRESH_BINARY);
                 cvThreshold(src,img_otsu,max((double) optimalThresh2,T0),255.0,CV_THRESH_BINARY);
    }
    else if (contrast<=0.04){
        double W0K, W1K, W2K, W3K, M0, M1, M2, M3, currVarB, maxBetweenVar, M0K, M1K, M2K, M3K,
                        MT;
                                                     int histogram[256] = { 0 };int value;
                                                                                      int N = src->width * src->height;
                                                                                      for (int y = 0; y < src->height; y++)
                                                                                      {
                                                                                      for (int x = 0; x < src->width; x++)
                                                                                      {
                                                                                      value = (int) round(cvGetReal2D(src,y,x));
                                                                                      histogram[value]++;
                                                                                      }
                                                                                      }
                        W0K = 0;
                        W1K = 0;
                        M0K = 0;
                        M1K = 0;
                        MT = 0;
                                                     int optimalThresh1 = 0;
                                                     int optimalThresh2 = 0;
                                                     int optimalThresh3 = 0;
                        maxBetweenVar = 0;
                        for (int k = 0; k <= 255; k++) {
                        MT += k * (histogram[k] / (double) N);
                        }
                        for (int t1 = 0; t1 <= 255; t1++)
                        {
                        W0K += histogram[t1] / (double) N; //Pi
                        M0K += t1 * (histogram[t1] / (double) N); //i * Pi
                        M0 = M0K / W0K; //(i * Pi)/Pi
                        W1K = 0;
                        M1K = 0;
                        for (int t2 = t1 + 1; t2 <= 255; t2++)
                        {
                        W1K += histogram[t2] / (double) N; //Pi
                        M1K += t2 * (histogram[t2] / (double) N); //i * Pi
                        M1 = M1K / W1K; //(i * Pi)/Pi
                        W2K = 1 - (W0K + W1K);
                        M2K = MT - (M0K + M1K);
                        if (W2K <= 0) break;
                        M2 = M2K / W2K;
                        W2K = 0;
                        M2K = 0;
                        for (int t3 = t2 + 1; t3 <= 255; t3++)
                        {
                        W2K += histogram[t3] / (double) N; //Pi
                        M2K += t3 * (histogram[t3] / (double) N); // i*Pi
                        M2 = M2K / W2K; //(i*Pi)/Pi
                        W3K = 1 - (W0K + W1K + W2K);
                        M3K = MT - (M0K + M1K + M2K);
                        M3 = M3K / W3K;
                        currVarB = W0K * (M0 - MT) * (M0 - MT) + W1K * (M1 - MT) * (M1 - MT) + W2K *
                        (M2 - MT) * (M2 - MT) + W3K * (M3 - MT) * (M3 - MT);
                        if (maxBetweenVar < currVarB)
                        {
                        maxBetweenVar = currVarB;
                        optimalThresh1 = t1;
                        optimalThresh2 = t2;
                        optimalThresh3 = t3;
                        }
                        }
                        }
                        }
                        double T0=cvThreshold(src,img_otsu,0,255,CV_THRESH_OTSU | CV_THRESH_BINARY);
                        int NT=0,N2=0;
                        for (int i=0; i<src->width;i++){
                            for (int j=0; j<src->height;j++){
                                if (cvGetReal2D(src,j,i)<=T0){NT++;}
                                else if(cvGetReal2D(src,j,i)<=optimalThresh2){N2++;}

                            }
                            }

                         int diff=  abs((int) (optimalThresh2-T0));
                         double quant=(double) N2/NT;
                   if (diff>=5&&diff<=25&&quant<=0.5){cvThreshold(src,img_otsu,max((double) optimalThresh2,T0),255.0,CV_THRESH_BINARY);}
                   else{cvThreshold(src,img_otsu,0,255,CV_THRESH_OTSU | CV_THRESH_BINARY);}



    }
    else if (contrast<=0.085){cvThreshold(src,img_otsu,0,255,CV_THRESH_OTSU | CV_THRESH_BINARY);}
    else {double W0K, W1K, W2K, W3K, M0, M1, M2, M3, currVarB, maxBetweenVar, M0K, M1K, M2K, M3K,
                MT;
                                             int histogram[256] = { 0 };int value;
                                                                              int N = src->width * src->height;
                                                                              for (int y = 0; y < src->height; y++)
                                                                              {
                                                                              for (int x = 0; x < src->width; x++)
                                                                              {
                                                                              value = (int) round(cvGetReal2D(src,y,x));
                                                                              histogram[value]++;
                                                                              }
                                                                              }
                W0K = 0;
                W1K = 0;
                M0K = 0;
                M1K = 0;
                MT = 0;
                                             int optimalThresh1 = 0;
                                             int optimalThresh2 = 0;
                                             int optimalThresh3 = 0;
                maxBetweenVar = 0;
                for (int k = 0; k <= 255; k++) {
                MT += k * (histogram[k] / (double) N);
                }
                for (int t1 = 0; t1 <= 255; t1++)
                {
                W0K += histogram[t1] / (double) N; //Pi
                M0K += t1 * (histogram[t1] / (double) N); //i * Pi
                M0 = M0K / W0K; //(i * Pi)/Pi
                W1K = 0;
                M1K = 0;
                for (int t2 = t1 + 1; t2 <= 255; t2++)
                {
                W1K += histogram[t2] / (double) N; //Pi
                M1K += t2 * (histogram[t2] / (double) N); //i * Pi
                M1 = M1K / W1K; //(i * Pi)/Pi
                W2K = 1 - (W0K + W1K);
                M2K = MT - (M0K + M1K);
                if (W2K <= 0) break;
                M2 = M2K / W2K;
                W2K = 0;
                M2K = 0;
                for (int t3 = t2 + 1; t3 <= 255; t3++)
                {
                W2K += histogram[t3] / (double) N; //Pi
                M2K += t3 * (histogram[t3] / (double) N); // i*Pi
                M2 = M2K / W2K; //(i*Pi)/Pi
                W3K = 1 - (W0K + W1K + W2K);
                M3K = MT - (M0K + M1K + M2K);
                M3 = M3K / W3K;
                currVarB = W0K * (M0 - MT) * (M0 - MT) + W1K * (M1 - MT) * (M1 - MT) + W2K *
                (M2 - MT) * (M2 - MT) + W3K * (M3 - MT) * (M3 - MT);
                if (maxBetweenVar < currVarB)
                {
                maxBetweenVar = currVarB;
                optimalThresh1 = t1;
                optimalThresh2 = t2;
                optimalThresh3 = t3;
                }
                }
                }
                }double T0=cvThreshold(src,img_otsu,0,255.0,CV_THRESH_OTSU | CV_THRESH_BINARY);
                 cvThreshold(src,img_otsu,min((double) optimalThresh1, T0),255.0,CV_THRESH_BINARY);}

    return img_otsu;
                                           }
void TadSTATMetriques(CvSeq *seq, int *nb, double *mean, double *SD){
*nb=0;CvSeq *ptr;*mean=0.0;*SD=0.0;
for(ptr = seq; ptr != NULL; ptr = ptr->h_next) {
    *mean+=ptr->total;

    *SD+=pow(ptr->total,2);

    *nb=*nb+1;}
*mean= (double)*mean/ *nb;
*SD=(double) sqrt(((*SD/ *nb)-pow(*mean,2)));
}
IplImage*  TadHYBRIDNick(IplImage * nvg, IplImage *src, CvSeq *seq, double KN, double seuil){
    double valeur,mean,sum;IplImage* img_nick=cvCloneImage(src);
            int i,j,k,l;CvPoint *pnt; CvSeq *ptr;
    for(ptr = seq; ptr != NULL; ptr = ptr->h_next) {
   if(ptr->total>seuil){
        for(int index=0;index<ptr->total;index++){
            mean=0;sum=0;
            pnt=(CvPoint *) cvGetSeqElem(ptr,index);
           i=pnt->x;j=pnt->y;

           for(int d = i-17;d<i+18; d++)
                        {for(int  s= j-17;s<j+18; s++){
           if(d<0||d>=src->width){l=2*i-d;}else{l=d;}
           if(s<0||s>=src->height){k=2*j-s;}else{k=s;}
       valeur=cvGetReal2D(nvg,k,l);
       mean+=valeur;
       sum+=pow(valeur,2);
      }}
  mean/=1225;
  seuil=(mean+(KN*sqrt(((sum)-pow(mean,2))/1225)));
           valeur=cvGetReal2D(nvg,j,i);
           (valeur<=seuil)?cvSetReal2D(img_nick,j,i,0):cvSetReal2D(img_nick,j,i,255);
        }

   }
    }

    return img_nick;

}
IplImage *TadIsolatedpixelsfiltering(IplImage* src){
    IplImage* dst=cvCloneImage(src); bool isolated=true;
        for( int i = 0; i <src->width; i++)
        {for(int  j = 0; j < src->height; j++)
            {
                if (cvGetReal2D(src,j,i)==255.0){
                isolated=true;
                if ((i-1>=0&&cvGetReal2D(src,j,i-1)==255.0)||(j-1>=0&&cvGetReal2D(src,j-1,i)==255.0)||
                   (i+1<src->width&&cvGetReal2D(src,j,i+1)==255.0)||(j+1<src->height&&cvGetReal2D(src,j+1,i)==255.0)||
                   (i-1>=0&&j-1>=0&&cvGetReal2D(src,j-1,i-1)==255.0)||(i-1>=0&&j+1<src->height&&cvGetReal2D(src,j+1,i-1)==255.0)||
                   (j-1>=0&&i+1<src->width&&cvGetReal2D(src,j-1,i+1)==255.0)||(i+1<src->width&&j+1<src->height&&cvGetReal2D(src,j+1,i+1)==255.0))

                {isolated=false;}


            if (isolated){cvSetReal2D(dst,j,i,0);}}

                else {
            isolated=true;
            if ((i-1>=0&&cvGetReal2D(src,j,i-1)==0.0)||(j-1>=0&&cvGetReal2D(src,j-1,i)==0.0)||
               (i+1<src->width&&cvGetReal2D(src,j,i+1)==0.0)||(j+1<src->height&&cvGetReal2D(src,j+1,i)==0.0)||
               (i-1>=0&&j-1>=0&&cvGetReal2D(src,j-1,i-1)==0.0)||(i-1>=0&&j+1<src->height&&cvGetReal2D(src,j+1,i-1)==0.0)||
               (j-1>=0&&i+1<src->width&&cvGetReal2D(src,j-1,i+1)==0.0)||(i+1<src->width&&j+1<src->height&&cvGetReal2D(src,j+1,i+1)==0.0))
                {isolated=false;}

        if (isolated){cvSetReal2D(dst,j,i,255.0);}
           }

            }}
                                                            return dst;
}
IplImage *TadConvertirBackground(IplImage *src, CvSeq* seq, double seuil){
IplImage* dst=cvCloneImage(src);
CvPoint *pnt; CvSeq *ptr; int i,j;
    for(ptr = seq; ptr != NULL; ptr = ptr->h_next) {
   if(ptr->total<=seuil){
        for(int index=0;index<ptr->total;index++){
            pnt=(CvPoint *) cvGetSeqElem(ptr,index);
           i=pnt->x;j=pnt->y;
        cvSetReal2D(dst,j,i,255.0);
        }}}
    return dst;
}

IplImage *TadSinglePixelsArtifects(IplImage* src){
    IplImage* dst=cvCloneImage(src); bool artifacts;

        for( int i = 0; i <src->width; i++)
        {for(int  j = 0; j < src->height; j++)
            {
                if (cvGetReal2D(src,j,i)==255.0){

                    if(i-1>=0&&j-1>=0&&i+1<src->width&&j+1<src->height){
                artifacts=false;
                if(cvGetReal2D(dst,j-1,i-1)==255.0&&cvGetReal2D(dst,j-1,i)==255.0&&cvGetReal2D(dst,j-1,i+1)==255.0
                        &&cvGetReal2D(dst,j,i-1)==0.0&&cvGetReal2D(dst,j,i+1)==0.0&&cvGetReal2D(dst,j+1,i-1)==0.0
                        &&cvGetReal2D(dst,j+1,i)==0.0&&cvGetReal2D(dst,j+1,i+1)==0.0)
                   {artifacts=true;}
          else if (cvGetReal2D(dst,j-1,i-1)==255.0&&cvGetReal2D(dst,j,i-1)==255.0&&cvGetReal2D(dst,j+1,i-1)==255.0
                        &&cvGetReal2D(dst,j-1,i)==0.0&&cvGetReal2D(dst,j+1,i)==0.0&&cvGetReal2D(dst,j-1,i+1)==0.0
                        &&cvGetReal2D(dst,j,i+1)==0.0&&cvGetReal2D(dst,j+1,i+1)==0.0)
                   {artifacts=true;}
          else if (cvGetReal2D(dst,j+1,i-1)==255.0&&cvGetReal2D(dst,j+1,i)==255.0&&cvGetReal2D(dst,j+1,i+1)==255.0
                        &&cvGetReal2D(dst,j,i-1)==0.0&&cvGetReal2D(dst,j,i+1)==0.0&&cvGetReal2D(dst,j-1,i-1)==0.0
                        &&cvGetReal2D(dst,j-1,i)==0.0&&cvGetReal2D(dst,j-1,i+1)==0.0)
                   {artifacts=true;}
          else if (cvGetReal2D(dst,j-1,i+1)==255.0&&cvGetReal2D(dst,j,i+1)==255.0&&cvGetReal2D(dst,j+1,i+1)==255.0
                        &&cvGetReal2D(dst,j-1,i)==0.0&&cvGetReal2D(dst,j+1,i)==0.0&&cvGetReal2D(dst,j-1,i-1)==0.0
                        &&cvGetReal2D(dst,j,i-1)==0.0&&cvGetReal2D(dst,j+1,i-1)==0.0)
                   {artifacts=true;}
                else {}
                if (artifacts){cvSetReal2D(dst,j,i,0); }
                    }
                    }
            }
        }

        for( int i = 0; i <src->width; i++)
        {for(int  j = 0; j < src->height; j++)
            {
                if (cvGetReal2D(src,j,i)==0.0){

                    if(i-1>=0&&j-1>=0&&i+1<src->width&&j+1<src->height){
                artifacts=false;
                if(cvGetReal2D(dst,j-1,i-1)==0.0&&cvGetReal2D(dst,j-1,i)==0.0&&cvGetReal2D(dst,j-1,i+1)==0.0
                        &&cvGetReal2D(dst,j,i-1)==255.0&&cvGetReal2D(dst,j,i+1)==255.0&&cvGetReal2D(dst,j+1,i-1)==255.0
                        &&cvGetReal2D(dst,j+1,i)==255.0&&cvGetReal2D(dst,j+1,i+1)==255.0)
                   {artifacts=true;}
          else if (cvGetReal2D(dst,j-1,i-1)==0.0&&cvGetReal2D(dst,j,i-1)==0.0&&cvGetReal2D(dst,j+1,i-1)==0.0
                        &&cvGetReal2D(dst,j-1,i)==255.0&&cvGetReal2D(dst,j+1,i)==255.0&&cvGetReal2D(dst,j-1,i+1)==255.0
                        &&cvGetReal2D(dst,j,i+1)==255.0&&cvGetReal2D(dst,j+1,i+1)==255.0)
                   {artifacts=true;}
          else if (cvGetReal2D(dst,j+1,i-1)==0.0&&cvGetReal2D(dst,j+1,i)==0.0&&cvGetReal2D(dst,j+1,i+1)==0.0
                        &&cvGetReal2D(dst,j,i-1)==255.0&&cvGetReal2D(dst,j,i+1)==255.0&&cvGetReal2D(dst,j-1,i-1)==255.0
                        &&cvGetReal2D(dst,j-1,i)==255.0&&cvGetReal2D(dst,j-1,i+1)==255.0)
                   {artifacts=true;}
          else if (cvGetReal2D(dst,j-1,i+1)==0.0&&cvGetReal2D(dst,j,i+1)==0.0&&cvGetReal2D(dst,j+1,i+1)==0.0
                        &&cvGetReal2D(dst,j-1,i)==255.0&&cvGetReal2D(dst,j+1,i)==255.0&&cvGetReal2D(dst,j-1,i-1)==255.0
                        &&cvGetReal2D(dst,j,i-1)==255.0&&cvGetReal2D(dst,j+1,i-1)==255.0)
                   {artifacts=true;}
                else {}
                if (artifacts){cvSetReal2D(dst,j,i,255.0);}
                    }
                    }
            }
        }
                                                            return dst;
}

IplImage * hybridbinarization(IplImage * src)
{

    IplImage *img_nvg= cvCloneImage(src);
    double ctr=TadLocalContrast(img_nvg);
    img_nvg=TadConditionnalCLAHE(img_nvg,1.5,cvSize(64,64),0.02,ctr);
IplImage * img_bin=TadMLVOtsu(img_nvg,ctr);
 CvSeq* CC1=TadFindCC(img_bin);
 int nbre1; double moy1, ET1;
 TadSTATMetriques(CC1->h_next, &nbre1, &moy1, &ET1);
img_bin=TadHYBRIDNick(img_nvg, img_bin, CC1->h_next, -0.1, (double) moy1+(8.0*ET1));
IplImage *img_bin_PT=TadIsolatedpixelsfiltering(img_bin);
CvSeq* CC2=TadFindCC(img_bin_PT);
int nbre2; double moy2, ET2;
TadSTATMetriques(CC2->h_next, &nbre2, &moy2, &ET2);
img_bin_PT=TadConvertirBackground(img_bin_PT, CC2->h_next,(double) (15.0*moy2)/ET2);
img_bin_PT=TadSinglePixelsArtifects(img_bin_PT);
    return img_bin_PT;
}
// Final code


