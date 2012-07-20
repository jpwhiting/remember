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

#include "rememberapp.h"

#include "apikey.h"

#include "qrtm/src/filteredtasksmodel.h"
#include "qrtm/src/listsmodel.h"
#include "qrtm/src/request.h"
#include "qrtm/src/service.h"
#include "qrtm/src/task.h"
#include "qrtm/src/tasksmodel.h"

#include <QtDeclarative>
#include <QSettings>

const QString kAuthTokenKey = "AuthToken";

class RememberApp::Private {
public:

    RTM::Service *service;

    RTM::ListsModel *listsModel;
    RTM::TasksModel *tasksModel;
    RTM::FilteredTasksModel *filteredTasksModel;
    QMap<RTM::Request*, QString> smartListRequestIds;
    QMap<QString, RTM::TasksModel *> smartListTasksModels;

    RTM::Task *currentTask;
    QSettings settings;
    QString authToken;
};

RememberApp::RememberApp(QObject *parent) :
    QObject(parent), d(new Private)
{
    d->service = new RTM::Service(kRTMKey, kRTMSecret, this);
    d->currentTask = NULL;

    d->listsModel = new RTM::ListsModel(d->service);
    connect(d->service, SIGNAL(listsGetListFinished(QVariantMap,RTM::ResponseStatus)),
            d->listsModel, SLOT(onGetListFinished(QVariantMap,RTM::ResponseStatus)));
    d->tasksModel = new RTM::TasksModel(this);
    d->filteredTasksModel = new RTM::FilteredTasksModel(this);
    d->filteredTasksModel->setSourceModel(d->tasksModel);

    connect(d->service, SIGNAL(authenticationDone(bool)),
            SLOT(onAuthenticationDone(bool)));
    connect(d->service, SIGNAL(tasksGetListFinished(QVariantMap,RTM::ResponseStatus)),
            this, SLOT(onTasksGetListFinished(QVariantMap, RTM::ResponseStatus)));
    connect(d->service, SIGNAL(tasksGetSmartListFinished(QString,QVariantMap,RTM::ResponseStatus)),
            this, SLOT(onTasksGetSmartListFinished(QString,QVariantMap, RTM::ResponseStatus)));

    d->authToken = d->settings.value(kAuthTokenKey).toString();
    connect(d->listsModel, SIGNAL(loadedListInfo(RTM::List*)),
            this, SLOT(onLoadedListInfo(RTM::List*)));

    qmlRegisterType<RTM::Service>("com.jpwhiting", 1, 0, "Service");
    qmlRegisterType<RTM::ListsModel>("com.jpwhiting", 1, 0, "ListsModel");
    qmlRegisterType<RTM::Task>("com.jpwhiting", 1, 0, "Task");
    qmlRegisterType<RTM::FilteredTasksModel>("com.jpwhiting", 1, 0, "FilteredTasksModel");
    qmlRegisterType<RememberApp>("com.jpwhiting", 1, 0, "RememberApp");
}

RememberApp::~RememberApp()
{
    delete d->listsModel;
    d->listsModel = NULL;

    delete d->tasksModel;
    d->tasksModel = NULL;
}

void RememberApp::start()
{
    if (d->authToken.isEmpty())
        d->service->authenticate(RTM::Read);
    else
        d->service->authCheckToken(d->authToken);
}

void RememberApp::forgetAuthToken()
{
    d->settings.remove(kAuthTokenKey);
    d->service->authenticate(RTM::Read);
}

void RememberApp::onAuthenticationDone(bool success)
{
    if (success)
    {
        // Get the auth key and write it to settings.
        d->authToken = d->service->getToken();
        d->settings.setValue(kAuthTokenKey, d->authToken);
    }
    else
    {
        d->listsModel->clear();
        d->tasksModel->clear();
        Q_FOREACH (RTM::TasksModel* model, d->smartListTasksModels)
        {
            model->clear();
        }

        d->smartListTasksModels.clear();
    }
}

RTM::Service *RememberApp::getService() const
{
    return d->service;
}

void RememberApp::setCurrentTask(int row)
{
    RTM::Task * task = d->filteredTasksModel->taskForRow(row);
    if (task && task != d->currentTask)
    {
        d->currentTask = task;
        emit currentTaskChanged();
    }
}

RTM::ListsModel *RememberApp::getListsModel() const
{
    return d->listsModel;
}

RTM::Task *RememberApp::getCurrentTask() const
{
    return d->currentTask;
}

RTM::FilteredTasksModel *RememberApp::getTasksModel() const
{
    return d->filteredTasksModel;
}

void RememberApp::setListId(QString id)
{
    // Get the list parameters from from the listsModel.
    RTM::List *list = d->listsModel->listFromId(id);

    if (list->isSmart())
    {
        Q_ASSERT(d->smartListTasksModels.contains(list->id()));

        RTM::TasksModel *model = d->smartListTasksModels.value(list->id());
        if (d->filteredTasksModel->sourceModel() != model)
            d->filteredTasksModel->setSourceModel(model);

        // Give the list parameters to the filtered model
        d->filteredTasksModel->setListParameters(QString(), list->sortOrder());
    }
    else
    {
        // Reset the source model to the main model if needed.
        if (d->filteredTasksModel->sourceModel() != d->tasksModel)
            d->filteredTasksModel->setSourceModel(d->tasksModel);

        // Give the list parameters to the filtered model
        d->filteredTasksModel->setListParameters(id, list->sortOrder());
    }
}

void RememberApp::onTasksGetListFinished(QVariantMap response,
                                         RTM::ResponseStatus status)
{
    if (status == RTM::OK)
    {
        QList<RTM::Task*> tasks = RTM::GenerateTaskList(response);

        Q_FOREACH (RTM::Task *task, tasks)
        {
            d->tasksModel->addTask(task);
        }
    }
}

void RememberApp::onTasksGetSmartListFinished(QString listId,
                                              QVariantMap response,
                                              RTM::ResponseStatus status)
{
    if (status == RTM::OK)
    {
        QList<RTM::Task*> tasks = RTM::GenerateTaskList(response);

        Q_ASSERT(d->smartListTasksModels.contains(listId));
        RTM::TasksModel *model = d->smartListTasksModels.value(listId);

        Q_FOREACH (RTM::Task *task, tasks)
        {
            model->addTask(task);
        }
    }
}

void RememberApp::onLoadedListInfo(RTM::List *listInfo)
{
    if (listInfo->isSmart())
    {
        // Create a model for this list.
        RTM::TasksModel *newModel = new RTM::TasksModel(this);
        d->smartListTasksModels.insert(listInfo->id(), newModel);

        // Request this list's tasks from the server.
        d->service->tasksGetSmartList(listInfo->id(), listInfo->filter());
    }
}


