#include "new_neuralnet.h"

NEW_neuralNet::NEW_neuralNet()
{

}


void NEW_neuralNet::Generate(Mat frame, CV_SettingsWidget *W)
{
    frame = frame(Rect(frame.cols*0.05, 0, frame.cols*0.67, frame.rows*0.75));  //отрезаем края кадра
    cvtColor(frame, frame, CV_BGR2RGB);
    Mat show = CV_Detect::ColorFilter(frame, W, 0);  //выделяем чёрный цвет

    vector<vector<Point>> contours;
    Mat tmp(show.clone());
    cvtColor(tmp, tmp, CV_RGB2GRAY);
    findContours(tmp, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);

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
        return;

    for(int i=0;i<3; i++)
    {
        for(int j=0; j<3; j++)
        {
            if(i<j && rects.at(i).x>rects.at(j).x)
            {
                Rect tmpr = rects.at(i);
                rects[i] = rects.at(j);
                rects[j] = tmpr;
            }
        }
    }


    QFile file(QCoreApplication::applicationDirPath()+"/res/" + QString::number(W->ChoosenTmpl()) + ".txt");
    if (!file.open(QIODevice::WriteOnly))
        return;
    QTextStream out(&file);

    out << W->ChoosenTmpl() << "\r\n";

    for(int i=0; i<3; i++)
    {
        Rect rect = rects.at(i);

        Mat part_origin = show(rect);///вырезать выделенный прямоугольник из кадра



        Mat resized; //новая матрица для измененного фрейма


        Size size;
        size.height = 14;
        size.width = part_origin.cols * 14 / part_origin.rows; //задаем новый размер (мелкий)

        resize(part_origin, resized, size);  //изменяем размер фрейма (уменьшаем)

        out << resized.cols << " " << resized.rows << "\r\n";

    //int OUT[resized.cols][resized.rows] = {0};



        int t;



        for(int rw = 0; rw<resized.rows; rw++)   //циклы по всем пикселям новой матрицы
        {
            for(int cl = 0; cl<resized.cols; cl++)
            {
                if(resized.at<Vec3b>(rw, cl)[0]==0
                        && resized.at<Vec3b>(rw, cl)[1]==255
                        && resized.at<Vec3b>(rw, cl)[2]==0
                        )
                    t = 1;
                else t = 0;


                out << t << " ";
            }

            out << "\r\n";
        }
    }

}
