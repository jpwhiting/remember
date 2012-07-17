#ifndef REMEMBERAPP_H
#define REMEMBERAPP_H

#include <QObject>

#include "qrtm/src/service.h"
#include "qrtm/src/task.h"

class RememberApp : public QObject
{
    Q_OBJECT
    Q_PROPERTY(RTM::Task * currentTask
               READ getCurrentTask
               NOTIFY currentTaskChanged)

public:
    explicit RememberApp(QObject *parent = 0);

    void start();

    Q_INVOKABLE RTM::Task *getCurrentTask() const;

    RTM::Service *getService() const;
signals:
    void currentTaskChanged();

public slots:
    // Forget the auth token, to log in again or as someone else.
    void forgetAuthToken();

    // Set the current task to the task from filtered tasks model at row given.
    void setCurrentTask(int row);

private slots:
    void onAuthenticationDone(bool success);

private:
    class Private;
    Private *const d;
};

#endif // REMEMBERSETTINGS_H
