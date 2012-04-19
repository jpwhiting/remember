#ifndef REMEMBERAPP_H
#define REMEMBERAPP_H

#include <QObject>

#include "qrtm/src/service.h"

class RememberApp : public QObject
{
    Q_OBJECT
public:
    explicit RememberApp(QObject *parent = 0);

    void start();
    
    RTM::Service *getService() const;
signals:
    
public slots:
    // Forget the auth token, to log in again or as someone else.
    void forgetAuthToken();

private slots:
    void onAuthenticationDone(bool success);
    
private:
    class Private;
    Private *const d;
};

#endif // REMEMBERSETTINGS_H
