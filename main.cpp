#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
//#include <QIcon>
#include "cameramodellist.h"
#include "ffmpeghelper.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    CameraModelList *cameraModelList=new CameraModelList();

    FfmpegHelper *Ffmpeg=new FfmpegHelper();

    engine.rootContext()->setContextProperty("FFmpeg",Ffmpeg);
    engine.addImageProvider(QLatin1String("imageprovider"), Ffmpeg->imageProvider);

    engine.rootContext()->setContextProperty("CameraModelList",cameraModelList);
    engine.rootContext()->setContextProperty("ResolutionModelList",cameraModelList->resolutionModelList);

    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    app.exec();

    if(cameraModelList)
    {
        delete cameraModelList;
    }
    return 1;
}
