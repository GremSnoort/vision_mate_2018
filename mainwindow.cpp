#include "MainWindow.hpp"

#include <QDockWidget>
#include <QMenu>
#include <QMenuBar>

QTextEdit* MainWindow::logWidget = nullptr;

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , m_cameraWidget(new RovCameraWidget(this))
{
    logWidget = new QTextEdit;
    createActions();
    createMenus();
    createDocks();
    createConnections();
    setCentralWidget(m_cameraWidget.data());
    setMinimumSize(800, 530);
    setWindowIcon(QIcon(QCoreApplication::applicationDirPath()+"/res/ic.jpg"));
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    Q_UNUSED(event);
    m_cameraWidget->stop();
}

MainWindow::~MainWindow() {}

void MainWindow::createDocks()
{
    QMenu* view = menuBar()->addMenu(tr("&Вид"));

    QDockWidget* logDock = new QDockWidget(tr("Информация"), this);
    logDock->setObjectName("LogDockWidget");
    logDock->setFeatures(QDockWidget::DockWidgetClosable | QDockWidget::DockWidgetMovable);
    logDock->setWidget(logWidget);
    addDockWidget(Qt::LeftDockWidgetArea, logDock);
    view->addAction(logDock->toggleViewAction());
}

void MainWindow::createMenus()
{
    QMenu* camera = menuBar()->addMenu(tr("Камера"));
    camera->addAction(m_startCameraAct.data());
    camera->addAction(m_stopCameraAct.data());
    camera->addAction(m_startDetectMode.data());
    camera->addAction(m_stopDetectMode.data());
    camera->addAction(m_startDebug.data());
    camera->addAction(m_stopDebug.data());
    camera->addAction(m_takeScreenshot.data());
}

void MainWindow::createConnections()
{
    QObject::connect(m_startCameraAct.data(), &QAction::triggered, [this](bool) {
        m_cameraWidget->startCapture();
    });
    QObject::connect(m_stopCameraAct.data(), &QAction::triggered, [this](bool) {
        m_cameraWidget->stopCapture();
    });

    QObject::connect(m_startDetectMode.data(), &QAction::triggered, [this](bool) {
        m_cameraWidget->startDetect();
    });
    QObject::connect(m_stopDetectMode.data(), &QAction::triggered, [this](bool) {
        m_cameraWidget->stopDetect();
    });

    QObject::connect(m_startDebug.data(), &QAction::triggered, [this](bool) {
        m_cameraWidget->startDebug();
    });
    QObject::connect(m_stopDebug.data(), &QAction::triggered, [this](bool) {
        m_cameraWidget->stopDebug();
    });

    QObject::connect(m_takeScreenshot.data(), &QAction::triggered, [this](bool) {
        m_cameraWidget->takeScreenshot();
    });
}

void MainWindow::createActions()
{
    m_startCameraAct.reset(new QAction(tr("Начать захват изображения"), this));
    m_stopCameraAct.reset(new QAction(tr("Остановить захват изображения"), this));

    m_startDetectMode.reset(new QAction(tr("Начать распознавание"), this));
    m_stopDetectMode.reset(new QAction(tr("Остановить распознавание"), this));
    m_startDebug.reset(new QAction(tr("Начать отладку"), this));
    m_stopDebug.reset(new QAction(tr("Завершить отладку"), this));

    m_takeScreenshot.reset(new QAction(tr("Скриншот")));

    m_startCameraAct->setShortcut(tr("Ctrl+A"));
    m_stopCameraAct->setShortcut(tr("Ctrl+X"));
    m_startDetectMode->setShortcut(tr("Ctrl+D"));
    m_stopDetectMode->setShortcut(tr("Ctrl+Q"));
    m_startDebug->setShortcut(tr("Ctrl+B"));
    m_stopDebug->setShortcut(tr("Ctrl+W"));
    m_takeScreenshot->setShortcut(tr("Ctrl+S"));
}
