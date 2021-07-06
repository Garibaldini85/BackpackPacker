/*
 *  BackpackPacker Copyright (C) 2021  Kambarov I. G.
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 *  Subsequent modifications must be distributed under the same license.
 */

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
 * "ver_0.2.0"
*/


int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
    qRegisterMetaType<QGeneratorKey*>("QGeneratorKey*");
    qRegisterMetaType<QEncoder*>("QEncoder*");
    qRegisterMetaType<QDecoder*>("QDecoder*");
    qRegisterMetaType<QChecker*>("QChecker*");

    QGuiApplication app(argc, argv);

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
