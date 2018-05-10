#ifndef CV_DATATYPES_H
#define CV_DATATYPES_H

#include <QObject>

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

Q_DECLARE_METATYPE(figureColor)
Q_DECLARE_METATYPE(type)
Q_DECLARE_METATYPE(color)
Q_DECLARE_METATYPE(default_color)

#endif // CV_DATATYPES_H
