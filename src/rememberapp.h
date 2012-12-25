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

#ifndef REMEMBERAPP_H
#define REMEMBERAPP_H

#include <QObject>

#include "rtm/rtm.h"
#include "rtm/session.h"

namespace RTM {
    class FilteredTasksModel;
    class List;
    class ListsModel;
    class Task;
}

class RememberApp : public QObject
{
    Q_OBJECT
    Q_PROPERTY(RTM::ListsModel * listsModel READ getListsModel)
    Q_PROPERTY(RTM::Task * currentTask
               READ getCurrentTask
               NOTIFY currentTaskChanged)
    Q_PROPERTY(RTM::FilteredTasksModel * tasksModel READ getTasksModel)
    Q_PROPERTY(int selectedTasksCount READ getSelectedTasksCount
               NOTIFY selectedTasksCountChanged)

public:
    explicit RememberApp(QObject *parent = 0);
    virtual ~RememberApp();

    void start();

    Q_INVOKABLE RTM::ListsModel *getListsModel() const;
    Q_INVOKABLE RTM::Task *getCurrentTask() const;
    Q_INVOKABLE RTM::FilteredTasksModel *getTasksModel() const;

    Q_INVOKABLE void setListId(qulonglong id);
    // Select or deselect the task at given row.
    Q_INVOKABLE void selectTask(int row, bool select);
    // Add a task to the current list.
    Q_INVOKABLE void addTask(QString taskText);
    // Add a list.
    Q_INVOKABLE void addList(QString listText);

    // Get whether or not to show the splash screen.
    // By default as soon as it has been shown once, it wont be shown again
    // unless the user asks for it from menu -> About
    Q_INVOKABLE bool showSplashScreen() const;

    RTM::Session *getSession() const;
    int getSelectedTasksCount() const;

signals:
    void currentTaskChanged();
    void selectedTasksCountChanged();

public slots:
    // Forget the auth token, to log in again or as someone else.
    void forgetAuthToken();

    // Set the current task to the task from filtered tasks model at row given.
    void setCurrentTask(int row);

    // Mark the selected tasks as completed.
    void markTasksCompleted();

private slots:
    void onAuthenticationDone(bool success);

    void onListChanged(RTM::List* list);

    void onListsChanged();

private:
    class Private;
    Private *const d;
};

#endif // REMEMBERSETTINGS_H
