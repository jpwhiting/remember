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

#include "listsmodel.h"

#include "rtm/list.h"

#include <QtDebug>

namespace RTM {

class ListsModel::Private {
public:
    QList<List*> lists;
    RTM::Session *session;
};

ListsModel::ListsModel(QObject *parent) :
    QAbstractListModel(parent),
    d(new Private)
{
    d->session = 0;
    QHash<int, QByteArray> roles;
    roles[Qt::DisplayRole] = "display";
    roles[IdRole] = "id";
    roles[FilterRole] = "filter";
    roles[SortOrderRole] = "sortorder";
    setRoleNames(roles);
}

void ListsModel::setSession(RTM::Session* session)
{
    if (d->session)
        disconnect(this, SLOT(onListsChanged()));
    d->session = session;
    connect(d->session, SIGNAL(listsChanged()),
            SLOT(onListsChanged()));
}

List *ListsModel::listFromId(RTM::ListId &id)
{
    if (d->session)
        return d->session->cachedLists().value(id);
    else
        return 0;
}

int ListsModel::rowCount(const QModelIndex &index) const
{
    if (!index.isValid())
        return d->lists.size();
    return 0;
}

QVariant ListsModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    QVariant retval;

    if (orientation == Qt::Horizontal && section == 0)
    {
        switch (role)
        {
        case Qt::DisplayRole:
            retval = "Lists";
            break;
        }
    }
    return retval;
}

QVariant ListsModel::data ( const QModelIndex & index, int role) const
{
    QVariant retval;

    if (index.isValid())
    {
        List * list = d->lists.at(index.row());
        switch (role)
        {
        case Qt::DisplayRole:
            retval = list->name();
            break;
        case IdRole:
            retval = list->id();
            break;
        case FilterRole:
            retval = list->filter();
            break;
        case SortOrderRole:
            retval = list->sortOrder();
            break;
        }
    }

    return retval;
}

bool SortLists(const List *l1, const List *l2)
{
    if (l1->isSmart() == l2->isSmart())
    {
        if (l1->position() != l2->position())
            return l1->position() < l2->position();
        else
            return l1->name() < l2->name();
    }
    else
        // If l2 is smart, l1 goes first.
        return (l2->isSmart());
}

void ListsModel::onListsChanged()
{
    qDebug() << "onListsChanged called";
    QHash<ListId, List*> lists = d->session->cachedLists();
    if (lists.size() != d->lists.size())
    {
        beginResetModel();
        d->lists.clear();
        Q_FOREACH (List* info, lists.values())
        {
            d->lists.append(info);
        }
        qSort(d->lists.begin(), d->lists.end(), SortLists);

        endResetModel();
    }
}

}
