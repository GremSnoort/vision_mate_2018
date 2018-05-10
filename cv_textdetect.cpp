#include "cv_textdetect.h"

CV_TextDetect::CV_TextDetect()
{

}

Mat CV_TextDetect::Detect(Mat frame, CV_SettingsWidget *W)
{
    frame = frame(Rect(frame.cols*0.05, 0, frame.cols*0.9, frame.rows*0.95));
    cvtColor(frame, frame, CV_BGR2RGB);
    Mat showBlack = CV_Detect::ColorFilter(frame, W, 0);

    Mat show;

    if(CV_Detect::COLOR == BLACK_)
        show = showBlack;
    else
        return frame;

    vector<vector<Point>> contours;
    Mat tmp(show.clone());
    cvtColor(tmp, tmp, CV_RGB2GRAY);
    findContours(tmp, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);

    QList<int>NUMS;

    QList<Rect> rects;
    QList<int> rectpos;

    for(unsigned int i=0;i<contours.size();i++)
    {
        if(contourArea(contours[i])>500)
        {
            if(rects.size()==3)
                break;

            Rect rect = boundingRect(contours[i]);
            rects << rect;
            rectpos << i;
        }
    }

    if(rects.size()!=3)
        return show;

    double h0 = rects.at(0).height;
    double h1 = rects.at(1).height;
    double h2 = rects.at(2).height;

    double w0 = rects.at(0).width;
    double w1 = rects.at(1).width;
    double w2 = rects.at(2).width;

    if (h1==0 || h2==0 || w1==0 || w2==0)
        return show;

    if(     abs(1.0 - h0/h1)<=0.35 &&
            abs(1.0 - h0/h2)<=0.35 &&
            abs(1.0 - w0/w1)<=0.5 &&
            abs(1.0 - w0/w2)<=0.5)
    {

        for(int i=0; i<3; i++)
        {
            Rect rect = rects.at(i);

            Mat part_origin = show(rect);///вырезать выделенный прямоугольник из кадра
            Mat part_tmpl;

            int RES = 0;
            int num = 0;

            /*double d1 = part_origin.rows;
            double d2 = part_origin.cols;
            double dor = d1/d2;
            qDebug()<<i << "___" <<dor;*/

            for(int j = 0; j<6; j++)///проверка шести шаблонов
            {
                //
                Mat part;
                part_tmpl = CV_TextDetect::tmpl_arr[j+NUMS.size()*6];

                //qDebug()<<i<<"  "<<i+NUMS.size()*6 << "___" << part_origin.rows/part_origin.cols <<" __ " << part_tmpl.rows/part_tmpl.cols;


                double d1 = part_origin.rows;
                double d2 = part_origin.cols;
                double d01 = part_tmpl.rows;
                double d02 = part_tmpl.cols;
                if(d1==0 || d2==0)
                    break;
                double dor = d1/d2;
                double dtm = d01/d02;

                //qDebug()<<i<<"  "<<i+NUMS.size()*6 << "___" <<dor<<"  "<<dtm;


                if(abs(dor-dtm)>0.3)
                    continue;

                /*if( != )
                    continue;*/

                resize(part_origin, part, part_tmpl.size());

                int res = 0;

                for(int rows = 0; rows<part_tmpl.rows; rows++)
                {
                    for(int cols = 0; cols<part_tmpl.cols; cols++)
                    {
                        if(part.at<Vec3b>(rows, cols)[0]==0
                                && part.at<Vec3b>(rows, cols)[1]==255
                                && part.at<Vec3b>(rows, cols)[2]==0
                                && part_tmpl.at<Vec3b>(rows, cols)[0]==0
                                && part_tmpl.at<Vec3b>(rows, cols)[1]==0
                                && part_tmpl.at<Vec3b>(rows, cols)[2]==0
                                )
                            res++;
                    }
                }

                if (RES<res)///выбор наибольшего соответствия
                {
                    RES=res;
                    num = j;
                }
            }

            NUMS.append(num);



            drawContours( show, contours, rectpos.at(i),  CV_RGB(255, 0, 0), 2, LINE_8, -1, 0 );
            rectangle(show, rect, CV_RGB(0, 255, 0), 3, 8, 0);
            int x = rect.x + rect.width/2;
            int y = rect.y + rect.height/2;

            putText(show, "+",   Point2i(x,y), CV_FONT_HERSHEY_COMPLEX, 1, Scalar(255, 0, 0), 2);

            if (NUMS.size()==3)
            {
                qDebug()<<"  "<<NUMS.at(0)+1<<" "<<NUMS.at(1)+1<<" "<<NUMS.at(2)+1;
                show = CV_TextDetect::PrintAns(show, NUMS);

                break;
            }
        }
    }

    cvtColor(show, show, CV_RGB2BGR);
    return show;
}

Mat CV_TextDetect::PrintAns(Mat show, QList<int> NUMS)
{
    if(NUMS.at(1)==4 || NUMS.at(2)==4)
    {
        CV_Detect::TYPE = TYPE_TEXT;
        putText(show, "E",  Point2i(50,100), CV_FONT_HERSHEY_COMPLEX, 3, cv::Scalar(255, 255, 0), 2);
    }

    else if(NUMS.at(1)==3 || (NUMS.at(0)==NUMS.at(2) && NUMS.at(0)==3))
    {
        CV_Detect::TYPE = TYPE_TEXT;
        putText(show, "D",  Point2i(50,100), CV_FONT_HERSHEY_COMPLEX, 3, cv::Scalar(255, 0, 0), 2);
    }

    else if((NUMS.at(0)==1 && NUMS.at(1)==1)
            || (NUMS.at(0)==1 && NUMS.at(2)==1)
            || (NUMS.at(1)==1 && NUMS.at(2)==1))
    {
        CV_Detect::TYPE = TYPE_TEXT;
        putText(show, "B",  Point2i(50,100), CV_FONT_HERSHEY_COMPLEX, 3, cv::Scalar(255, 255, 0), 2);
    }

    else if(NUMS.at(0)==5)
    {
        CV_Detect::TYPE = TYPE_TEXT;
        putText(show, "F",  Point2i(50,100), CV_FONT_HERSHEY_COMPLEX, 3, cv::Scalar(0, 0, 255), 2);
    }

    else if(NUMS.at(1)==2)
    {
        CV_Detect::TYPE = TYPE_TEXT;
        putText(show, "C",  Point2i(50,100), CV_FONT_HERSHEY_COMPLEX, 3, cv::Scalar(0, 0, 255), 2);
    }

    else if(NUMS.at(0)==NUMS.at(1) && NUMS.at(0)==NUMS.at(2) && NUMS.at(0)==0)
    {
        CV_Detect::TYPE = TYPE_TEXT;
        putText(show, "A",  Point2i(50,100), CV_FONT_HERSHEY_COMPLEX, 3, cv::Scalar(255, 0, 0), 2);
    }

    /*if((NUMS.at(0)==NUMS.at(1))
            || (NUMS.at(0)&&NUMS.at(2))
            || (NUMS.at(1)&&NUMS.at(2)))
        CV_Detect::TYPE = TYPE_TEXT;



    if((NUMS.at(0)==0 && NUMS.at(1)==0)
            || (NUMS.at(0)==0 && NUMS.at(2)==0)
            || (NUMS.at(1)==0 && NUMS.at(2)==0))
        putText(show, "A",  Point2i(50,100), CV_FONT_HERSHEY_COMPLEX, 3, cv::Scalar(255, 0, 0), 2);

    else if((NUMS.at(0)==1 && NUMS.at(1)==1)
            || (NUMS.at(0)==1 && NUMS.at(2)==1)
            || (NUMS.at(1)==1 && NUMS.at(2)==1))
        putText(show, "B",  Point2i(50,100), CV_FONT_HERSHEY_COMPLEX, 3, cv::Scalar(255, 255, 0), 2);

    else if((NUMS.at(0)==2 && NUMS.at(1)==2)
            || (NUMS.at(0)==2 && NUMS.at(2)==2)
            || (NUMS.at(1)==2 && NUMS.at(2)==2))
        putText(show, "C",  Point2i(50,100), CV_FONT_HERSHEY_COMPLEX, 3, cv::Scalar(0, 0, 255), 2);


    else if((NUMS.at(1)==3) || (NUMS.at(0)==3 && NUMS.at(1)==3)
            || (NUMS.at(0)==3 && NUMS.at(2)==3)
            || (NUMS.at(1)==3 && NUMS.at(2)==3))
        putText(show, "D",  Point2i(50,100), CV_FONT_HERSHEY_COMPLEX, 3, cv::Scalar(255, 0, 0), 2);

    else if((NUMS.at(1)==4) || (NUMS.at(0)==4 && NUMS.at(1)==4)
            || (NUMS.at(0)==4 && NUMS.at(2)==4)
            || (NUMS.at(1)==4 && NUMS.at(2)==4))
        putText(show, "E",  Point2i(50,100), CV_FONT_HERSHEY_COMPLEX, 3, cv::Scalar(255, 255, 0), 2);

    else if((NUMS.at(0)==5 && NUMS.at(1)==5)
            || (NUMS.at(0)==5 && NUMS.at(2)==5)
            || (NUMS.at(1)==5 && NUMS.at(2)==5))
        putText(show, "F",  Point2i(50,100), CV_FONT_HERSHEY_COMPLEX, 3, cv::Scalar(0, 0, 255), 2);*/

    return show;
}
