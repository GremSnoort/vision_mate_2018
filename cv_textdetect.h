#ifndef CV_TEXTDETECT_H
#define CV_TEXTDETECT_H

#include "cv_detect.h"

using namespace std;

using namespace cv;

class CV_TextDetect
{
public:
    CV_TextDetect();

    static QList<Mat> tmpl_arr;

    static Mat Detect(Mat frame, CV_SettingsWidget* W);

    static Mat PrintAns(Mat show, QList<int> NUMS);
};

#endif // CV_TEXTDETECT_H
