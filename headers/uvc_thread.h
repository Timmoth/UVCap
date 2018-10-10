#pragma once

#include <QThread>
#include <QtCore>
#include <QtGui>

#include "opencv2/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "libuvc/libuvc.h"
#include "jpeglib.h"
#include <stdio.h>
#include <iostream>
#include <unistd.h>
#include <QImage>

using namespace cv;

class UVCThread : public QThread
{
    Q_OBJECT
public:
    explicit UVCThread(QObject *parent = 0);
    ~UVCThread();
    void run();
    bool running;
    static void cb(uvc_frame_t *frame, void *ptr);

signals:
    void captureImage(QImage);
};