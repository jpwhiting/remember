

#include <QtGui/QApplication>
#include <QtDeclarative>
#include <QSettings>
#include "qmlapplicationviewer.h"
#include "rememberapp.h"

Q_DECL_EXPORT int main(int argc, char *argv[])
{
    QScopedPointer<QApplication> app(createApplication(argc, argv));
    QCoreApplication::setOrganizationName("JPWhiting");
    QCoreApplication::setApplicationName("Remember");

    RememberApp *rememberApp = new RememberApp;

    QmlApplicationViewer viewer;
    viewer.engine()->rootContext()->setContextProperty("myApp", rememberApp);
    viewer.engine()->rootContext()->setContextProperty("service", rememberApp->getService());
    viewer.setOrientation(QmlApplicationViewer::ScreenOrientationAuto);
    viewer.setMainQmlFile(QLatin1String("qml/remember/main.qml"));
    viewer.showExpanded();

    rememberApp->start();

    return app->exec();
}
