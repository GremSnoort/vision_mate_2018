#pragma once

#include <QCamera>
#include <QCameraViewfinder>
#include <QComboBox>
#include <QPushButton>
#include <QStackedWidget>
#include <QWidget>
#include <QBasicTimer>
#include <QTimerEvent>

#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"

#include "cv_colorfiguredetect.h"
#include "cv_textdetect.h"
#include "new_neuralnet.h"

using namespace cv;

class RovCameraWidget : public QStackedWidget {
    Q_OBJECT
public:
    explicit RovCameraWidget(QWidget* parent = nullptr);

    QScopedPointer<CV_SettingsWidget> m_settings;

    QBasicTimer detect_timer;
    QBasicTimer debug_timer;

    void timerEvent(QTimerEvent * ev);

    void Detect();

    void Debug();

    Mat convert();

    void stop();

signals:

public slots:
    void startCapture();

    void stopCapture();

    void startDetect();

    void stopDetect();

    void startDebug();

    void stopDebug();

    void takeScreenshot();

    void Generate();

    QCamera::Status cameraStatus();

private:
    enum WidgetType {
        CameraSelectionWidget = 0,
        CameraViewWidget = 1
    };

    int count = 0;

    void createCameraSelectionWidget();

    void createCameraViewWidget();

    void createConnections();

    QScopedPointer<QCameraViewfinder> m_cameraView;
    QScopedPointer<QCamera> m_camera;
    QScopedPointer<QComboBox> m_cameraList;
    QScopedPointer<QPushButton> m_refreshButton;
};
