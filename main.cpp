#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <GlobeOsg.h>
#include <QtGui/QGuiApplication>
#include <QtQuick/QQuickView>

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);

    qmlRegisterType<GlobeOsg>("qmlosg.globe", 1, 0, "GlobeOsg");

    QQmlApplicationEngine engine;
    engine.load(QUrl(QLatin1String("main.qml")));

    return app.exec();
}
