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

#include "tasksmodel.h"

#include "rtm/list.h"
#include "rtm/task.h"

#include <QDateTime>
#include <QStringList>

namespace RTM {

class TasksModel::Private {
public:
    RTM::List* list;
};

TasksModel::TasksModel(QObject *parent) :
    QAbstractListModel(parent),
    d(new Private)
{
    QHash<int, QByteArray> roles;
    roles[Qt::DisplayRole] = "display";
    roles[PriorityRole] = "priority";
    roles[ListIdRole] = "listid";
    roles[SeriesIdRole] = "seriesid";
    roles[TaskIdRole] = "taskid";
    roles[CompletedRole] = "completed";
    roles[DueDateRole] = "duedate";
    roles[TagsRole] = "tags";
    roles[NotesRole] = "notes";
    roles[LocationRole] = "location";
    roles[UrlRole] = "url";
    setRoleNames(roles);
}

void TasksModel::setList(RTM::List* list)
{
    beginResetModel();
    d->list = list;
    endResetModel();
}

void TasksModel::onListChanged()
{
    beginResetModel();
    endResetModel();
}

int TasksModel::rowCount(const QModelIndex &index) const
{
    if (!index.isValid())
        return d->list->tasks.size();
    return 0;
}

QVariant TasksModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    QVariant retval;

    if (orientation == Qt::Horizontal && section == 0)
    {
        switch (role)
        {
        case Qt::DisplayRole:
            retval = tr("Tasks");
            break;
        }
    }
    return retval;
}

QVariant TasksModel::data ( const QModelIndex & index, int role) const
{
    QVariant retval;

    if (index.isValid())
    {
        const Task *task = taskForRow(index.row());
        switch (role)
        {
        case Qt::DisplayRole:
            retval = task->name();
            break;
        case PriorityRole:
            retval = task->priority();
            break;
        case ListIdRole:
            retval = task->listId();
            break;
        case SeriesIdRole:
            retval = task->seriesId();
            break;
        case TaskIdRole:
            retval = task->id();
            break;
        case CompletedRole:
            retval = task->completed();
            break;
        case DueDateRole:
            retval = task->due();
            break;
        case TagsRole:
            retval = task->tags();
            break;
        case NotesRole:
//            retval = task->notes();
            break;
        case LocationRole:
            retval = task->locationId();
            break;
        case UrlRole:
            retval = task->url();
            break;
        }
    }

    return retval;
}

RTM::Task* TasksModel::taskForRow(const int row) const
{
    Task * value = 0;
    if (row >= 0 && row < d->list->tasks.size())
    {
        QHash<RTM::TaskId, Task*>::iterator i = d->list->tasks.begin();
        i += row;
        value = i.value();
    }

    return value;
}

} // namespace RTM
