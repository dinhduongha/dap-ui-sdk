#ifndef DAPALTERNATIVESERVERSCONTAINER_H
#define DAPALTERNATIVESERVERSCONTAINER_H

#include <QObject>
#include "DapServerInfo.h"

class DapAlternativeServersContainer : public QObject
{
public:
    DapAlternativeServersContainer(QObject* a_parent = nullptr);

    DapServerInfo takeNext(const QString& a_location = "UNKNOWN");
    void refill(const QList<DapServerInfo>& serversList);


private:
    bool hasAddress(const QString& a_allress);

    QMultiMap<QString, DapServerInfo> m_servers;
};

#endif // DAPALTERNATIVESERVERSCONTAINER_H
