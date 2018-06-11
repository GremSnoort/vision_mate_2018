#-------------------------------------------------
#
# Project created by QtCreator 2018-04-28T13:38:09
#
#-------------------------------------------------

QT       += core gui network multimedia multimediawidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Nyasha_0X4_vision
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp \
        RovCameraWidget.cpp \
        cv_colorfiguredetect.cpp \
        cv_detect.cpp \
        cv_settingswidget.cpp \
        cv_textdetect.cpp \
    new_neuralnet.cpp

HEADERS += \
        mainwindow.hpp \
        RovCameraWidget.hpp \
        cv_colorfiguredetect.h \
        cv_datatypes.h \
        cv_detect.h \
        cv_settingswidget.h \
        cv_textdetect.h \
    new_neuralnet.h

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/thirdpaty/opencv/build/x64/vc15/lib/ -lopencv_world340
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/thirdpaty/opencv/build/x64/vc15/lib/ -lopencv_world340d
else:unix: LIBS += -L$$PWD/thirdpaty/opencv/build/x64/vc15/lib/ -lopencv_world340

INCLUDEPATH += $$PWD/thirdpaty/opencv/build/include
DEPENDPATH += $$PWD/thirdpaty/opencv/build/include

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/thirdpaty/opencv/build/x64/vc15/lib/libopencv_world340.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/thirdpaty/opencv/build/x64/vc15/lib/libopencv_world340d.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/thirdpaty/opencv/build/x64/vc15/lib/opencv_world340.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/thirdpaty/opencv/build/x64/vc15/lib/opencv_world340d.lib
else:unix: PRE_TARGETDEPS += $$PWD/thirdpaty/opencv/build/x64/vc15/lib/libopencv_world340.a
