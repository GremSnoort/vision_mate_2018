#ifndef NEW_NEURALNET_H
#define NEW_NEURALNET_H

#include "cv_detect.h"

#include <QFile>
#include <QCoreApplication>

using namespace std;

using namespace cv;

class NEW_neuralNet
{
public:
    NEW_neuralNet();

    //static QList<int[][]> tmpl_arr;

    static void Generate(Mat frame, CV_SettingsWidget* W);
};

#endif // NEW_NEURALNET_H
