QT += quick multimedia

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Refer to the documentation for the
# deprecated API to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        cameramodellist.cpp \
        ffmpeghelper.cpp \
        imageprovider.cpp \
        main.cpp \
        resolutionmodel.cpp \
        resolutionmodellist.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

INCLUDEPATH +=$$PWD/include/ffmpeg

LIBS += $$PWD/lib/ffmpeg/avcodec.lib \
        $$PWD/lib/ffmpeg/avfilter.lib \
        $$PWD/lib/ffmpeg/avdevice.lib \
        $$PWD/lib/ffmpeg/avformat.lib \
        $$PWD/lib/ffmpeg/avutil.lib \
        $$PWD/lib/ffmpeg/postproc.lib \
        $$PWD/lib/ffmpeg/swresample.lib \
        $$PWD/lib/ffmpeg/swscale.lib

INCLUDEPATH +=$$PWD/include \
              $$PWD/include/opencv2

LIBS += $$PWD/lib/opencv2/opencv_world480.lib

HEADERS += \
    cameramodellist.h \
    ffmpeghelper.h \
    imageprovider.h \
    resolutionmodel.h \
    resolutionmodellist.h

RC_ICONS=position.ico

DISTFILES += \
    position.ico
