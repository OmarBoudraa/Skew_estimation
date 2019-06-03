#-------------------------------------------------
#
# Project created by QtCreator 2016-07-14T18:46:36
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
qtHaveModule(printsupport): QT += printsupport
CONFIG +=debug
QMAKE_LFLAGS_WINDOWS += -Wl,--stack,32000000
TARGET = Skew_Estimation
TEMPLATE = app
#opencv include files
#INCLUDEPATH += "C:\openc-mingw\install\include"
INCLUDEPATH += "C:\OpenCV3.2.0-MGW\install\include"
#INCLUDEPATH += "C:\QT-project\install\include\opencv2"
#INCLUDEPATH += "C:\openc-mingw\install\include\opencv2"

#opencv libs
#LIBS += -L"C:\openc-mingw\install\x64\mingw\bin"
#LIBS += -lopencv_calib3d248 -lopencv_contrib248 -lopencv_features2d248 -lopencv_core248  -lopencv_features2d248 -lopencv_flann248 -lopencv_gpu248 -lopencv_highgui248 -lopencv_imgproc248 -lopencv_legacy248 -lopencv_ml248 -lopencv_nonfree248 -lopencv_objdetect248 -lopencv_ocl248 -lopencv_photo248 -lopencv_stitching248 -lopencv_superres248 -lopencv_video248 -lopencv_videostab248  # here just use dll file names without "lib" just add "-l" in front
LIBS += -L"C:\OpenCV3.2.0-MGW\install\x86\mingw\bin"
LIBS += -lopencv_calib3d320d -lopencv_features2d320d -lopencv_core320d -lopencv_flann320d -lopencv_highgui320d -lopencv_imgproc320d -lopencv_imgcodecs320d -lopencv_ml320d -lopencv_objdetect320d -lopencv_photo320d -lopencv_shape320d -lopencv_stitching320d -lopencv_superres320d -lopencv_video320d -lopencv_videostab320d  # here just use dll file names without "lib" just add "-l" in front


SOURCES += main.cpp\
        skew_estimation.cpp \
    statistique.cpp \
    pretraitement.cpp \
    inclinaison.cpp \
    houghtransformdialog.cpp

HEADERS  += skew_estimation.h \
    statistique.h \
    pretraitement.h \
    inclinaison.h \
    houghtransformdialog.h

FORMS    += skew_estimation.ui \
    houghtransformdialog.ui

