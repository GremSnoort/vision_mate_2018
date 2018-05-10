#ifndef CV_DATATYPES_H
#define CV_DATATYPES_H

#include <QObject>

enum type
{
    TYPE_TEXT,
    TYPE_FIGURE
};

Q_DECLARE_METATYPE(type)

enum figureColor
{
    RED_ = 1,
    YELLOW_ = 2,
    BLUE_ = 3,
    BLACK_ = 0
};

Q_DECLARE_METATYPE(figureColor)

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

Q_DECLARE_METATYPE(color)

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

Q_DECLARE_METATYPE(default_color)

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

       if(i==1)
           return red;

       if(i==2)
           return yellow;

       if(i==3)
           return blue;
    }
};

Q_DECLARE_METATYPE(on_off)

#endif // CV_DATATYPES_H
