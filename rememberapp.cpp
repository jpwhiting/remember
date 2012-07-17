#include "rememberapp.h"

#include "apikey.h"

#include "qrtm/src/listsmodel.h"
#include "qrtm/src/service.h"
#include "qrtm/src/task.h"
#include "qrtm/src/filteredtasksmodel.h"

#include <QtDeclarative>
#include <QSettings>

const QString kAuthTokenKey = "AuthToken";

using namespace RTM;

class RememberApp::Private {
public:

    RTM::Service *service;
    RTM::Task *currentTask;
    QSettings settings;
    QString authToken;
};

RememberApp::RememberApp(QObject *parent) :
    QObject(parent), d(new Private)
{
    d->service = new RTM::Service(kRTMKey, kRTMSecret, this);
    d->currentTask = NULL;
    connect(d->service, SIGNAL(authenticationDone(bool)),
            SLOT(onAuthenticationDone(bool)));
    d->authToken = d->settings.value(kAuthTokenKey).toString();

    qmlRegisterType<RTM::Service>("com.jpwhiting", 1, 0, "Service");
    qmlRegisterType<RTM::ListsModel>("com.jpwhiting", 1, 0, "ListsModel");
    qmlRegisterType<RTM::Task>("com.jpwhiting", 1, 0, "Task");
    qmlRegisterType<RTM::FilteredTasksModel>("com.jpwhiting", 1, 0, "FilteredTasksModel");
    qmlRegisterType<RememberApp>("com.jpwhiting", 1, 0, "RememberApp");
}

void RememberApp::start()
{
    if (d->authToken.isEmpty())
        d->service->authenticate(Read);
    else
        d->service->authCheckToken(d->authToken);
}

void RememberApp::forgetAuthToken()
{
    d->settings.remove(kAuthTokenKey);
    d->service->authenticate(Read);
}

void RememberApp::onAuthenticationDone(bool success)
{
    if (success)
    {
        // Get the auth key and write it to settings.
        d->authToken = d->service->getToken();
        d->settings.setValue(kAuthTokenKey, d->authToken);
    }
}

RTM::Service *RememberApp::getService() const
{
    return d->service;
}

void RememberApp::setCurrentTask(int row)
{
    RTM::Task * task = d->service->getTasksModel()->taskForRow(row);
    if (task && task != d->currentTask)
    {
        d->currentTask = task;
        emit currentTaskChanged();
    }
}

RTM::Task *RememberApp::getCurrentTask() const
{
    return d->currentTask;
}
