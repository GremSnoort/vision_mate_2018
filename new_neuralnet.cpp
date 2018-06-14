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

    double r0 = 100*(double(rectList[0].rect.width) / double(rectList[0].rect.height));  //проверка по конфигурации
    double r1 = 100*(double(rectList[1].rect.width) / double(rectList[1].rect.height));
    double r2 = 100*(double(rectList[2].rect.width) / double(rectList[2].rect.height));

    if(!(r0>25.00 && r0<37.00
            && r1>14.00 && r1<53.00
            && r2>29.00 && r2<38.00))
        return false;

    //qDebug() << r0 << "   " << r1 << "    " << r2;
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



return frame;

/*
















    if(abs(rects.at(0).y-rects.at(1).y)>4 || abs(rects.at(1).y-rects.at(2).y)>4)
        return frame;

    double h0 = rects.at(0).height;
    double h1 = rects.at(1).height;
    double h2 = rects.at(2).height;

    double w0 = rects.at(0).width;
    double w1 = rects.at(1).width;
    double w2 = rects.at(2).width;

    if (h1==0 || h2==0 || w1==0 || w2==0)
        return frame;

    if(     abs(1.0 - h0/h1)<=0.35 &&
            abs(1.0 - h0/h2)<=0.35
            )
    {

        for(int i=0; i<3; i++)
        {
            Rect rect = rects.at(i);

            Mat part_origin = show(rect);///вырезать выделенный прямоугольник из кадра
            Mat part_tmpl;

            int RES = 0;
            int num = 0;


            for(int j = 0; j<6; j++)///проверка шести шаблонов
            {
                Mat part;
                part_tmpl = CV_TextDetect::tmpl_arr[j+i*6];

                double d1 = part_origin.rows;
                double d2 = part_origin.cols;
                double d01 = part_tmpl.rows;
                double d02 = part_tmpl.cols;
                if(d2==0 || d02==0)
                    break;
                double dor = d1/d2;
                double dtm = d01/d02;

                if(dor>4.5)
                {
                    num = 3;
                    break;
                }

                if(dor<2.0)
                {
                    num = 4;
                    break;
                }

                if(abs(dor-dtm)>0.3)
                    continue;

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
    return show;*/
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

                int tmpp = rectpos.at(i);
                rectpos[i] = rectpos.at(j);
                rectpos[j] = tmpp;
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
