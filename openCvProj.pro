QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
INCLUDEPATH += C:\openCV\opencv\build\include
LIBS += C:\openCV\opencv\release\bin\libopencv_calib3d470.dll
LIBS += C:\openCV\opencv\release\bin\libopencv_core470.dll
LIBS += C:\openCV\opencv\release\bin\libopencv_highgui470.dll
LIBS += C:\openCV\opencv\release\bin\libopencv_imgcodecs470.dll
LIBS += C:\openCV\opencv\release\bin\libopencv_imgproc470.dll


SOURCES += \
    EdgeDetection.cpp \
    Histogram.cpp \
    Image.cpp \
    main.cpp \
    mainwindow.cpp\
    Processing.cpp\
    Threshold.cpp \
    noise.cpp \
    noiseFilters.cpp

HEADERS += \
    EdgeDetection.h \
    Histogram.h \
    Image.h \
    mainwindow.h\
    Processing.h\
    Threshold.h \
    noise.h \
    noiseFilters.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
