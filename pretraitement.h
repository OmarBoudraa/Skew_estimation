#ifndef PRETRAITEMENT_H
#define PRETRAITEMENT_H
#include "Skew_estimation.h"
IplImage*  TadFiltreGauss(IplImage *src);
IplImage*  TadFiltreMoyen(IplImage *src);
IplImage*  TadFiltreMedian(IplImage *src);
IplImage*  TadFiltreWiener(IplImage *src);
IplImage*  TadFiltreMedianIter(int ite, IplImage *src);
IplImage* TadSeuillageGlobFixe(IplImage *src);
int TadOtsuSeuil(IplImage *src);
IplImage* TadOtsu(IplImage *src);
IplImage* TadNiblack(IplImage *src);
IplImage* TadBernsen(IplImage *src);
IplImage* TadSauvola(IplImage *src);
IplImage* TadWolf(IplImage *src);
IplImage* TadNick(IplImage *src);

// Reserved to Hybrid Binarization

double  TadLocalContrast(IplImage *src);
IplImage*  TadConditionnalCLAHE(IplImage *src, double CL, CvSize GS, double Seuil, double contrast);
CvSeq *TadFindCC(IplImage *src);
IplImage* TadMLVOtsu(IplImage *src, double contrast);
void TadSTATMetriques(CvSeq *seq, int *nb, double *mean, double *SD);
IplImage*  TadHYBRIDNick(IplImage * nvg, IplImage *src, CvSeq *seq, double KN, double seuil);
IplImage *TadIsolatedpixelsfiltering(IplImage* src);
IplImage *TadConvertirBackground(IplImage *src, CvSeq* seq, double seuil);
IplImage *TadSinglePixelsArtifects(IplImage* src);
IplImage * hybridbinarization(IplImage * src);
// Final code

#endif // PRETRAITEMENT_H
