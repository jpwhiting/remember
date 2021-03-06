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

#ifndef FILTEREDTASKSMODEL_H
#define FILTEREDTASKSMODEL_H

#include "rtm/list.h"
#include "rtm/task.h"
#include <QSortFilterProxyModel>

namespace RTM
{

class FilteredTasksModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit FilteredTasksModel(QObject *parent = 0);

    Q_INVOKABLE void setCompleted(bool completed);

    Q_INVOKABLE void setListParameters(List::SortOrder sortOrder);

    Q_INVOKABLE RTM::Task *taskForRow(const int row) const;

protected:
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const;
    bool lessThan(const QModelIndex &left, const QModelIndex &right) const;

private:
    class Private;
    Private *const d;
};

}

#endif // TASKSMODELFILTER_H
