#include "cv_settingswidget.h"

CV_SettingsWidget::CV_SettingsWidget(QWidget *parent)
    : QWidget(parent)
    , l_RedChannelTOP(new QLabel("RED_top: ", this))
    , l_GreenChannelTOP(new QLabel("GREEN_top: ", this))
    , l_BlueChannelTOP(new QLabel("BLUE_top: ", this))
    , l_RedChannelBOTTOM(new QLabel("RED_bottom: ", this))
    , l_GreenChannelBOTTOM(new QLabel("GREEN_bottom: ", this))
    , l_BlueChannelBOTTOM(new QLabel("BLUE_bottom: ", this))

    , s_RedChannelTOP(new QSlider(Qt::Horizontal, this))
    , s_GreenChannelTOP(new QSlider(Qt::Horizontal, this))
    , s_BlueChannelTOP(new QSlider(Qt::Horizontal, this))
    , s_RedChannelBOTTOM(new QSlider(Qt::Horizontal, this))
    , s_GreenChannelBOTTOM(new QSlider(Qt::Horizontal, this))
    , s_BlueChannelBOTTOM(new QSlider(Qt::Horizontal, this))

    , b_showDefoltRED(new QPushButton("Show RED", this))
    , b_showDefoltYELLOW(new QPushButton("Show YELLOW", this))
    , b_showDefoltBLUE(new QPushButton("Show BLUE", this))
    , b_showDefoltBLACK(new QPushButton("Show BLACK", this))

    , b_setDefoltRED(new QPushButton("Set default RED", this))
    , b_setDefoltYELLOW(new QPushButton("Set default YELLOW", this))
    , b_setDefoltBLUE(new QPushButton("Set default BLUE", this))
    , b_setDefoltBLACK(new QPushButton("Set default BLACK", this))

    , b_onRED(new QCheckBox("ON", this))
    , b_onYELLOW(new QCheckBox("ON", this))
    , b_onBLUE(new QCheckBox("ON", this))
    , b_onBLACK(new QCheckBox("ON", this))

    , b_save_config(new QPushButton("SAVE .cfg", this))


    , UH8(new QRadioButton("UH8", this))
    , L6R(new QRadioButton("L6R", this))
    , G7C(new QRadioButton("G7C", this))
    , S1P(new QRadioButton("S1P", this))
    , JW3(new QRadioButton("JW3", this))
    , A2X(new QRadioButton("A2X", this))

    , Generate(new QPushButton("Generate", this))


    , l_debug(new QLabel("for debug mode: ", this))
    , cb_debugColor(new QComboBox(this))

    , defolt(new QList<default_color*>())
    , On_Off(on_off())
{    
    setMinimumSize(640, 320);

    /*defolt_RED = default_color(color(120, 255), color(0, 65), color(0, 65));
    defolt_YELLOW = default_color(color(150, 255), color(150, 255), color(0, 100));
    defolt_BLUE = default_color(color(0, 79), color(0, 118), color(110, 255));
    defolt_BLACK = default_color(color(0, 25), color(0, 25), color(0, 25));*/

    readColors();

    defolt.data()->append(&defolt_BLACK);
    defolt.data()->append(&defolt_RED);
    defolt.data()->append(&defolt_YELLOW);
    defolt.data()->append(&defolt_BLUE);

    connect(b_showDefoltRED.data(), &QPushButton::released, this, &CV_SettingsWidget::ShowRED);
    connect(b_showDefoltYELLOW.data(), &QPushButton::released, this, &CV_SettingsWidget::ShowYELLOW);
    connect(b_showDefoltBLUE.data(), &QPushButton::released, this, &CV_SettingsWidget::ShowBLUE);
    connect(b_showDefoltBLACK.data(), &QPushButton::released, this, &CV_SettingsWidget::ShowBLACK);

    connect(b_setDefoltRED.data(), &QPushButton::released, this, &CV_SettingsWidget::SetDefoltRED);
    connect(b_setDefoltYELLOW.data(), &QPushButton::released, this, &CV_SettingsWidget::SetDefoltYELLOW);
    connect(b_setDefoltBLUE.data(), &QPushButton::released, this, &CV_SettingsWidget::SetDefoltBLUE);
    connect(b_setDefoltBLACK.data(), &QPushButton::released, this, &CV_SettingsWidget::SetDefoltBLACK);

    ///__________________________

    connect(b_onRED.data(), &QCheckBox::stateChanged, this, &CV_SettingsWidget::OnRED);
    connect(b_onYELLOW.data(), &QCheckBox::stateChanged, this, &CV_SettingsWidget::OnYELLOW);
    connect(b_onBLUE.data(), &QCheckBox::stateChanged, this, &CV_SettingsWidget::OnBLUE);
    connect(b_onBLACK.data(), &QCheckBox::stateChanged, this, &CV_SettingsWidget::OnBLACK);

    ///__________________________

    connect(cb_debugColor.data(), SIGNAL(currentIndexChanged(int)), this, SLOT(cb_answer(int)));

    connect(b_save_config.data(), &QPushButton::released, this, &CV_SettingsWidget::save_cfg);

    CreateLayout();

    setWindowIcon(QIcon(QCoreApplication::applicationDirPath()+"/res/settings.jpg"));

    setWindowTitle("SETTINGS");
}

void CV_SettingsWidget::readColors()
{
    defolt_RED = default_color(color(0, 255), color(0, 255), color(0, 255));
    defolt_YELLOW = default_color(color(0, 255), color(0, 255), color(0, 255));
    defolt_BLUE = default_color(color(0, 255), color(0, 255), color(0, 255));
    defolt_BLACK = default_color(color(0, 255), color(0, 255), color(0, 255));

    QFile file(QCoreApplication::applicationDirPath()+"/res/colors.cfg");
    QByteArray data;
    if (!file.open(QIODevice::ReadOnly))
        return;
    data = file.readAll();
    QList<QByteArray> fields = data.split('\r\n');
    if(fields.size()>=4)
        for(int i=0; i<fields.size(); i++)
        {
            QList<QByteArray> values = fields.at(i).split(' ');

            /*qDebug() << values.at(0).toInt()
                     << " " << values.at(1).toInt()
                     << " " << values.at(2).toInt()
                     << " " << values.at(3).toInt()
                     << " " << values.at(4).toInt()
                     << " " << values.at(5).toInt();*/

            if(values.size()==6)
            {
                if(i==0)
                    defolt_RED = default_color(color(values.at(0).toInt(), values.at(1).toInt()), color(values.at(2).toInt(), values.at(3).toInt()), color(values.at(4).toInt(), values.at(5).toInt()));

                if(i==1)
                    defolt_YELLOW = default_color(color(values.at(0).toInt(), values.at(1).toInt()), color(values.at(2).toInt(), values.at(3).toInt()), color(values.at(4).toInt(), values.at(5).toInt()));

                if(i==2)
                    defolt_BLUE = default_color(color(values.at(0).toInt(), values.at(1).toInt()), color(values.at(2).toInt(), values.at(3).toInt()), color(values.at(4).toInt(), values.at(5).toInt()));

                if(i==3)
                    defolt_BLACK = default_color(color(values.at(0).toInt(), values.at(1).toInt()), color(values.at(2).toInt(), values.at(3).toInt()), color(values.at(4).toInt(), values.at(5).toInt()));
            }
        }

}

void CV_SettingsWidget::updateColors(QString filename)
{
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly))
        return;
    QTextStream out(&file);
    out << defolt_RED.R.bottom
        << " "
        << defolt_RED.R.top
        << " "
        << defolt_RED.G.bottom
        << " "
        << defolt_RED.G.top
        << " "
        << defolt_RED.B.bottom
        << " "
        << defolt_RED.B.top
        << "\r\n";
    out << defolt_YELLOW.R.bottom
        << " "
        << defolt_YELLOW.R.top
        << " "
        << defolt_YELLOW.G.bottom
        << " "
        << defolt_YELLOW.G.top
        << " "
        << defolt_YELLOW.B.bottom
        << " "
        << defolt_YELLOW.B.top
        << "\r\n";
    out << defolt_BLUE.R.bottom
        << " "
        << defolt_BLUE.R.top
        << " "
        << defolt_BLUE.G.bottom
        << " "
        << defolt_BLUE.G.top
        << " "
        << defolt_BLUE.B.bottom
        << " "
        << defolt_BLUE.B.top
        << "\r\n";
    out << defolt_BLACK.R.bottom
        << " "
        << defolt_BLACK.R.top
        << " "
        << defolt_BLACK.G.bottom
        << " "
        << defolt_BLACK.G.top
        << " "
        << defolt_BLACK.B.bottom
        << " "
        << defolt_BLACK.B.top
        << "\r\n";
}

void CV_SettingsWidget::save_cfg()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Сохранить файл"), "colors_.cfg", tr("Cfg (*.cfg)"));

    updateColors(fileName);
}

void CV_SettingsWidget::cb_answer(int index)
{
    if(index==0)
        ShowRED();
    else if(index==1)
        ShowYELLOW();
    else if(index==2)
        ShowBLUE();
    else if(index==3)
        ShowBLACK();
}

default_color CV_SettingsWidget::GetCurrentColor()
{
    return default_color(color(R_bottom(), R_top()),
                        color(G_bottom(), G_top()),
                        color(B_bottom(), B_top()));
}

void CV_SettingsWidget::ShowRED()
{
    s_RedChannelTOP.data()->setValue(defolt_RED.R.top);
    s_GreenChannelTOP.data()->setValue(defolt_RED.G.top);
    s_BlueChannelTOP.data()->setValue(defolt_RED.B.top);

    s_RedChannelBOTTOM.data()->setValue(defolt_RED.R.bottom);
    s_GreenChannelBOTTOM.data()->setValue(defolt_RED.G.bottom);
    s_BlueChannelBOTTOM.data()->setValue(defolt_RED.B.bottom);
}

void CV_SettingsWidget::ShowYELLOW()
{
    s_RedChannelTOP.data()->setValue(defolt_YELLOW.R.top);
    s_GreenChannelTOP.data()->setValue(defolt_YELLOW.G.top);
    s_BlueChannelTOP.data()->setValue(defolt_YELLOW.B.top);

    s_RedChannelBOTTOM.data()->setValue(defolt_YELLOW.R.bottom);
    s_GreenChannelBOTTOM.data()->setValue(defolt_YELLOW.G.bottom);
    s_BlueChannelBOTTOM.data()->setValue(defolt_YELLOW.B.bottom);
}

void CV_SettingsWidget::ShowBLUE()
{
    s_RedChannelTOP.data()->setValue(defolt_BLUE.R.top);
    s_GreenChannelTOP.data()->setValue(defolt_BLUE.G.top);
    s_BlueChannelTOP.data()->setValue(defolt_BLUE.B.top);

    s_RedChannelBOTTOM.data()->setValue(defolt_BLUE.R.bottom);
    s_GreenChannelBOTTOM.data()->setValue(defolt_BLUE.G.bottom);
    s_BlueChannelBOTTOM.data()->setValue(defolt_BLUE.B.bottom);
}

void CV_SettingsWidget::ShowBLACK()
{
    s_RedChannelTOP.data()->setValue(defolt_BLACK.R.top);
    s_GreenChannelTOP.data()->setValue(defolt_BLACK.G.top);
    s_BlueChannelTOP.data()->setValue(defolt_BLACK.B.top);

    s_RedChannelBOTTOM.data()->setValue(defolt_BLACK.R.bottom);
    s_GreenChannelBOTTOM.data()->setValue(defolt_BLACK.G.bottom);
    s_BlueChannelBOTTOM.data()->setValue(defolt_BLACK.B.bottom);
}

void CV_SettingsWidget::SetDefoltRED()
{
    defolt_RED.R.top = s_RedChannelTOP.data()->value();
    defolt_RED.R.bottom = s_RedChannelBOTTOM.data()->value();

    defolt_RED.G.top = s_GreenChannelTOP.data()->value();
    defolt_RED.G.bottom = s_GreenChannelBOTTOM.data()->value();

    defolt_RED.B.top = s_BlueChannelTOP.data()->value();
    defolt_RED.B.bottom = s_BlueChannelBOTTOM.data()->value();

    updateColors(QCoreApplication::applicationDirPath()+"/res/colors.cfg");
}

void CV_SettingsWidget::SetDefoltYELLOW()
{
    defolt_YELLOW.R.top = s_RedChannelTOP.data()->value();
    defolt_YELLOW.R.bottom = s_RedChannelBOTTOM.data()->value();

    defolt_YELLOW.G.top = s_GreenChannelTOP.data()->value();
    defolt_YELLOW.G.bottom = s_GreenChannelBOTTOM.data()->value();

    defolt_YELLOW.B.top = s_BlueChannelTOP.data()->value();
    defolt_YELLOW.B.bottom = s_BlueChannelBOTTOM.data()->value();

    updateColors(QCoreApplication::applicationDirPath()+"/res/colors.cfg");
}

void CV_SettingsWidget::SetDefoltBLUE()
{
    defolt_BLUE.R.top = s_RedChannelTOP.data()->value();
    defolt_BLUE.R.bottom = s_RedChannelBOTTOM.data()->value();

    defolt_BLUE.G.top = s_GreenChannelTOP.data()->value();
    defolt_BLUE.G.bottom = s_GreenChannelBOTTOM.data()->value();

    defolt_BLUE.B.top = s_BlueChannelTOP.data()->value();
    defolt_BLUE.B.bottom = s_BlueChannelBOTTOM.data()->value();

    updateColors(QCoreApplication::applicationDirPath()+"/res/colors.cfg");
}

void CV_SettingsWidget::SetDefoltBLACK()
{
    defolt_BLACK.R.top = s_RedChannelTOP.data()->value();
    defolt_BLACK.R.bottom = s_RedChannelBOTTOM.data()->value();

    defolt_BLACK.G.top = s_GreenChannelTOP.data()->value();
    defolt_BLACK.G.bottom = s_GreenChannelBOTTOM.data()->value();

    defolt_BLACK.B.top = s_BlueChannelTOP.data()->value();
    defolt_BLACK.B.bottom = s_BlueChannelBOTTOM.data()->value();

    updateColors(QCoreApplication::applicationDirPath()+"/res/colors.cfg");
}

void CV_SettingsWidget::CreateLayout()
{
    QGridLayout* layout = new QGridLayout(this);
    QWidget* spacer = new QWidget;
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    layout->addWidget(l_RedChannelTOP.data(), 0, 0);
    layout->addWidget(l_RedChannelBOTTOM.data(), 1, 0);

    layout->addWidget(new QWidget(), 2, 0, 2, 1);

    layout->addWidget(l_GreenChannelTOP.data(), 3, 0);
    layout->addWidget(l_GreenChannelBOTTOM.data(), 4, 0);

    layout->addWidget(new QWidget(), 5, 0, 5, 1);

    layout->addWidget(l_BlueChannelTOP.data(), 6, 0);
    layout->addWidget(l_BlueChannelBOTTOM.data(), 7, 0);

    s_RedChannelTOP.data()->setRange(0, 255);
    s_GreenChannelTOP.data()->setRange(0, 255);
    s_BlueChannelTOP.data()->setRange(0, 255);
    s_RedChannelBOTTOM.data()->setRange(0, 255);
    s_GreenChannelBOTTOM.data()->setRange(0, 255);
    s_BlueChannelBOTTOM.data()->setRange(0, 255);

    s_RedChannelTOP.data()->setValue(255);
    s_GreenChannelTOP.data()->setValue(255);
    s_BlueChannelTOP.data()->setValue(255);
    s_RedChannelBOTTOM.data()->setValue(0);
    s_GreenChannelBOTTOM.data()->setValue(0);
    s_BlueChannelBOTTOM.data()->setValue(0);

    layout->addWidget(s_RedChannelTOP.data(), 0, 1);
    layout->addWidget(s_RedChannelBOTTOM.data(), 1, 1);

    layout->addWidget(s_GreenChannelTOP.data(), 3, 1);
    layout->addWidget(s_GreenChannelBOTTOM.data(), 4, 1);

    layout->addWidget(s_BlueChannelTOP.data(), 6, 1);
    layout->addWidget(s_BlueChannelBOTTOM.data(), 7, 1);

    layout->addWidget(b_showDefoltRED.data(), 10, 0);
    layout->addWidget(b_showDefoltYELLOW.data(), 11, 0);
    layout->addWidget(b_showDefoltBLUE.data(), 12, 0);
    layout->addWidget(b_showDefoltBLACK.data(), 13, 0);

    layout->addWidget(b_setDefoltRED.data(), 10, 1);
    layout->addWidget(b_setDefoltYELLOW.data(), 11, 1);
    layout->addWidget(b_setDefoltBLUE.data(), 12, 1);
    layout->addWidget(b_setDefoltBLACK.data(), 13, 1);

    ///___________________________________________________________________________        

    b_onRED.data()->setChecked(true);
    b_onYELLOW.data()->setChecked(true);
    b_onBLUE.data()->setChecked(true);
    b_onBLACK.data()->setChecked(true);

    layout->addWidget(b_onRED.data(), 10, 2);
    layout->addWidget(b_onYELLOW.data(), 11, 2);
    layout->addWidget(b_onBLUE.data(), 12, 2);
    layout->addWidget(b_onBLACK.data(), 13, 2);



    layout->addWidget(UH8.data(), 4, 3);
    layout->addWidget(L6R.data(), 7, 3);
    layout->addWidget(G7C.data(), 10, 3);
    layout->addWidget(S1P.data(), 11, 3);
    layout->addWidget(JW3.data(), 12, 3);
    layout->addWidget(A2X.data(), 13, 3);


    layout->addWidget(Generate.data(), 0, 3);

    QStringList str;
    str<<"try RED"<<"try YELLOW"<<"try BLUE"<<"try BLACK";
    cb_debugColor->addItems(str);

    layout->addWidget(l_debug.data(), 14, 0);
    layout->addWidget(cb_debugColor.data(), 14, 1);

    layout->addWidget(b_save_config.data(), 14, 2);

    layout->setRowMinimumHeight(11, 50);
    layout->setRowMinimumHeight(12, 50);
    layout->setRowMinimumHeight(10, 50);
    layout->setRowMinimumHeight(13, 50);

    setLayout(layout);
}
