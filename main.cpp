#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QThread>
#include <QQmlContext>
#include <QString>
#include <QIcon>

#include "qmainpackingpackageofpackedpackpacker.h"
#include "qfilehandler.h"
#include "qdecoder.h"

/*
 * "ver_0.1.1"
*/


int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    QGuiApplication app(argc, argv);

    //app.setWindowIcon(QIcon("qml.qrc:/icon.png"));

    QQmlApplicationEngine engine;
    QMainPackingPackageOfPackedPackPacker *packer = new QMainPackingPackageOfPackedPackPacker();
    QQmlContext* ctx = engine.rootContext();

    ctx->setContextProperty("packer", packer);

    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);

    engine.load(url);

    return app.exec();
}
