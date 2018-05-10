#include "cv_colorfiguredetect.h"

CV_ColorFigureDetect::CV_ColorFigureDetect()
{

}

Mat CV_ColorFigureDetect::Debug(Mat frame_, CV_SettingsWidget *W)
{
    Mat frame = frame_(Rect(frame_.cols*0.05, 0, frame_.cols*0.95, frame_.rows*0.95));
    cv::cvtColor(frame, frame, CV_BGR2RGB);

    Mat debugMat = CV_Detect::debug(frame, W);

    vector<vector<Point>> contours;
    Mat tmp(debugMat.clone());
    cvtColor(tmp, tmp, CV_RGB2GRAY);
    findContours(tmp, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);

    for(unsigned int i=0;i<contours.size();i++)
    {
        if(contourArea(contours[i])>500)
        {
            drawContours( debugMat, contours, i,  CV_RGB(255, 0, 0), 2, LINE_8, -1, 0 );
            Rect rect = boundingRect(contours[i]);
            rectangle(debugMat, rect, CV_RGB(0, 255, 0), 3, 8, 0);
            int x = rect.x + rect.width/2;
            int y = rect.y + rect.height/2;
            putText(debugMat, "+",   Point2i(x,y), CV_FONT_HERSHEY_COMPLEX, 1, Scalar(255, 0, 0), 2);

            break;
        }
    }

    cv::cvtColor(debugMat, debugMat, CV_RGB2BGR);
    return debugMat;
}

Mat CV_ColorFigureDetect::Detect(Mat frame_, CV_SettingsWidget *W)
{
    Mat frame = frame_.clone();
    frame = frame(Rect(frame_.cols*0.05, 0, frame_.cols*0.9, frame_.rows*0.95));
    cv::cvtColor(frame, frame, CV_BGR2RGB);

    Mat showRed = CV_Detect::ColorFilter(frame, W, 1);
    Mat showYellow = CV_Detect::ColorFilter(frame, W, 2);
    Mat showBlue = CV_Detect::ColorFilter(frame, W, 3);

    Mat show;

    if(CV_Detect::COLOR == RED_)
        show = showRed;
    else if(CV_Detect::COLOR == YELLOW_)
        show = showYellow;
    else if(CV_Detect::COLOR == BLUE_)
        show = showBlue;
    else
    {
        cv::cvtColor(frame, frame, CV_RGB2BGR);
        return frame;
    }

    vector<vector<Point>> contours;
    Mat tmp(show.clone());
    cvtColor(tmp, tmp, CV_RGB2GRAY);
    findContours(tmp, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);

    for(unsigned int i=0;i<contours.size();i++)
    {
        if(contourArea(contours[i])>500)
        {
            drawContours( show, contours, i,  CV_RGB(255, 0, 0), 2, LINE_8, -1, 0 );
            Rect rect = boundingRect(contours[i]);
            rectangle(show, rect, CV_RGB(0, 255, 0), 3, 8, 0);
            int x = rect.x + rect.width/2;
            int y = rect.y + rect.height/2;
            putText(show, "+",   Point2i(x,y), CV_FONT_HERSHEY_COMPLEX, 1, Scalar(255, 0, 0), 2);

            vector<Point> approx;
            approxPolyDP(Mat(contours[i]), approx, arcLength(cv::Mat(contours[i]), true) * 0.01, true);

            if(approx.size()==3)
            {
                if(CV_Detect::COLOR == RED_)
                    putText(show, "A",   Point2i(50, 100), CV_FONT_HERSHEY_COMPLEX, 3, cv::Scalar(255, 0, 0), 2);
                else if(CV_Detect::COLOR == YELLOW_)
                    putText(show, "B",   Point2i(50, 100), CV_FONT_HERSHEY_COMPLEX, 3, cv::Scalar(255, 255, 0), 2);
                else if(CV_Detect::COLOR == BLUE_)
                    putText(show, "C",   Point2i(50, 100), CV_FONT_HERSHEY_COMPLEX, 3, cv::Scalar(0, 0, 255), 2);

                CV_Detect::TYPE = TYPE_FIGURE;
            }

            else if(approx.size()==4)
            {
                if(CV_Detect::COLOR == RED_)
                    putText(show, "D",   Point2i(50, 100), CV_FONT_HERSHEY_COMPLEX, 3, cv::Scalar(255, 0, 0), 2);
                else if(CV_Detect::COLOR == YELLOW_)
                    putText(show, "E",   Point2i(50, 100), CV_FONT_HERSHEY_COMPLEX, 3, cv::Scalar(255, 255, 0), 2);
                else if(CV_Detect::COLOR == BLUE_)
                    putText(show, "F",   Point2i(50, 100), CV_FONT_HERSHEY_COMPLEX, 3, cv::Scalar(0, 0, 255), 2);

                CV_Detect::TYPE = TYPE_FIGURE;
            }

            break;
        }
    }

    cv::cvtColor(show, show, CV_RGB2BGR);
    return show;
}
