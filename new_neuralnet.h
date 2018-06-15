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

    static QList<QList<QList<int>>> tmpl_arr;

    static QList<RECT> GetRects(Mat show);

    static bool PropertiesAreValid(QList<RECT> rectList);

    static Mat Detect(Mat frame, CV_SettingsWidget* W);

    static Mat PrintAnswer(Mat show,  int FINAL_1, int FINAL_2, int FINAL_3);

    static void Generate(Mat frame, CV_SettingsWidget* W);
};

#endif // NEW_NEURALNET_H
