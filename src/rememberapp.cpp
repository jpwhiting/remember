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

#include "filteredtasksmodel.h"
#include "listsmodel.h"
#include "tasksmodel.h"
#include "rtm/request.h"
#include "rtm/task.h"

#include <QtDeclarative>
#include <QSettings>

const QString kAuthTokenKey = "AuthToken";

class RememberApp::Private {
public:

    RTM::Session *session;

    RTM::ListsModel *listsModel;
    RTM::FilteredTasksModel *filteredTasksModel;
    QMap<RTM::ListId, RTM::TasksModel *> tasksModels;

    RTM::Task *currentTask;
    QMap<RTM::TaskId, RTM::Task*> selectedTasks;
    QSettings settings;
    QString authToken;
};

RememberApp::RememberApp(QObject *parent) :
    QObject(parent), d(new Private)
{
    d->session = new RTM::Session(kRTMKey, kRTMSecret, RTM::Delete, QString(), this);
    d->currentTask = NULL;

    d->listsModel = new RTM::ListsModel(this);
    d->listsModel->setSession(d->session);
    d->filteredTasksModel = new RTM::FilteredTasksModel(this);

    connect(d->session, SIGNAL(tokenCheck(bool)),
            SLOT(onAuthenticationDone(bool)));
    connect(d->session, SIGNAL(listChanged(RTM::List*)),
            SLOT(onListChanged(RTM::List*)));

    d->authToken = d->settings.value(kAuthTokenKey).toString();

    qmlRegisterUncreatableType<RTM::Session>("com.jpwhiting", 1, 0, "Session",
                                             "Session cannot be created");
    qmlRegisterType<RTM::ListsModel>("com.jpwhiting", 1, 0, "ListsModel");
    qmlRegisterType<RTM::FilteredTasksModel>("com.jpwhiting", 1, 0, "FilteredTasksModel");
    qmlRegisterUncreatableType<RTM::Task>("com.jpwhiting", 1, 0, "Task",
                                          "Task cannot be created");
    qmlRegisterType<RememberApp>("com.jpwhiting", 1, 0, "RememberApp");
}

RememberApp::~RememberApp()
{
    delete d->listsModel;
    d->listsModel = NULL;
}

void RememberApp::start()
{
    if (!d->authToken.isEmpty())
        d->session->setToken(d->authToken);
}

void RememberApp::forgetAuthToken()
{
    d->settings.remove(kAuthTokenKey);
    d->session->setToken(QString());
}

void RememberApp::onAuthenticationDone(bool success)
{
    if (success)
    {
        // Get the auth key and write it to settings.
        d->authToken = d->session->token();
        d->settings.setValue(kAuthTokenKey, d->authToken);

        // Request lists and tasks.
        d->session->refreshListsFromServer();
        d->session->refreshTasksFromServer();
    }
}

void RememberApp::onListChanged(RTM::List* list)
{
    qDebug() << "onListChanged called for list " << list->name();
    if (d->tasksModels.contains(list->id()))
    {
        d->tasksModels.value(list->id())->onListChanged();
    }
    else
    {
        RTM::TasksModel *model = new RTM::TasksModel(this);
        model->setList(list);
        d->tasksModels.insert(list->id(), model);
    }
}

RTM::Session *RememberApp::getSession() const
{
    return d->session;
}

bool RememberApp::showSplashScreen() const
{
    bool retval = false;
    if (!d->settings.contains("FirstRunComplete"))
    {
        retval = true;
        d->settings.setValue("FirstRunComplete", true);
    }
    return retval;
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

void RememberApp::markTasksCompleted()
{
    Q_FOREACH(RTM::Task* task, d->selectedTasks.values())
    {
        task->setCompleted(true);
    }
    d->selectedTasks.clear();
    emit selectedTasksCountChanged();
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

void RememberApp::setListId(qulonglong id)
{
    // Get the list parameters from from the listsModel.
    RTM::List *list = d->listsModel->listFromId(id);
    d->selectedTasks.clear();

    Q_ASSERT(d->tasksModels.contains(list->id()));

    RTM::TasksModel *model = d->tasksModels.value(list->id());
    if (d->filteredTasksModel->sourceModel() != model)
        d->filteredTasksModel->setSourceModel(model);

    // Give the list parameters to the filtered model
    d->filteredTasksModel->setListParameters(list->sortOrder());
}

int RememberApp::getSelectedTasksCount() const
{
    return d->selectedTasks.count();
}

void RememberApp::selectTask(int row, bool select)
{
    RTM::Task* task = d->filteredTasksModel->taskForRow(row);
    if (select)
    {
        d->selectedTasks.insert(task->id(), task);
    }
    else
    {
        d->selectedTasks.remove(task->id());
    }

    emit selectedTasksCountChanged();
}

