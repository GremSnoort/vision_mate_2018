#ifndef CV_DETECT_H
#define CV_DETECT_H

#include <QObject>
#include <QWidget>
#include <QCoreApplication>
#include <QDebug>

#include <opencv2/opencv.hpp>

#include "cv_settingswidget.h"

using namespace std;

using namespace cv;

class CV_Detect
{
public:
    CV_Detect();

    static type TYPE;
    static figureColor COLOR;

    static bool tmp_detector(Mat show);

    static Mat MakeSelection(Mat frame, Mat mask, Mat show, int c);

    static Mat ColorFilter(Mat frame, CV_SettingsWidget* W, int c);

    static Mat debug(Mat frame, CV_SettingsWidget* W);
};

#endif // CV_DETECT_H
