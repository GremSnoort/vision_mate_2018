#include "RovCameraWidget.hpp"

#include <QCameraInfo>
#include <QVBoxLayout>

QList<cv::Mat> CV_TextDetect::tmpl_arr = QList<cv::Mat>();
type CV_Detect::TYPE = TYPE_FIGURE;
figureColor CV_Detect::COLOR = BLACK_;

RovCameraWidget::RovCameraWidget(QWidget* parent)
    : QStackedWidget(parent)
    , m_settings(new CV_SettingsWidget())
    , m_cameraView(new QCameraViewfinder(this))
    , m_cameraList(new QComboBox)
    , m_refreshButton(new QPushButton(tr("Обновить")))
{
    for(int i=1; i<=3; i++)
    {
        for(int j=1; j<=6; j++)
        {
            std::string str =  (QCoreApplication::applicationDirPath()+"/tmpl/simple/"+QString::number(i)+"_"+QString::number(j)+".jpg").toStdString();
            CV_TextDetect::tmpl_arr.append(imread(str));
        }
    }

    m_settings->hide();
    createCameraSelectionWidget();
    createCameraViewWidget();
    createConnections();
    setCurrentIndex(WidgetType::CameraSelectionWidget);

    connect(m_settings->Generate.data(), &QPushButton::released, this, &RovCameraWidget::Generate);
}

void RovCameraWidget::Generate()
{
    if(m_camera)
    {
        Mat frame = convert();
        NEW_neuralNet::Generate(frame, m_settings.data());
    }
}

void RovCameraWidget::stop()
{
    debug_timer.stop();
    detect_timer.stop();
    cvDestroyAllWindows();
    m_settings->hide();
    //emit stopped();
}

void RovCameraWidget::takeScreenshot()
{
    QPixmap pm = m_cameraView->grab(QRect(0, 0, m_cameraView->size().width(), m_cameraView->size().height()));
    pm.save("C:/Users/Eva/Desktop/PIC TO TD/WORK/screen"+QString::number(count)+".jpg", "JPG");////DESKTOP-3I4GEIE/Users/Public/Pictures/screen
    count+=1;
}

Mat RovCameraWidget::convert()
{
    QPixmap pm = m_cameraView->grab(QRect(0, 0, m_cameraView->size().width(), m_cameraView->size().height()));

    QImage inImage = pm.toImage();

    Mat  mat( inImage.height(), inImage.width(),
                              CV_8UC4,
                              const_cast<uchar*>(inImage.bits()),
                              static_cast<size_t>(inImage.bytesPerLine())
                              );

    Mat  matNoAlpha;

    cvtColor( mat, matNoAlpha, cv::COLOR_BGRA2BGR );   // drop the all-white alpha channel

    return matNoAlpha;
}

void RovCameraWidget::Detect()
{
    if(m_settings->isHidden())
        m_settings->show();

    Mat frame = convert();
    Mat showimg = CV_ColorFigureDetect::Detect(frame, m_settings.data());
    Mat shownew = NEW_neuralNet::Detect(frame, m_settings.data());
    //Mat showtxt = CV_TextDetect::Detect(frame, m_settings.data());

    if(CV_Detect::TYPE == TYPE_FIGURE)
        imshow("detect", showimg);
    //else if(CV_Detect::TYPE == TYPE_TEXT)
       // imshow("detect", showtxt);
    else imshow("detect",  frame);

}

void RovCameraWidget::Debug()
{
    if(m_settings->isHidden())
        m_settings->show();

    Mat frame = convert();
    Mat show = CV_ColorFigureDetect::Debug(frame, m_settings.data());
    imshow("~debug~", show);
}

void RovCameraWidget::timerEvent(QTimerEvent *ev)
{
    if(ev->timerId() == detect_timer.timerId())
        Detect();
    if(ev->timerId() == debug_timer.timerId())
        Debug();
}

void RovCameraWidget::startCapture()
{
    const QList<QCameraInfo> availableCameras = QCameraInfo::availableCameras();
    if (availableCameras.empty()) {
        qWarning() << "Нет доступных камер!";
        return;
    }
    QCameraInfo cameraInfo = availableCameras.at(m_cameraList->currentIndex());

    m_camera.reset(new QCamera(cameraInfo));
    m_camera->setViewfinder(m_cameraView.data());
    m_camera->start();
    setCurrentIndex(WidgetType::CameraViewWidget);
}

void RovCameraWidget::stopCapture()
{
    if (!m_camera) {
        return;
    }

    detect_timer.stop();
    debug_timer.stop();
    m_camera->stop();
    m_settings->hide();
    setCurrentIndex(WidgetType::CameraSelectionWidget);
}

void RovCameraWidget::startDetect()
{
    if(m_camera)
    {
        cvNamedWindow("detect", CV_WINDOW_AUTOSIZE);
        m_settings->show();
        detect_timer.start(100, this);
    }
}

void RovCameraWidget::stopDetect()
{
    detect_timer.stop();
    cvDestroyWindow("detect");
    m_settings->hide();
}

void RovCameraWidget::startDebug()
{
    if(m_camera)
    {
        cvNamedWindow("~debug~", CV_WINDOW_AUTOSIZE);
        m_settings->show();
        debug_timer.start(100, this);
    }
}

void RovCameraWidget::stopDebug()
{
    debug_timer.stop();
    cvDestroyWindow("~debug~");
    m_settings->hide();
}

QCamera::Status RovCameraWidget::cameraStatus()
{
    if (m_camera) {
        return m_camera->status();
    }
    return QCamera::Status::UnavailableStatus;
}

void RovCameraWidget::createCameraSelectionWidget()
{
    QWidget* cameraWidget = new QWidget;

    QWidget* topSpacer = new QWidget;
    QWidget* bottomSpacer = new QWidget;
    QWidget* leftSpacer = new QWidget;
    QWidget* rightSpacer = new QWidget;

    topSpacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    bottomSpacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    leftSpacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    rightSpacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    QHBoxLayout* listLayout = new QHBoxLayout;

    listLayout->addWidget(leftSpacer);
    listLayout->addWidget(m_cameraList.data());
    listLayout->addWidget(m_refreshButton.data());
    listLayout->addWidget(rightSpacer);

    m_refreshButton->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    m_cameraList->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    const QList<QCameraInfo> availableCameras = QCameraInfo::availableCameras();
    for (const QCameraInfo& cameraInfo : availableCameras) {
        m_cameraList->addItem(cameraInfo.description());
    }

    QVBoxLayout* cameraListLayout = new QVBoxLayout;
    cameraListLayout->addWidget(topSpacer);
    cameraListLayout->addLayout(listLayout);
    cameraListLayout->addWidget(bottomSpacer);
    cameraWidget->setLayout(cameraListLayout);
    addWidget(cameraWidget);
}

void RovCameraWidget::createCameraViewWidget()
{
    addWidget(m_cameraView.data());
}

void RovCameraWidget::createConnections()
{
    QObject::connect(m_refreshButton.data(), &QPushButton::pressed, [this]() {
        m_cameraList->clear();
        const QList<QCameraInfo> availableCameras = QCameraInfo::availableCameras();
        for (const QCameraInfo& cameraInfo : availableCameras) {
            m_cameraList->addItem(cameraInfo.description());
        }
    });
}
