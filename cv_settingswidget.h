#ifndef CV_SETTINGSWIDGET_H
#define CV_SETTINGSWIDGET_H

#include <QObject>
#include <QWidget>
#include <QScopedPointer>
#include <QSlider>
#include <QLabel>
#include <QGridLayout>
#include <QPushButton>
#include <QCoreApplication>
#include <QComboBox>
#include <QPainter>
#include <QFile>
#include <QTextStream>

#include "cv_datatypes.h"

class CV_SettingsWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CV_SettingsWidget(QWidget *parent = nullptr);

    void CreateLayout();    

    default_color defolt_RED;//= defolt_color(color(120, 255), color(0, 65), color(0, 65));
    default_color defolt_YELLOW;//= defolt_color(color(150, 255), color(150, 255), color(0, 100));
    default_color defolt_BLUE;//= defolt_color(color(0, 79), color(0, 118), color(110, 255));
    default_color defolt_BLACK;

    QScopedPointer<QLabel> l_RedChannelTOP;
    QScopedPointer<QLabel> l_GreenChannelTOP;
    QScopedPointer<QLabel> l_BlueChannelTOP;
    QScopedPointer<QLabel> l_RedChannelBOTTOM;
    QScopedPointer<QLabel> l_GreenChannelBOTTOM;
    QScopedPointer<QLabel> l_BlueChannelBOTTOM;

    QScopedPointer<QSlider> s_RedChannelTOP;
    QScopedPointer<QSlider> s_GreenChannelTOP;
    QScopedPointer<QSlider> s_BlueChannelTOP;
    QScopedPointer<QSlider> s_RedChannelBOTTOM;
    QScopedPointer<QSlider> s_GreenChannelBOTTOM;
    QScopedPointer<QSlider> s_BlueChannelBOTTOM;

    QScopedPointer<QPushButton> b_showDefoltRED;
    QScopedPointer<QPushButton> b_showDefoltYELLOW;
    QScopedPointer<QPushButton> b_showDefoltBLUE;
    QScopedPointer<QPushButton> b_showDefoltBLACK;

    QScopedPointer<QPushButton> b_setDefoltRED;
    QScopedPointer<QPushButton> b_setDefoltYELLOW;
    QScopedPointer<QPushButton> b_setDefoltBLUE;
    QScopedPointer<QPushButton> b_setDefoltBLACK;

    QScopedPointer<QLabel> l_debug;
    QScopedPointer<QComboBox> cb_debugColor;

    QScopedPointer<QList<default_color*>> defolt;

    int R_top()
    {
        return this->s_RedChannelTOP.data()->value();
    }

    int G_top()
    {
        return this->s_GreenChannelTOP.data()->value();
    }

    int B_top()
    {
        return this->s_BlueChannelTOP.data()->value();
    }

    int R_bottom()
    {
        return this->s_RedChannelBOTTOM.data()->value();
    }

    int G_bottom()
    {
        return this->s_GreenChannelBOTTOM.data()->value();
    }

    int B_bottom()
    {
        return this->s_BlueChannelBOTTOM.data()->value();
    }

    default_color GetCurrentColor();

    void readColors();

    void updateColors();

signals:

public slots:
    void SetDefoltRED();
    void SetDefoltYELLOW();
    void SetDefoltBLUE();
    void SetDefoltBLACK();

    void ShowRED();
    void ShowYELLOW();
    void ShowBLUE();
    void ShowBLACK();

    void cb_answer(int index);
};

#endif // CV_SETTINGSWIDGET_H
