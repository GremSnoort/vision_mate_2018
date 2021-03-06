#ifndef CV_DATATYPES_H
#define CV_DATATYPES_H

#include <opencv2/opencv.hpp>
#include <QObject>
#include <stdexcept>

struct RECT
{
    cv::Rect rect;
    int rectpos;

    RECT(cv::Rect r = cv::Rect(0, 0, 0, 0), int rp = 0)
    {
        rect = r;
        rectpos = rp;
    }
};

enum type
{
    TYPE_TEXT,
    TYPE_FIGURE
};

enum figureColor
{
    RED_ = 1,
    YELLOW_ = 2,
    BLUE_ = 3,
    BLACK_ = 0
};

struct color
{
    int top;
    int bottom;

    color(int b = 0, int t = 255)
    {
        bottom = b;
        top = t;
    }

    bool operator ==(color ref)
    {
        if(this->bottom>=ref.bottom
               && this->top<=ref.top
               && this->bottom<=this->top)
            return true;
        return false;
    }
};

struct default_color
{
    color R;
    color G;
    color B;

    default_color(color r = color(), color g = color(), color b = color())
    {
        R = r;
        G = g;
        B = b;
    }

    bool operator ==(default_color ref)
    {
        if(this->R==ref.R
               && this->G==ref.G
               && this->B==ref.B)
            return true;
        return false;
    }
};
struct on_off
{
    bool red;
    bool yellow;
    bool blue;
    bool black;

    on_off(bool r = true, bool y = true, bool bl = true, bool blk = true)
    {
        red = r;
        yellow = y;
        blue = bl;
        black = blk;
    }

    bool &operator [] ( int i )
    {
       if(i==0)
           return black;

       else if(i==1)
           return red;

       else if(i==2)
           return yellow;

       else if(i==3)
           return blue;
       else
           throw std::invalid_argument("Incorrect index");
    }
};



Q_DECLARE_METATYPE(RECT)
Q_DECLARE_METATYPE(type)
Q_DECLARE_METATYPE(figureColor)
Q_DECLARE_METATYPE(color)
Q_DECLARE_METATYPE(default_color)
Q_DECLARE_METATYPE(on_off)


#endif // CV_DATATYPES_H
