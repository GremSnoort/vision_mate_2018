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
#include <QCheckBox>
#include <QFileDialog>
#include <QRadioButton>

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

    QScopedPointer<QCheckBox> b_onRED;
    QScopedPointer<QCheckBox> b_onYELLOW;
    QScopedPointer<QCheckBox> b_onBLUE;
    QScopedPointer<QCheckBox> b_onBLACK;

    QScopedPointer<QPushButton> b_save_config;



    ///_________________________________________________________________


    QScopedPointer<QRadioButton> UH8;
    QScopedPointer<QRadioButton> L6R;
    QScopedPointer<QRadioButton> G7C;
    QScopedPointer<QRadioButton> S1P;
    QScopedPointer<QRadioButton> JW3;
    QScopedPointer<QRadioButton> A2X;

    QScopedPointer<QPushButton> Generate;


    ///_________________________________________________________________


    QScopedPointer<QLabel> l_debug;
    QScopedPointer<QComboBox> cb_debugColor;

    QScopedPointer<QList<default_color*>> defolt;

    on_off On_Off;

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

    int ChoosenTmpl()
    {
        if(UH8->isChecked())
            return 1;
        if(L6R->isChecked())
            return 2;
        if(G7C->isChecked())
            return 3;
        if(S1P->isChecked())
            return 4;
        if(JW3->isChecked())
            return 5;
        if(A2X->isChecked())
            return 6;
    }

    default_color GetCurrentColor();

    void readColors();

    void updateColors(QString filename);

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

    void save_cfg();

    ///_____________________________________________________________________________

    void OnRED(int state)
    {
        On_Off.red = bool(state);
    }

    void OnYELLOW(int state)
    {
        On_Off.yellow = bool(state);
    }

    void OnBLUE(int state)
    {
        On_Off.blue = bool(state);
    }

    void OnBLACK(int state)
    {
        On_Off.black = bool(state);
    }



    ///_____________________________________________________________________________
};

#endif // CV_SETTINGSWIDGET_H
