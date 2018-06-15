#include "new_neuralnet.h"

NEW_neuralNet::NEW_neuralNet()
{

}

QList<RECT> NEW_neuralNet::GetRects(Mat show)
{
    QList<RECT> list;

    vector<vector<Point>> contours;
    Mat tmp(show.clone());
    cvtColor(tmp, tmp, CV_RGB2GRAY);
    findContours(tmp, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);

    for(unsigned int i=0;i<contours.size();i++)
    {
        if(contourArea(contours[i])>500)
        {
            if(list.size()==3)
                break;

            Rect rect = boundingRect(contours[i]);

            list << RECT(rect, i);
        }
    }

    if(list.size()!=3)
        return list;

    for(int i=0;i<3; i++)
    {
        for(int j=0; j<3; j++)
        {
            if(i<j && list[i].rect.x>list[j].rect.x)
            {
                RECT tmpr = list.at(i);
                list[i] = list.at(j);
                list[j] = tmpr;
            }
        }
    }

    return list;
}

bool NEW_neuralNet::PropertiesAreValid(QList<RECT> rectList)
{
    int d01 = abs(rectList[0].rect.y - rectList[1].rect.y);  //проверка по координате
    int d12 = abs(rectList[1].rect.y - rectList[2].rect.y);

    if(!(d01<=10 && d12<=10))
        return false;

    int h01 = abs(rectList[0].rect.height - rectList[1].rect.height);  //проверка по высоте
    int h12 = abs(rectList[1].rect.height - rectList[2].rect.height);

    if(!(h01<=10 && h12<=10))
        return false;

    /*double r0 = 100*(double(rectList[0].rect.width) / double(rectList[0].rect.height));  //проверка по конфигурации
    double r1 = 100*(double(rectList[1].rect.width) / double(rectList[1].rect.height));
    double r2 = 100*(double(rectList[2].rect.width) / double(rectList[2].rect.height));

    if(!(r0>25.00 && r0<37.00
            && r1>14.00 && r1<53.00
            && r2>29.00 && r2<38.00))
        return false;

    qDebug() << r0 << "   " << r1 << "    " << r2;*/
    return true;
}

Mat NEW_neuralNet::Detect(Mat frame, CV_SettingsWidget *W)
{
    frame = frame(Rect(frame.cols*0.05, 0, frame.cols*0.67, frame.rows*0.75));
    cvtColor(frame, frame, CV_BGR2RGB);
    Mat showBlack = CV_Detect::ColorFilter(frame, W, 0);

    Mat show;

    if(CV_Detect::COLOR == BLACK_)
        show = showBlack;
    else
        return frame;

    QList<RECT> rectList = NEW_neuralNet::GetRects(show);  //получаем области букв

    if(rectList.size()!=3)    //если их не три, то возвращаем необработанный кадр
        return frame;

    if(!NEW_neuralNet::PropertiesAreValid(rectList)) //проверяем конфигурацию
        return frame;

    //тут уже точно имеем три области нужной конфигурации

    double MAX_VAL_1 = 0;  //тут будет максимальное соответствие
    double MAX_VAL_2 = 0;
    double MAX_VAL_3 = 0;

    int FINAL_1 = -1;  //тут будет номер соответствия
    int FINAL_2 = -1;
    int FINAL_3 = -1;

    for(int i=0; i<6; i++)  //цикл по 6 хвостам из массива tmpl_arr
    {
        QList<QList<int>> ref_1 = NEW_neuralNet::tmpl_arr.at(3*i);
        QList<QList<int>> ref_2 = NEW_neuralNet::tmpl_arr.at(3*i + 1);
        QList<QList<int>> ref_3 = NEW_neuralNet::tmpl_arr.at(3*i + 2);

        double r1 = 100*(double(ref_1.at(0).size()) / double(ref_1.size()));  //конфигурация шаблонов
        double r2 = 100*(double(ref_2.at(0).size()) / double(ref_2.size()));
        double r3 = 100*(double(ref_3.at(0).size()) / double(ref_3.size()));

        double prop_1 = 100*(double(rectList[0].rect.width) / double(rectList[0].rect.height));  //конфигурация найденных букв
        double prop_2 = 100*(double(rectList[1].rect.width) / double(rectList[1].rect.height));
        double prop_3 = 100*(double(rectList[2].rect.width) / double(rectList[2].rect.height));

        double d1 = abs(prop_1-r1);
        double d2 = abs(prop_2-r2);
        double d3 = abs(prop_3-r3);

        //if(!(d1<10 && d2<10 && d3<10))
          //  continue;

        //тут точно знаем, что шаблоны совместимы, т.е. 1 с W сравнивать не будем

        Mat letter_1 = show(rectList.at(0).rect);  //вырезаем буквы
        Mat letter_2 = show(rectList.at(1).rect);
        Mat letter_3 = show(rectList.at(2).rect);

        resize(letter_1, letter_1, Size(ref_1.at(0).size(), ref_1.size()));  //приводим к размеру шаблонов
        resize(letter_2, letter_2, Size(ref_2.at(0).size(), ref_2.size()));
        resize(letter_3, letter_3, Size(ref_3.at(0).size(), ref_3.size()));

        int res_1 = 0;

        for(int rows = 0; rows<letter_1.rows; rows++)
        {
            for(int cols = 0; cols<letter_1.cols; cols++)
            {
                if((letter_1.at<Vec3b>(rows, cols)[0]==0
                        && letter_1.at<Vec3b>(rows, cols)[1]==255
                        && letter_1.at<Vec3b>(rows, cols)[2]==0
                        && ref_1.at(rows).at(cols)==1)
                        ||
                        (letter_1.at<Vec3b>(rows, cols)[0]==0
                        && letter_1.at<Vec3b>(rows, cols)[1]==0
                        && letter_1.at<Vec3b>(rows, cols)[2]==0
                        && ref_1.at(rows).at(cols)==0)
                        )
                    res_1++;
            }
        }

        int res_2 = 0;

        for(int rows = 0; rows<letter_2.rows; rows++)
        {
            for(int cols = 0; cols<letter_2.cols; cols++)
            {
                if((letter_2.at<Vec3b>(rows, cols)[0]==0
                        && letter_2.at<Vec3b>(rows, cols)[1]==255
                        && letter_2.at<Vec3b>(rows, cols)[2]==0
                        && ref_2.at(rows).at(cols)==1)
                        ||
                        (letter_2.at<Vec3b>(rows, cols)[0]==0
                        && letter_2.at<Vec3b>(rows, cols)[1]==0
                        && letter_2.at<Vec3b>(rows, cols)[2]==0
                        && ref_2.at(rows).at(cols)==0)
                        )
                    res_2++;
            }
        }

        int res_3 = 0;

        for(int rows = 0; rows<letter_3.rows; rows++)
        {
            for(int cols = 0; cols<letter_3.cols; cols++)
            {
                if((letter_3.at<Vec3b>(rows, cols)[0]==0
                        && letter_3.at<Vec3b>(rows, cols)[1]==255
                        && letter_3.at<Vec3b>(rows, cols)[2]==0
                        && ref_3.at(rows).at(cols)==1)
                        ||
                        (letter_3.at<Vec3b>(rows, cols)[0]==0
                        && letter_3.at<Vec3b>(rows, cols)[1]==0
                        && letter_3.at<Vec3b>(rows, cols)[2]==0
                        && ref_3.at(rows).at(cols)==0)
                        )
                    res_3++;
            }
        }

        double S1 = 100 * double(res_1) / double(letter_1.rows * letter_1.cols);
        double S2 = 100 * double(res_2) / double(letter_2.rows * letter_2.cols);
        double S3 = 100 * double(res_3) / double(letter_3.rows * letter_3.cols);

        if(S1>MAX_VAL_1)
        {
            MAX_VAL_1 = S1;
            FINAL_1 = i+1;
        }

        if(S2>MAX_VAL_2)
        {
            MAX_VAL_2 = S2;
            FINAL_2 = i+1;
        }

        if(S3>MAX_VAL_3)
        {
            MAX_VAL_3 = S3;
            FINAL_3 = i+1;
        }

        qDebug() << i+1 << ":        " << S1 << "   " << S2 << "   " << S3;
    }

    qDebug() << FINAL_1 << "  " << FINAL_2 << "  " << FINAL_3;

    Mat resultMat = NEW_neuralNet::PrintAnswer(show, FINAL_1, FINAL_2, FINAL_3);

    cv::cvtColor(resultMat, resultMat, CV_RGB2BGR);

    return resultMat;
}

Mat NEW_neuralNet::PrintAnswer(Mat show, int FINAL_1, int FINAL_2, int FINAL_3)
{
    if(FINAL_1==1 && FINAL_2==1 && FINAL_3==1)  //A: UH8
    {
        CV_Detect::TYPE = TYPE_TEXT;
        putText(show, "A",   Point2i(50, 100), CV_FONT_HERSHEY_COMPLEX, 3, cv::Scalar(255, 0, 0), 2);
    }

    if(FINAL_1==2 && FINAL_2==2 && FINAL_3==2)  //B: L6R
    {
        CV_Detect::TYPE = TYPE_TEXT;
        putText(show, "B",   Point2i(50, 100), CV_FONT_HERSHEY_COMPLEX, 3, cv::Scalar(255, 255, 0), 2);
    }

    if(FINAL_1==3 && FINAL_2==3 && FINAL_3==3)  //C: G7C
    {
        CV_Detect::TYPE = TYPE_TEXT;
        putText(show, "C",   Point2i(50, 100), CV_FONT_HERSHEY_COMPLEX, 3, cv::Scalar(0, 0, 255), 2);
    }

    if(FINAL_1==4 && FINAL_2==4 && FINAL_3==4)  //D: S1P
    {
        CV_Detect::TYPE = TYPE_TEXT;
        putText(show, "D",   Point2i(50, 100), CV_FONT_HERSHEY_COMPLEX, 3, cv::Scalar(255, 0, 0), 2);
    }

    if(FINAL_1==5 && FINAL_2==5 && FINAL_3==5)  //E: JW3
    {
        CV_Detect::TYPE = TYPE_TEXT;
        putText(show, "E",   Point2i(50, 100), CV_FONT_HERSHEY_COMPLEX, 3, cv::Scalar(255, 255, 0), 2);
    }

    if(FINAL_1==6 && FINAL_2==6 && FINAL_3==6)  //F: A2X
    {
        CV_Detect::TYPE = TYPE_TEXT;
        putText(show, "F",   Point2i(50, 100), CV_FONT_HERSHEY_COMPLEX, 3, cv::Scalar(0, 0, 255), 2);
    }

    return show;
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

    QList<RECT> rects;

    for(unsigned int i=0;i<contours.size();i++)
    {
        if(contourArea(contours[i])>500)
        {
            if(rects.size()==3)
                break;

            Rect rect = boundingRect(contours[i]);
            rects << RECT(rect, i);
        }
    }

    if(rects.size()!=3)
        return;

    for(int i=0;i<3; i++)
    {
        for(int j=0; j<3; j++)
        {
            if(i<j && rects[i].rect.x>rects[j].rect.x)
            {
                RECT tmpr = rects.at(i);
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
        Rect rect = rects.at(i).rect;

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
