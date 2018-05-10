#ifndef CV_COLORFIGUREDETECT_H
#define CV_COLORFIGUREDETECT_H

#include "cv_detect.h"

using namespace cv;

class CV_ColorFigureDetect
{
public:
    CV_ColorFigureDetect();

    static Mat Detect(Mat frame_, CV_SettingsWidget* W);

    static Mat Debug(Mat frame_, CV_SettingsWidget* W);
};

#endif // CV_COLORFIGUREDETECT_H
