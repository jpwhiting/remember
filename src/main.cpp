/*
 *  Copyright (C) 2012  Jeremy Whiting <jpwhiting@kde.org>
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>

 */

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
    viewer.engine()->rootContext()->setContextProperty("remember", rememberApp);
    viewer.engine()->rootContext()->setContextProperty("session", rememberApp->getSession());
    viewer.setOrientation(QmlApplicationViewer::ScreenOrientationAuto);
    viewer.setMainQmlFile(QLatin1String("qml/remember/main.qml"));
    viewer.showExpanded();

    rememberApp->start();

    return app->exec();
}
