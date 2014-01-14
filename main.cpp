#include <QtGui/QGuiApplication>
#include "qtquick2applicationviewer.h"
#include <QQmlExtensionPlugin>
#include "oauth2.h"
#include <qqml.h>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);


    QtQuick2ApplicationViewer viewer;
    qmlRegisterType<OAuth2>("OAuth2",0,1,"OAuth2");

    viewer.setMainQmlFile(QStringLiteral("qml/OAuthPlayground/main.qml"));
    viewer.showExpanded();

    return app.exec();
}
