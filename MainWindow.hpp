#pragma once


#include "RovCameraWidget.hpp"
#include <QAction>
#include <QMainWindow>
#include <QTextEdit>

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget* parent = 0);

    ~MainWindow();
    static QTextEdit* logWidget;

    void closeEvent(QCloseEvent *event);

private:
    void createMenus();

    void createDocks();

    void createConnections();

    void createActions();

    QScopedPointer<QAction> m_startCameraAct;
    QScopedPointer<QAction> m_stopCameraAct;

    QScopedPointer<QAction> m_startDetectMode;
    QScopedPointer<QAction> m_stopDetectMode;

    QScopedPointer<QAction> m_startDebug;
    QScopedPointer<QAction> m_stopDebug;

    QScopedPointer<QAction> m_takeScreenshot;

    QScopedPointer<RovCameraWidget> m_cameraWidget;
};
