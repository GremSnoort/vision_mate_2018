#include "cv_detect.h"

CV_Detect::CV_Detect()
{

}

Mat CV_Detect::debug(Mat frame, CV_SettingsWidget *W)
{
    Mat mask(frame.clone());
    Mat show(mask.rows, mask.cols, CV_8UC3, Scalar::all(0));//0
    inRange(frame
            , Scalar(W->R_bottom()
                     , W->G_bottom()
                     , W->B_bottom())
            , Scalar(W->R_top()
                     , W->G_top()
                     , W->B_top())
            , mask);

    show = CV_Detect::MakeSelection(frame, mask, show, W->cb_debugColor->currentIndex());

    return show;
}

Mat CV_Detect::MakeSelection(Mat frame, Mat mask, Mat show, int c)
{
    for(int x=0;x<frame.rows;x++)
        for(int y=0;y<frame.cols;y++)
            if( mask.at<unsigned char>(x,y)== 255)
            {
                if(c==0)
                    show.at<Vec3b>(x,y)[1] = 255;
                else
                    show.at<Vec3b>(x,y) = frame.at<Vec3b>(x,y);
            }

    return show;
}

Mat CV_Detect::ColorFilter(Mat frame, CV_SettingsWidget *W, int c)
{
    Mat mask(frame.clone());
    Mat show(mask.rows, mask.cols, CV_8UC3, Scalar::all(0));//0
    inRange(frame
            , Scalar(W->defolt.data()->at(c)->R.bottom
                     , W->defolt.data()->at(c)->G.bottom
                     , W->defolt.data()->at(c)->B.bottom)
            , Scalar(W->defolt.data()->at(c)->R.top
                     , W->defolt.data()->at(c)->G.top
                     , W->defolt.data()->at(c)->B.top)
            , mask);

    show = CV_Detect::MakeSelection(frame, mask, show, c);

    if(CV_Detect::tmp_detector(show))
        CV_Detect::COLOR = static_cast<figureColor>(c);

    return show;
}

bool CV_Detect::tmp_detector(Mat show)
{
    vector<vector<Point> > contours;
    Mat tmp(show.clone());
    cvtColor(tmp, tmp, CV_RGB2GRAY);
    findContours(tmp, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);

    for(unsigned int i=0;i<contours.size();i++)
    {
        if(contourArea(contours[i])>500)
            return true;
    }

    return false;
}
